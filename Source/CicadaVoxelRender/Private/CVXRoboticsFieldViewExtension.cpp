#include "CVXRoboticsFieldViewExtension.h"

#include "CVXGPUPageUploadBridge.h"
#include "CVXRoboticsFieldBridge.h"
#include "CVXRoboticsFieldShader.h"

#include "PostProcess/PostProcessMaterialInputs.h"
#include "ScreenPass.h"
#include "PixelShaderUtils.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "RenderGraphResources.h"
#include "RHIStaticStates.h"

FCVXRoboticsFieldViewExtension::FCVXRoboticsFieldViewExtension(const FAutoRegister& AutoRegister)
    : FSceneViewExtensionBase(AutoRegister)
{
}

void FCVXRoboticsFieldViewExtension::SubscribeToPostProcessingPass(
    EPostProcessingPass Pass,
    const FSceneView& InView,
    FPostProcessingPassDelegateArray& InOutPassCallbacks,
    bool bIsPassEnabled)
{
    if (!bIsPassEnabled)
    {
        return;
    }

    if (Pass == EPostProcessingPass::BeforeDOF)
    {
        InOutPassCallbacks.Add(
            FPostProcessingPassDelegate::CreateRaw(
                this,
                &FCVXRoboticsFieldViewExtension::RenderRoboticsField));
    }
}

void FCVXRoboticsFieldViewExtension::EnsurePersistentBuffers(
    FRDGBuilder& GraphBuilder,
    FRDGBufferRef& OutPageHash,
    FRDGBufferRef& OutPageMeta,
    FRDGBufferRef& OutChunkStates,
    FRDGBufferRef& OutBlockMasks)
{
    auto EnsureOne = [](
        TRefCountPtr<FRDGPooledBuffer>& Buffer,
        uint32 NumWords,
        const TCHAR* Name)
    {
        FRDGBufferDesc Desc =
            FRDGBufferDesc::CreateStructuredDesc(sizeof(uint32), NumWords);

        Desc.Usage |= EBufferUsageFlags::ShaderResource;
        Desc.Usage |= EBufferUsageFlags::UnorderedAccess;

        AllocatePooledBuffer(
            Desc,
            Buffer,
            Name,
            ERDGPooledBufferAlignment::None);
    };

    EnsureOne(
        Persistent.PageHash,
        FCVXGPUPageUploadBridge::PageHashWords,
        TEXT("CVX004B_PersistentPageHash"));

    EnsureOne(
        Persistent.PageMeta,
        FCVXGPUPageUploadBridge::PageMetaWords,
        TEXT("CVX004B_PersistentPageMeta"));

    EnsureOne(
        Persistent.ChunkStates,
        FCVXGPUPageUploadBridge::ChunkStateWords,
        TEXT("CVX004B_PersistentChunkStates"));

    EnsureOne(
        Persistent.BlockMasks,
        FCVXGPUPageUploadBridge::BlockMaskWords,
        TEXT("CVX004B_PersistentBlockMasks"));

    OutPageHash = GraphBuilder.RegisterExternalBuffer(
        Persistent.PageHash,
        TEXT("CVX004B_PageHash"),
        ERDGBufferFlags::MultiFrame);

    OutPageMeta = GraphBuilder.RegisterExternalBuffer(
        Persistent.PageMeta,
        TEXT("CVX004B_PageMeta"),
        ERDGBufferFlags::MultiFrame);

    OutChunkStates = GraphBuilder.RegisterExternalBuffer(
        Persistent.ChunkStates,
        TEXT("CVX004B_ChunkStates"),
        ERDGBufferFlags::MultiFrame);

    OutBlockMasks = GraphBuilder.RegisterExternalBuffer(
        Persistent.BlockMasks,
        TEXT("CVX004B_BlockMasks"),
        ERDGBufferFlags::MultiFrame);

    const FCVXGPUPersistentFieldStats Stats =
        FCVXGPUPageUploadBridge::GetPersistentFieldStats();

    if (!Persistent.bInitialized
        || Persistent.LastResetSerial != Stats.ResetSerial)
    {
        Persistent.bInitialized = true;
        Persistent.LastResetSerial = Stats.ResetSerial;

        AddClearUAVPass(
            GraphBuilder,
            GraphBuilder.CreateUAV(
                FRDGBufferUAVDesc(OutPageHash),
                ERDGUnorderedAccessViewFlags::None),
            0u,
            ERDGPassFlags::Compute);

        AddClearUAVPass(
            GraphBuilder,
            GraphBuilder.CreateUAV(
                FRDGBufferUAVDesc(OutPageMeta),
                ERDGUnorderedAccessViewFlags::None),
            0u,
            ERDGPassFlags::Compute);

        // State/mask data is ignored until the corresponding explicit-chunk bit
        // is set, but clearing on reset makes debugging deterministic and costs
        // a fixed amount only when a field session starts.
        AddClearUAVPass(
            GraphBuilder,
            GraphBuilder.CreateUAV(
                FRDGBufferUAVDesc(OutChunkStates),
                ERDGUnorderedAccessViewFlags::None),
            0u,
            ERDGPassFlags::Compute);

        AddClearUAVPass(
            GraphBuilder,
            GraphBuilder.CreateUAV(
                FRDGBufferUAVDesc(OutBlockMasks),
                ERDGUnorderedAccessViewFlags::None),
            0u,
            ERDGPassFlags::Compute);
    }
}

void FCVXRoboticsFieldViewExtension::ApplyPendingFieldPatches(
    FRDGBuilder& GraphBuilder,
    const FSceneView& View,
    FRDGBufferRef PageHash,
    FRDGBufferRef PageMeta,
    FRDGBufferRef ChunkStates,
    FRDGBufferRef BlockMasks)
{
    TArray<uint32> PatchWords;
    uint32 PatchCount = 0;

    FCVXGPUPageUploadBridge::DrainCoalescedPatchWords(
        PatchWords,
        PatchCount);

    if (PatchCount == 0u || PatchWords.Num() == 0)
    {
        return;
    }

    FRDGBufferRef PatchBuffer =
        CreateStructuredBuffer(
            GraphBuilder,
            TEXT("CVX004B_CoalescedPatchWords"),
            MoveTemp(PatchWords));

    FCVXApplyFieldPatchesCS::FParameters* Parameters =
        GraphBuilder.AllocParameters<FCVXApplyFieldPatchesCS::FParameters>();

    Parameters->PatchCount = PatchCount;
    Parameters->PatchStrideWords = FCVXGPUPageUploadBridge::PatchStrideWords;
    Parameters->PatchWords =
        GraphBuilder.CreateSRV(FRDGBufferSRVDesc(PatchBuffer));

    Parameters->PageHashRW =
        GraphBuilder.CreateUAV(
            FRDGBufferUAVDesc(PageHash),
            ERDGUnorderedAccessViewFlags::None);

    Parameters->PageMetaRW =
        GraphBuilder.CreateUAV(
            FRDGBufferUAVDesc(PageMeta),
            ERDGUnorderedAccessViewFlags::None);

    Parameters->ChunkStateWordsRW =
        GraphBuilder.CreateUAV(
            FRDGBufferUAVDesc(ChunkStates),
            ERDGUnorderedAccessViewFlags::None);

    Parameters->BlockMasksRW =
        GraphBuilder.CreateUAV(
            FRDGBufferUAVDesc(BlockMasks),
            ERDGUnorderedAccessViewFlags::None);

    TShaderMapRef<FCVXApplyFieldPatchesCS> ComputeShader(
        GetGlobalShaderMap(View.GetFeatureLevel()));

    FComputeShaderUtils::AddPass(
        GraphBuilder,
        RDG_EVENT_NAME("CVX004B Apply %u Coalesced Field Patches", PatchCount),
        ComputeShader,
        Parameters,
        FComputeShaderUtils::GetGroupCount(int32(PatchCount), 64));
}

FScreenPassTexture FCVXRoboticsFieldViewExtension::RenderRoboticsField(
    FRDGBuilder& GraphBuilder,
    const FSceneView& View,
    const FPostProcessMaterialInputs& Inputs)
{
    const TSharedPtr<const FCVXRoboticsFieldSnapshot, ESPMode::ThreadSafe> Snapshot =
        FCVXRoboticsFieldBridge::GetLatestSnapshot();

    if (!Snapshot.IsValid()
        || !Snapshot->bActive
        || Snapshot->LogicalSizeMM.GetMin() <= 0)
    {
        return Inputs.ReturnUntouchedSceneColorForPostProcessing(GraphBuilder);
    }

    const FScreenPassTextureSlice SceneColorSlice =
        Inputs.GetInput(EPostProcessMaterialInput::SceneColor);

    if (!SceneColorSlice.IsValid())
    {
        return Inputs.ReturnUntouchedSceneColorForPostProcessing(GraphBuilder);
    }

    const FScreenPassTexture SceneColor(SceneColorSlice);
    if (!SceneColor.IsValid())
    {
        return Inputs.ReturnUntouchedSceneColorForPostProcessing(GraphBuilder);
    }

    FRDGBufferRef PageHashBuffer = nullptr;
    FRDGBufferRef PageMetaBuffer = nullptr;
    FRDGBufferRef ChunkStatesBuffer = nullptr;
    FRDGBufferRef BlockMasksBuffer = nullptr;

    EnsurePersistentBuffers(
        GraphBuilder,
        PageHashBuffer,
        PageMetaBuffer,
        ChunkStatesBuffer,
        BlockMasksBuffer);

    ApplyPendingFieldPatches(
        GraphBuilder,
        View,
        PageHashBuffer,
        PageMetaBuffer,
        ChunkStatesBuffer,
        BlockMasksBuffer);

    FScreenPassRenderTarget Output =
        FScreenPassRenderTarget::CreateFromInput(
            GraphBuilder,
            SceneColor,
            ERenderTargetLoadAction::ENoAction,
            TEXT("CVX004B_PersistentFieldOutput"));

    AddDrawTexturePass(
        GraphBuilder,
        FScreenPassViewInfo(View),
        SceneColor,
        Output);

    const FIntRect ViewRect = SceneColor.ViewRect;
    const FVector2D TL(double(ViewRect.Min.X) + 0.5, double(ViewRect.Min.Y) + 0.5);
    const FVector2D TR(double(ViewRect.Max.X) - 0.5, double(ViewRect.Min.Y) + 0.5);
    const FVector2D BL(double(ViewRect.Min.X) + 0.5, double(ViewRect.Max.Y) - 0.5);
    const FVector2D BR(double(ViewRect.Max.X) - 0.5, double(ViewRect.Max.Y) - 0.5);

    FVector OriginTL, DirTL;
    FVector OriginTR, DirTR;
    FVector OriginBL, DirBL;
    FVector OriginBR, DirBR;

    View.DeprojectFVector2D(TL, OriginTL, DirTL);
    View.DeprojectFVector2D(TR, OriginTR, DirTR);
    View.DeprojectFVector2D(BL, OriginBL, DirBL);
    View.DeprojectFVector2D(BR, OriginBR, DirBR);

    const FTransform& WorldToLocal = Snapshot->WorldToLocal;
    const FVector LocalOriginCM = WorldToLocal.TransformPosition(OriginTL);

    const FVector LocalDirTL = WorldToLocal.TransformVector(DirTL).GetSafeNormal();
    const FVector LocalDirTR = WorldToLocal.TransformVector(DirTR).GetSafeNormal();
    const FVector LocalDirBL = WorldToLocal.TransformVector(DirBL).GetSafeNormal();
    const FVector LocalDirBR = WorldToLocal.TransformVector(DirBR).GetSafeNormal();

    FCVXRoboticsFieldPS::FParameters* Parameters =
        GraphBuilder.AllocParameters<FCVXRoboticsFieldPS::FParameters>();

    Parameters->ViewRectMinSize = FVector4f(
        float(ViewRect.Min.X),
        float(ViewRect.Min.Y),
        float(ViewRect.Width()),
        float(ViewRect.Height()));

    Parameters->LogicalSizeMM_PageEdge = FVector4f(
        float(Snapshot->LogicalSizeMM.X),
        float(Snapshot->LogicalSizeMM.Y),
        float(Snapshot->LogicalSizeMM.Z),
        float(FCVXRoboticsFieldSnapshot::PageEdgeMM));

    Parameters->PageCounts = FVector4f(
        float(Snapshot->PageCounts.X),
        float(Snapshot->PageCounts.Y),
        float(Snapshot->PageCounts.Z),
        0.0f);

    Parameters->RayOriginMM = FVector4f(
        float(LocalOriginCM.X * 10.0),
        float(LocalOriginCM.Y * 10.0),
        float(LocalOriginCM.Z * 10.0),
        0.0f);

    Parameters->RayDirTL = FVector4f(FVector3f(LocalDirTL), 0.0f);
    Parameters->RayDirTR = FVector4f(FVector3f(LocalDirTR), 0.0f);
    Parameters->RayDirBL = FVector4f(FVector3f(LocalDirBL), 0.0f);
    Parameters->RayDirBR = FVector4f(FVector3f(LocalDirBR), 0.0f);

    Parameters->PageHashCapacity = FCVXGPUPageUploadBridge::HashCapacity;
    Parameters->PageCapacity = FCVXGPUPageUploadBridge::PageCapacity;

    Parameters->PageHash =
        GraphBuilder.CreateSRV(FRDGBufferSRVDesc(PageHashBuffer));

    Parameters->PageMeta =
        GraphBuilder.CreateSRV(FRDGBufferSRVDesc(PageMetaBuffer));

    Parameters->ChunkStateWords =
        GraphBuilder.CreateSRV(FRDGBufferSRVDesc(ChunkStatesBuffer));

    Parameters->BlockMasks =
        GraphBuilder.CreateSRV(FRDGBufferSRVDesc(BlockMasksBuffer));

    Parameters->RenderTargets[0] = Output.GetRenderTargetBinding();

    TShaderMapRef<FCVXRoboticsFieldPS> PixelShader(
        GetGlobalShaderMap(View.GetFeatureLevel()));

    FPixelShaderUtils::AddFullscreenPass(
        GraphBuilder,
        GetGlobalShaderMap(View.GetFeatureLevel()),
        RDG_EVENT_NAME("CVX004B Persistent Direct Field"),
        PixelShader,
        Parameters,
        ViewRect,
        TStaticBlendState<
            CW_RGBA,
            BO_Add, BF_SourceAlpha, BF_InverseSourceAlpha,
            BO_Add, BF_One, BF_InverseSourceAlpha>::GetRHI(),
        TStaticRasterizerState<>::GetRHI(),
        TStaticDepthStencilState<false, CF_Always>::GetRHI(),
        0,
        ERDGPassFlags::Raster);

    return Output;
}
