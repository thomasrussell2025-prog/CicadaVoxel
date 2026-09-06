#include "CicadaVoxelVolumeActor.h"
#include "CVXGPUBridge.h"
#include "CVXGPUPageUploadBridge.h"
#include "CVXRoboticsFieldBridge.h"
#include "CVXSurfaceMesher.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "Engine/StaticMesh.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformTime.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Async/Async.h"

DEFINE_LOG_CATEGORY_STATIC(LogCicadaVoxelActor, Log, All);

namespace
{
    constexpr int32 CVXChunkMM = CVX::ChunkEdgeVoxels; // 16 mm
    constexpr int32 CVXBlockMM = CVX::BlockEdgeVoxels; // 4 mm
    constexpr double CVXHiddenScale = 0.000001;

    static const FIntVector CVXNeighbours[6] =
    {
        FIntVector(1, 0, 0), FIntVector(-1, 0, 0),
        FIntVector(0, 1, 0), FIntVector(0, -1, 0),
        FIntVector(0, 0, 1), FIntVector(0, 0, -1)
    };
}

ACicadaVoxelVolumeActor::ACicadaVoxelVolumeActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CVXRoot"));
    SetRootComponent(SceneRoot);

    CoarseChunkInstances = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("CVX_Coarse16mm"));
    CoarseChunkInstances->SetupAttachment(SceneRoot);

    ExactColdShellInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CVX_ExactColdShell16mmISM"));
    ExactColdShellInstances->SetupAttachment(SceneRoot);

    FrontierChunkInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CVX_SolidFrontier16mmISM"));
    FrontierChunkInstances->SetupAttachment(SceneRoot);

    FullBlockInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CVX_Hot4mmISM"));
    FullBlockInstances->SetupAttachment(SceneRoot);

    FineVoxelInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CVX_Hot1mmISM"));
    FineVoxelInstances->SetupAttachment(SceneRoot);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMeshFinder.Succeeded())
    {
        CoarseChunkInstances->SetStaticMesh(CubeMeshFinder.Object);
        ExactColdShellInstances->SetStaticMesh(CubeMeshFinder.Object);
        FrontierChunkInstances->SetStaticMesh(CubeMeshFinder.Object);
        FullBlockInstances->SetStaticMesh(CubeMeshFinder.Object);
        FineVoxelInstances->SetStaticMesh(CubeMeshFinder.Object);
    }

    UInstancedStaticMeshComponent* Components[] =
    {
        CoarseChunkInstances,
        ExactColdShellInstances,
        FrontierChunkInstances,
        FullBlockInstances,
        FineVoxelInstances
    };

    for (UInstancedStaticMeshComponent* Component : Components)
    {
        Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Component->SetCanEverAffectNavigation(false);
        Component->SetGenerateOverlapEvents(false);
        Component->SetMobility(EComponentMobility::Movable);
        Component->CastShadow = true;
    }

    // Cold shell = HISM: static hierarchy and coarse culling are valuable here.
    // Hot 4 mm / 1 mm geometry = plain ISM: no HISM cluster tree to rebuild on destruction.
    CoarseChunkInstances->bAutoRebuildTreeOnInstanceChanges = false;

    // Tiny 1 mm shadow casters amplify aliasing and cost while adding almost no useful
    // lighting information. The larger 4/16 mm representations still cast shadows.
    FineVoxelInstances->CastShadow = false;
}

#if WITH_EDITOR
uint32 ACicadaVoxelVolumeActor::BuildEditorPreviewSignature() const
{
    uint32 Signature = GetTypeHash(LogicalSizeMM);
    Signature = HashCombine(Signature, GetTypeHash(ProofCenterMM));
    Signature = HashCombine(Signature, GetTypeHash(ProofSphereRadiusMM));
    Signature = HashCombine(Signature, GetTypeHash(bInclude1mmWitnessInDefaultProof));
    Signature = HashCombine(Signature, GetTypeHash(WitnessPatchOriginMM));
    Signature = HashCombine(Signature, GetTypeHash(WitnessPatchSizeMM));
    Signature = HashCombine(Signature, GetTypeHash(FineVoxelVisualScale));
    Signature = HashCombine(Signature, GetTypeHash(bRefineExposedHotSurfacesTo1mm));
    Signature = HashCombine(Signature, GetTypeHash(bCullOccludedFineVoxels));
    Signature = HashCombine(Signature, GetTypeHash(SolidFrontierDepthChunks));
    Signature = HashCombine(Signature, GetTypeHash(MaxVisualInstances));
    Signature = HashCombine(Signature, GetTypeHash(static_cast<uint8>(EditorPreviewMode)));
    Signature = HashCombine(Signature, GetTypeHash(bEnableEditorPreview));
    return Signature;
}

void ACicadaVoxelVolumeActor::ApplyEditorPreview()
{
    UWorld* World = GetWorld();
    if (!World || World->IsGameWorld())
    {
        return;
    }

    if (!bEnableEditorPreview || EditorPreviewMode == ECVXEditorPreviewMode::None)
    {
        if (CoarseChunkInstances) CoarseChunkInstances->ClearInstances();
        if (ExactColdShellInstances) ExactColdShellInstances->ClearInstances();
        if (FrontierChunkInstances) FrontierChunkInstances->ClearInstances();
        if (FullBlockInstances) FullBlockInstances->ClearInstances();
        if (FineVoxelInstances) FineVoxelInstances->ClearInstances();
        ResetVisualSlotCaches();
        LastEditorPreview = TEXT("Editor preview disabled");
        return;
    }

    switch (EditorPreviewMode)
    {
    case ECVXEditorPreviewMode::DefaultProof:
        RunDefaultProof();
        LastEditorPreview = TEXT("EDITOR PREVIEW: default crater + exact 1 mm witness");
        break;

    case ECVXEditorPreviewMode::Witness1mm:
    default:
        Run1mmWitnessProof();
        LastEditorPreview = TEXT("EDITOR PREVIEW: exact 1 mm checkerboard witness");
        break;
    }
}
#endif

void ACicadaVoxelVolumeActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

#if WITH_EDITOR
    UWorld* World = GetWorld();
    if (!World || World->IsGameWorld())
    {
        return;
    }

    const uint32 Signature = BuildEditorPreviewSignature();
    if (bHasEditorPreviewSignature && Signature == LastEditorPreviewSignature)
    {
        return;
    }

    LastEditorPreviewSignature = Signature;
    bHasEditorPreviewSignature = true;
    ApplyEditorPreview();
#endif
}

void ACicadaVoxelVolumeActor::BeginPlay()
{
    Super::BeginPlay();
    EnsureInitialized();

    if (IsRoboticsDirectFieldRuntimeActive())
    {
        FCVXGPUPageUploadBridge::ResetPersistentField(uint32(GetUniqueID()));
    }

    if (bAutoRunProofOnBeginPlay)
    {
        RunDefaultProof();
    }
    else
    {
        RebuildVisualization();
    }

    if (IsRoboticsDirectFieldRuntimeActive())
    {
        DisableLegacyRuntimeVisualsForRobotics();
        SubmitPersistentGPUStateBootstrap();
        MarkRoboticsFieldDirty();
        FlushRoboticsFieldSnapshot(0.0f, true);

        LastPerformanceStats =
            TEXT("CVX-004A R4 FORCE DIRECT | legacy HISM/ISM/DynamicMesh runtime skin = HARD OFF");

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                0xC004A004ull,
                12.0f,
                FColor::Green,
                TEXT("CVX-004A R4 DIRECT FIELD ACTIVE | LEGACY SKIN HARD OFF"));
        }
    }
}

void ACicadaVoxelVolumeActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    FCVXRoboticsFieldBridge::ClearSnapshot(uint32(GetUniqueID()));
    Super::EndPlay(EndPlayReason);
}

void ACicadaVoxelVolumeActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bEnableFirstPersonCarving)
    {
        TryCarveFromPlayerView(DeltaSeconds);
    }

    if (IsRoboticsDirectFieldRuntimeActive())
    {
        // CVX-004B: topology updates are persistent GPU deltas. Do not rescan the
        // whole sparse field or recreate field buffers while editing.
        FlushRuntimeStatsPublish(DeltaSeconds);
        return;
    }

    const bool bDeferReferenceSurface =
        bThroughputFirstEditing
        && bDeferReferenceSurfaceWhileBeam
        && bBeamWasDownLastFrame;

    if (IsPageNativeRuntimeActive())
    {
        ProcessPendingExteriorSurfaceUpdates();
        ProcessPendingSurfacePageUpdates();
        EnforceReferenceSurfaceBudget();
    }
    else if (IsExactSurfaceRuntimeActive())
    {
        if (!bDeferReferenceSurface)
        {
            ProcessPendingSurfaceMeshUpdates();
        }
    }
    else if (bUseIncrementalVisualUpdates)
    {
        if (!bDeferReferenceSurface)
        {
            ProcessPendingVisualUpdates();
        }
    }

    FlushRuntimeStatsPublish(DeltaSeconds);
    MaintainAsyncVisualTrees();
}

void ACicadaVoxelVolumeActor::EnsureInitialized()
{
    if (!bInitialized)
    {
        Volume.Reset(LogicalSizeMM, 1);
        VirtualPageSpine.Reset();
        bInitialized = true;
        RefreshStatsAndPublish();
    }
}

void ACicadaVoxelVolumeActor::ScheduleRuntimeStatsPublish()
{
    bRuntimeStatsPublishPending = true;
}

void ACicadaVoxelVolumeActor::FlushRuntimeStatsPublish(float DeltaSeconds)
{
    if (!bRuntimeStatsPublishPending)
    {
        RuntimeStatsPublishAccumulator = 0.0f;
        return;
    }

    RuntimeStatsPublishAccumulator += DeltaSeconds;
    if (RuntimeStatsPublishAccumulator >= FMath::Max(0.02f, RuntimeStatsPublishInterval))
    {
        RefreshStatsAndPublish();
        bRuntimeStatsPublishPending = false;
        RuntimeStatsPublishAccumulator = 0.0f;
    }
}

void ACicadaVoxelVolumeActor::ResetProof()
{
    BeamEditAccumulator = 0.0f;
    bBeamWasDownLastFrame = false;
    BeamCarvesThisHold = 0;
    BeamThrottleFrames = 0;
    SurfaceProxyRetirements = 0;
    PendingSurfacePageQueue.Reset();
    PendingSurfacePageSet.Reset();
    PendingSurfacePageReadIndex = 0;
    PendingSurfacePageCount = 0;
    LastSurfacePagesProcessed = 0;
    LastSurfacePageMS = 0.0f;
    LastSurfacePageQuads = 0;
    LastActualGPUDeltaBytes = 0;
    TotalActualGPUDeltaBytes = 0;
    GPUDeltaPacketCount = 0;
    MacroBlastCount = 0;
    LastBulkPagesCleared = 0;
    LastBulkChunksCleared = 0;
    LastBulkBlocksCleared = 0;
    LastBulkBoundaryBlocks = 0;
    LastBulkSubBlocks2mmCleared = 0;
    LastBulkFineVoxelTests = 0;
    LastBeamCarvesThisFrame = 0;
    ClearPageSurfaceRuntime();
    Volume.Reset(LogicalSizeMM, 1);
    VirtualPageSpine.Reset();
    bInitialized = true;
    RefreshStatsAndPublish();
    RebuildVisualization();
}

void ACicadaVoxelVolumeActor::RunDefaultProof()
{
    Volume.Reset(LogicalSizeMM, 1);
    bInitialized = true;

    Volume.CarveSphere(FVector3d(ProofCenterMM.X, ProofCenterMM.Y, ProofCenterMM.Z), ProofSphereRadiusMM);

    if (bInclude1mmWitnessInDefaultProof)
    {
        Apply1mmWitnessPattern();
    }

    RebuildVirtualPageSpine();
    RefreshStatsAndPublish();
    RebuildVisualization();
}

int32 ACicadaVoxelVolumeActor::Apply1mmWitnessPattern()
{
    const FIntVector Size = Volume.GetLogicalSizeMM();
    const int32 PatchSize = FMath::Clamp(WitnessPatchSizeMM, 4, 32);

    const int32 X = FMath::Clamp(WitnessPatchOriginMM.X, 0, Size.X - 1);
    const int32 StartY = FMath::Clamp(WitnessPatchOriginMM.Y, 0, FMath::Max(0, Size.Y - PatchSize));
    const int32 StartZ = FMath::Clamp(WitnessPatchOriginMM.Z, 0, FMath::Max(0, Size.Z - PatchSize));

    int32 ExactCarves = 0;
    for (int32 Z = 0; Z < PatchSize; ++Z)
    {
        for (int32 Y = 0; Y < PatchSize; ++Y)
        {
            if (((Y + Z) & 1) == 0)
            {
                ExactCarves += Volume.CarveVoxel(FIntVector(X, StartY + Y, StartZ + Z)) ? 1 : 0;
            }
        }
    }

    const FIntVector WitnessRemoved(X, StartY, StartZ);
    const FIntVector WitnessNeighbour(FMath::Min(X + 1, Size.X - 1), StartY, StartZ);

    Last1mmWitness = FString::Printf(
        TEXT("EXACT 1mm witness | carvedCells=%d | removed[%d,%d,%d]=%s | neighbour[%d,%d,%d]=%s | patch=%dx%d mm"),
        ExactCarves,
        WitnessRemoved.X, WitnessRemoved.Y, WitnessRemoved.Z,
        Volume.IsSolid(WitnessRemoved) ? TEXT("SOLID") : TEXT("EMPTY"),
        WitnessNeighbour.X, WitnessNeighbour.Y, WitnessNeighbour.Z,
        Volume.IsSolid(WitnessNeighbour) ? TEXT("SOLID") : TEXT("EMPTY"),
        PatchSize, PatchSize);

    UE_LOG(LogCicadaVoxelActor, Display, TEXT("%s"), *Last1mmWitness);
    return ExactCarves;
}

void ACicadaVoxelVolumeActor::Run1mmWitnessProof()
{
    Volume.Reset(LogicalSizeMM, 1);
    bInitialized = true;
    Apply1mmWitnessPattern();
    RebuildVirtualPageSpine();
    RefreshStatsAndPublish();
    RebuildVisualization();
}

bool ACicadaVoxelVolumeActor::CarveVoxelLocalMM(FIntVector VoxelMM)
{
    EnsureInitialized();

    const double StartSeconds = FPlatformTime::Seconds();
    const bool bChanged = Volume.CarveVoxel(VoxelMM);
    LastSparseEditMS = float((FPlatformTime::Seconds() - StartSeconds) * 1000.0);

    if (!bChanged)
    {
        return false;
    }

    const FIntVector ChangedChunkCoord(VoxelMM.X / CVXChunkMM, VoxelMM.Y / CVXChunkMM, VoxelMM.Z / CVXChunkMM);
    const int32 ChangedLBX = (VoxelMM.X / CVXBlockMM) & (CVX::ChunkEdgeBlocks - 1);
    const int32 ChangedLBY = (VoxelMM.Y / CVXBlockMM) & (CVX::ChunkEdgeBlocks - 1);
    const int32 ChangedLBZ = (VoxelMM.Z / CVXBlockMM) & (CVX::ChunkEdgeBlocks - 1);
    const int32 ChangedBlockIndex = CVX::LocalBlockIndex(ChangedLBX, ChangedLBY, ChangedLBZ);

    TArray<FCVXChangedBlock> PageChangedBlocks;
    PageChangedBlocks.Emplace(ChangedChunkCoord, ChangedBlockIndex);
    ApplyVirtualPageChanges(PageChangedBlocks);
    SubmitGPUPageDelta(PageChangedBlocks);

    if (IsRoboticsDirectFieldRuntimeActive())
    {
        ScheduleRuntimeStatsPublish();
    }
    else if (IsPageNativeRuntimeActive())
    {
        QueueExteriorSurfacePagesFromChangedBlocks(PageChangedBlocks);
        QueueSurfacePagesFromChangedBlocks(PageChangedBlocks);
        ScheduleRuntimeStatsPublish();
    }
    else if (IsExactSurfaceRuntimeActive())
    {
        QueueSurfaceMeshChunkAndNeighbours(ChangedChunkCoord);
        ScheduleRuntimeStatsPublish();
    }
    else if (bUseIncrementalVisualUpdates)
    {
        QueueVisualBlockAndSurfaceNeighbours(ChangedChunkCoord, ChangedBlockIndex);
        ScheduleRuntimeStatsPublish();
    }
    else
    {
        RefreshStatsAndPublish();
        RebuildVisualization();
    }

    return true;
}

void ACicadaVoxelVolumeActor::CarveSphereLocalMM(FVector CenterMM, float RadiusMM)
{
    EnsureInitialized();

    const double StartSeconds = FPlatformTime::Seconds();

    FCVXEditDelta EditDelta;
    FCVXBulkEditStats BulkStats;
    TArray<FCVXChangedBlock> LegacyChangedBlocks;

    if (bUseHierarchicalBulkEdits)
    {
        Volume.CarveSphereHierarchical(
            FVector3d(CenterMM.X, CenterMM.Y, CenterMM.Z),
            double(RadiusMM),
            EditDelta,
            BulkStats);
    }
    else
    {
        Volume.CarveSphere(
            FVector3d(CenterMM.X, CenterMM.Y, CenterMM.Z),
            double(RadiusMM),
            LegacyChangedBlocks);
        EditDelta.ChangedBlocks = LegacyChangedBlocks;
    }

    LastSparseEditMS = float((FPlatformTime::Seconds() - StartSeconds) * 1000.0);
    LastBulkPagesCleared = BulkStats.PagesCleared;
    LastBulkChunksCleared = BulkStats.ChunksCleared;
    LastBulkBlocksCleared = BulkStats.BlocksCleared;
    LastBulkBoundaryBlocks = BulkStats.BoundaryBlocks;
    LastBulkSubBlocks2mmCleared = BulkStats.SubBlocks2mmCleared;
    LastBulkFineVoxelTests = BulkStats.FineVoxelTests;

    if (EditDelta.IsEmpty())
    {
        return;
    }

    ApplyVirtualPageChanges(EditDelta);
    SubmitGPUPageDelta(EditDelta);

    if (IsRoboticsDirectFieldRuntimeActive())
    {
        ScheduleRuntimeStatsPublish();
    }
    else if (IsPageNativeRuntimeActive())
    {
        QueueExteriorSurfacePagesFromEditDelta(EditDelta);
        QueueSurfacePagesFromEditDelta(EditDelta);
        ScheduleRuntimeStatsPublish();
        if (bThroughputFirstEditing && bDeferReferenceSurfaceWhileBeam && bBeamWasDownLastFrame)
        {
            UpdatePerformanceString(0);
            ShowRuntimeDebugMessage();
        }
    }
    else if (IsExactSurfaceRuntimeActive())
    {
        // The exact 16 mm reference path predates coarse page/chunk tombstones. For
        // compatibility, queue only the exact boundary block changes here. CVX-003B
        // is page-native by default and the custom GPU renderer replaces this path.
        QueueSurfaceMeshFromChangedBlocks(EditDelta.ChangedBlocks);
        if (bImmediateImpactSurfaceMesh && !bThroughputFirstEditing)
        {
            RebuildImpactSurfaceMeshNow(CenterMM);
        }
        ScheduleRuntimeStatsPublish();
    }
    else if (bUseIncrementalVisualUpdates)
    {
        QueueVisualBlocks(EditDelta.ChangedBlocks);
        if (bImmediateImpactSurfaceRefresh && !bThroughputFirstEditing)
        {
            RefreshImpactSurfaceNow(CenterMM);
        }
        ScheduleRuntimeStatsPublish();
    }
    else
    {
        RefreshStatsAndPublish();
        RebuildVisualization();
    }
}

bool ACicadaVoxelVolumeActor::IsSolidLocalMM(FIntVector VoxelMM) const
{
    return Volume.IsSolid(VoxelMM);
}

FString ACicadaVoxelVolumeActor::GetStatsString() const
{
    return Volume.GetStats().ToString();
}

void ACicadaVoxelVolumeActor::RebuildVirtualPageSpine()
{
    if (!bEnableVirtualPageSpine)
    {
        VirtualPageSpine.Reset();
        return;
    }

    VirtualPageSpine.RebuildFromVolume(Volume);
}

void ACicadaVoxelVolumeActor::ApplyVirtualPageChanges(const TArray<FCVXChangedBlock>& ChangedBlocks)
{
    if (!bEnableVirtualPageSpine || ChangedBlocks.Num() == 0)
    {
        return;
    }

    VirtualPageSpine.ApplyChangedBlocks(Volume, ChangedBlocks);
}

void ACicadaVoxelVolumeActor::ApplyVirtualPageChanges(const FCVXEditDelta& Delta)
{
    if (!bEnableVirtualPageSpine || Delta.IsEmpty())
    {
        return;
    }

    VirtualPageSpine.ApplyEditDelta(Volume, Delta);
}

void ACicadaVoxelVolumeActor::RefreshStatsAndPublish()
{
    LastStats = Volume.GetStats().ToString();
    UE_LOG(LogCicadaVoxelActor, Display, TEXT("%s"), *LastStats);

    const FCVXVirtualPageStats& PageStats = VirtualPageSpine.GetStats();
    ResidentEditedPageCount = PageStats.ResidentEditedPages;
    ExplicitPageChunkCount = PageStats.ExplicitChunks;
    LastPageDeltaBytes = static_cast<int64>(PageStats.LastDeltaBytes);
    FullWorldHotUploadCount = static_cast<int64>(PageStats.FullWorldUploadCount);
    LastVirtualPageStats = PageStats.ToString();
    if (bEnableVirtualPageSpine)
    {
        UE_LOG(LogCicadaVoxelActor, Display, TEXT("%s"), *LastVirtualPageStats);
    }

    LastLegacyGPUSnapshotMS = 0.0f;
    if (bPublishLegacyFullGPUSnapshot)
    {
        const double SnapshotStart = FPlatformTime::Seconds();
        TArray<CVX::FCVXGPUUint4> ChunkWords;
        TArray<CVX::FCVXGPUUint2> MixedMasks;
        Volume.BuildGPUData(ChunkWords, MixedMasks);
        FCVXGPUBridge::Publish(Volume.GetGeneration(), ChunkWords, MixedMasks);
        LastLegacyGPUSnapshotMS = float((FPlatformTime::Seconds() - SnapshotStart) * 1000.0);
    }
}

bool ACicadaVoxelVolumeActor::IsBoundaryChunk(const FIntVector& ChunkCoord, const FIntVector& ChunkCounts) const
{
    return ChunkCoord.X == 0 || ChunkCoord.Y == 0 || ChunkCoord.Z == 0
        || ChunkCoord.X == ChunkCounts.X - 1
        || ChunkCoord.Y == ChunkCounts.Y - 1
        || ChunkCoord.Z == ChunkCounts.Z - 1;
}

bool ACicadaVoxelVolumeActor::IsChunkCoordInBounds(const FIntVector& ChunkCoord) const
{
    const FIntVector Size = Volume.GetLogicalSizeMM();
    const FIntVector Counts(
        FMath::DivideAndRoundUp(Size.X, CVXChunkMM),
        FMath::DivideAndRoundUp(Size.Y, CVXChunkMM),
        FMath::DivideAndRoundUp(Size.Z, CVXChunkMM));

    return ChunkCoord.X >= 0 && ChunkCoord.Y >= 0 && ChunkCoord.Z >= 0
        && ChunkCoord.X < Counts.X && ChunkCoord.Y < Counts.Y && ChunkCoord.Z < Counts.Z;
}

FTransform ACicadaVoxelVolumeActor::MakeVisualBoxTransform(const FVector& MinMM, const FVector& MaxMM) const
{
    const FVector SizeMM = MaxMM - MinMM;
    if (SizeMM.X <= 0.0 || SizeMM.Y <= 0.0 || SizeMM.Z <= 0.0)
    {
        return FTransform::Identity;
    }

    const FVector CenterMM = (MinMM + MaxMM) * 0.5;
    const FVector CenterCM = CenterMM * 0.1;
    const FVector Scale(SizeMM.X * 0.001, SizeMM.Y * 0.001, SizeMM.Z * 0.001);
    return FTransform(FQuat::Identity, CenterCM, Scale);
}

FTransform ACicadaVoxelVolumeActor::MakeHiddenTransform(const FVector& CenterMM) const
{
    return FTransform(FQuat::Identity, CenterMM * 0.1, FVector(CVXHiddenScale));
}

void ACicadaVoxelVolumeActor::AddVisualBox(
    TArray<FTransform>& OutTransforms,
    const FVector& MinMM,
    const FVector& MaxMM,
    int32& InOutCount)
{
    if (InOutCount >= MaxVisualInstances)
    {
        return;
    }

    const FVector SizeMM = MaxMM - MinMM;
    if (SizeMM.X <= 0.0 || SizeMM.Y <= 0.0 || SizeMM.Z <= 0.0)
    {
        return;
    }

    OutTransforms.Add(MakeVisualBoxTransform(MinMM, MaxMM));
    ++InOutCount;
}

FTransform ACicadaVoxelVolumeActor::MakeCoarseChunkTransform(const FIntVector& ChunkCoord, bool bVisible) const
{
    const FIntVector Size = Volume.GetLogicalSizeMM();
    const FVector MinMM(
        double(ChunkCoord.X * CVXChunkMM),
        double(ChunkCoord.Y * CVXChunkMM),
        double(ChunkCoord.Z * CVXChunkMM));
    const FVector MaxMM(
        double(FMath::Min((ChunkCoord.X + 1) * CVXChunkMM, Size.X)),
        double(FMath::Min((ChunkCoord.Y + 1) * CVXChunkMM, Size.Y)),
        double(FMath::Min((ChunkCoord.Z + 1) * CVXChunkMM, Size.Z)));

    if (bVisible)
    {
        return MakeVisualBoxTransform(MinMM, MaxMM);
    }

    return MakeHiddenTransform((MinMM + MaxMM) * 0.5);
}

FTransform ACicadaVoxelVolumeActor::BuildBlockSlotTransform(const FIntVector& ChunkCoord, int32 BlockIndex) const
{
    const FIntVector Size = Volume.GetLogicalSizeMM();
    const FIntVector ChunkOrigin(ChunkCoord.X * CVXChunkMM, ChunkCoord.Y * CVXChunkMM, ChunkCoord.Z * CVXChunkMM);
    const int32 BX = BlockIndex % CVX::ChunkEdgeBlocks;
    const int32 BY = (BlockIndex / CVX::ChunkEdgeBlocks) % CVX::ChunkEdgeBlocks;
    const int32 BZ = BlockIndex / (CVX::ChunkEdgeBlocks * CVX::ChunkEdgeBlocks);
    const FIntVector BlockOrigin = ChunkOrigin + FIntVector(BX * CVXBlockMM, BY * CVXBlockMM, BZ * CVXBlockMM);

    const FVector MinMM(double(BlockOrigin.X), double(BlockOrigin.Y), double(BlockOrigin.Z));
    const FVector MaxMM(
        double(FMath::Min(BlockOrigin.X + CVXBlockMM, Size.X)),
        double(FMath::Min(BlockOrigin.Y + CVXBlockMM, Size.Y)),
        double(FMath::Min(BlockOrigin.Z + CVXBlockMM, Size.Z)));
    const FVector CenterMM = (MinMM + MaxMM) * 0.5;

    if (BlockOrigin.X >= Size.X || BlockOrigin.Y >= Size.Y || BlockOrigin.Z >= Size.Z)
    {
        return MakeHiddenTransform(CenterMM);
    }

    const CVX::EBlockState State = Volume.GetBlockStateForVisualization(ChunkCoord, BlockIndex);
    if (State == CVX::EBlockState::Full || State == CVX::EBlockState::Procedural)
    {
        return MakeVisualBoxTransform(MinMM, MaxMM);
    }

    return MakeHiddenTransform(CenterMM);
}

void ACicadaVoxelVolumeActor::BuildBlockSlotTransforms(const FIntVector& ChunkCoord, TArray<FTransform>& OutTransforms) const
{
    OutTransforms.Reset(CVX::BlocksPerChunk);
    for (int32 BlockIndex = 0; BlockIndex < CVX::BlocksPerChunk; ++BlockIndex)
    {
        OutTransforms.Add(BuildBlockSlotTransform(ChunkCoord, BlockIndex));
    }
}

void ACicadaVoxelVolumeActor::BuildFineSlotTransforms(
    const FIntVector& ChunkCoord,
    int32 BlockIndex,
    uint64 SolidMask,
    TArray<FTransform>& OutTransforms) const
{
    OutTransforms.Reset(CVX::VoxelsPerBlock);

    const FIntVector Size = Volume.GetLogicalSizeMM();
    const FIntVector ChunkOrigin(ChunkCoord.X * CVXChunkMM, ChunkCoord.Y * CVXChunkMM, ChunkCoord.Z * CVXChunkMM);
    const int32 BX = BlockIndex % CVX::ChunkEdgeBlocks;
    const int32 BY = (BlockIndex / CVX::ChunkEdgeBlocks) % CVX::ChunkEdgeBlocks;
    const int32 BZ = BlockIndex / (CVX::ChunkEdgeBlocks * CVX::ChunkEdgeBlocks);
    const FIntVector BlockOrigin = ChunkOrigin + FIntVector(BX * CVXBlockMM, BY * CVXBlockMM, BZ * CVXBlockMM);

    double FineScale = FMath::Clamp(double(FineVoxelVisualScale), 0.55, 1.0);
    if (bForceSeamlessRuntimeFineVoxels && GetWorld() && GetWorld()->IsGameWorld())
    {
        FineScale = 1.0;
    }
    const FVector HalfExtentMM(0.5 * FineScale);

    for (int32 Bit = 0; Bit < CVX::VoxelsPerBlock; ++Bit)
    {
        const int32 VX = Bit % CVX::BlockEdgeVoxels;
        const int32 VY = (Bit / CVX::BlockEdgeVoxels) % CVX::BlockEdgeVoxels;
        const int32 VZ = Bit / (CVX::BlockEdgeVoxels * CVX::BlockEdgeVoxels);
        const FIntVector Voxel = BlockOrigin + FIntVector(VX, VY, VZ);
        const FVector CenterMM(double(Voxel.X) + 0.5, double(Voxel.Y) + 0.5, double(Voxel.Z) + 0.5);

        const bool bInside = Voxel.X >= 0 && Voxel.Y >= 0 && Voxel.Z >= 0
            && Voxel.X < Size.X && Voxel.Y < Size.Y && Voxel.Z < Size.Z;
        const bool bSolid = bInside && (SolidMask & (uint64(1) << Bit)) != 0ull;

        bool bVisibleSolid = bSolid;
        if (bVisibleSolid && bCullOccludedFineVoxels)
        {
            bool bExposed = false;
            for (const FIntVector& Delta : CVXNeighbours)
            {
                if (!Volume.IsSolid(Voxel + Delta))
                {
                    bExposed = true;
                    break;
                }
            }
            bVisibleSolid = bExposed;
        }

        if (bVisibleSolid)
        {
            OutTransforms.Add(MakeVisualBoxTransform(CenterMM - HalfExtentMM, CenterMM + HalfExtentMM));
        }
        else
        {
            OutTransforms.Add(MakeHiddenTransform(CenterMM));
        }
    }
}

bool ACicadaVoxelVolumeActor::GetNeighbourBlockAddress(
    const FIntVector& ChunkCoord,
    int32 BlockIndex,
    const FIntVector& BlockDelta,
    FIntVector& OutChunkCoord,
    int32& OutBlockIndex) const
{
    const int32 BX = BlockIndex % CVX::ChunkEdgeBlocks;
    const int32 BY = (BlockIndex / CVX::ChunkEdgeBlocks) % CVX::ChunkEdgeBlocks;
    const int32 BZ = BlockIndex / (CVX::ChunkEdgeBlocks * CVX::ChunkEdgeBlocks);

    FIntVector GlobalBlock(
        ChunkCoord.X * CVX::ChunkEdgeBlocks + BX + BlockDelta.X,
        ChunkCoord.Y * CVX::ChunkEdgeBlocks + BY + BlockDelta.Y,
        ChunkCoord.Z * CVX::ChunkEdgeBlocks + BZ + BlockDelta.Z);

    const FIntVector Size = Volume.GetLogicalSizeMM();
    const FIntVector BlockCounts(
        FMath::DivideAndRoundUp(Size.X, CVXBlockMM),
        FMath::DivideAndRoundUp(Size.Y, CVXBlockMM),
        FMath::DivideAndRoundUp(Size.Z, CVXBlockMM));

    if (GlobalBlock.X < 0 || GlobalBlock.Y < 0 || GlobalBlock.Z < 0
        || GlobalBlock.X >= BlockCounts.X || GlobalBlock.Y >= BlockCounts.Y || GlobalBlock.Z >= BlockCounts.Z)
    {
        return false;
    }

    OutChunkCoord = FIntVector(
        GlobalBlock.X / CVX::ChunkEdgeBlocks,
        GlobalBlock.Y / CVX::ChunkEdgeBlocks,
        GlobalBlock.Z / CVX::ChunkEdgeBlocks);

    const int32 LocalX = GlobalBlock.X % CVX::ChunkEdgeBlocks;
    const int32 LocalY = GlobalBlock.Y % CVX::ChunkEdgeBlocks;
    const int32 LocalZ = GlobalBlock.Z % CVX::ChunkEdgeBlocks;
    OutBlockIndex = CVX::LocalBlockIndex(LocalX, LocalY, LocalZ);
    return true;
}

bool ACicadaVoxelVolumeActor::ShouldRefineFullBlockTo1mm(const FIntVector& ChunkCoord, int32 BlockIndex) const
{
    if (!bRefineExposedHotSurfacesTo1mm)
    {
        return false;
    }

    const CVX::EBlockState State = Volume.GetBlockStateForVisualization(ChunkCoord, BlockIndex);
    if (State != CVX::EBlockState::Full && State != CVX::EBlockState::Procedural)
    {
        return false;
    }

    static const FIntVector BlockNeighbours[6] =
    {
        FIntVector(1, 0, 0), FIntVector(-1, 0, 0),
        FIntVector(0, 1, 0), FIntVector(0, -1, 0),
        FIntVector(0, 0, 1), FIntVector(0, 0, -1)
    };

    for (const FIntVector& Delta : BlockNeighbours)
    {
        FIntVector NeighbourChunk;
        int32 NeighbourBlock = 0;
        if (!GetNeighbourBlockAddress(ChunkCoord, BlockIndex, Delta, NeighbourChunk, NeighbourBlock))
        {
            return true;
        }

        const CVX::EBlockState NeighbourState = Volume.GetBlockStateForVisualization(NeighbourChunk, NeighbourBlock);
        if (NeighbourState == CVX::EBlockState::Empty || NeighbourState == CVX::EBlockState::Mixed)
        {
            return true;
        }
    }

    return false;
}

bool ACicadaVoxelVolumeActor::AreIndicesContiguous(const TArray<int32>& Indices) const
{
    if (Indices.Num() <= 1)
    {
        return true;
    }

    for (int32 Index = 1; Index < Indices.Num(); ++Index)
    {
        if (Indices[Index] != Indices[0] + Index)
        {
            return false;
        }
    }
    return true;
}

bool ACicadaVoxelVolumeActor::UpdateSlotTransforms(
    UInstancedStaticMeshComponent* Component,
    const TArray<int32>& Indices,
    const TArray<FTransform>& Transforms,
    bool bMarkRenderStateDirty)
{
    if (!Component || Indices.Num() == 0 || Indices.Num() != Transforms.Num())
    {
        return false;
    }

    if (AreIndicesContiguous(Indices))
    {
        return Component->BatchUpdateInstancesTransforms(
            Indices[0],
            Transforms,
            false,
            bMarkRenderStateDirty,
            false);
    }

    bool bSuccess = true;
    for (int32 Index = 0; Index < Indices.Num(); ++Index)
    {
        const bool bDirtyThisCall = bMarkRenderStateDirty && Index == Indices.Num() - 1;
        bSuccess &= Component->UpdateInstanceTransform(Indices[Index], Transforms[Index], false, bDirtyThisCall, false);
    }
    return bSuccess;
}

bool ACicadaVoxelVolumeActor::AllocateSlotBatch(
    UInstancedStaticMeshComponent* Component,
    const TArray<FTransform>& Transforms,
    TArray<int32>& OutIndices)
{
    OutIndices.Reset();

    if (!Component || Transforms.Num() == 0)
    {
        return false;
    }

    if (VisualAllocatedSlots + Transforms.Num() > MaxVisualInstances)
    {
        bVisualBudgetHit = true;
        return false;
    }

    Component->PreAllocateInstancesMemory(Transforms.Num());
    OutIndices = Component->AddInstances(Transforms, true, false, false);
    VisualAllocatedSlots += OutIndices.Num();
    return OutIndices.Num() == Transforms.Num();
}

bool ACicadaVoxelVolumeActor::AcquireFineSlotBatch(
    const TArray<FTransform>& Transforms,
    TArray<int32>& OutIndices,
    bool& bTouchedFine)
{
    OutIndices.Reset();

    if (Transforms.Num() != CVX::VoxelsPerBlock)
    {
        return false;
    }

    if (FreeFineSlotBatches.Num() > 0)
    {
        const int32 LastBatch = FreeFineSlotBatches.Num() - 1;
        OutIndices = MoveTemp(FreeFineSlotBatches[LastBatch]);
        FreeFineSlotBatches.RemoveAt(LastBatch, 1, EAllowShrinking::No);

        if (OutIndices.Num() == CVX::VoxelsPerBlock
            && UpdateSlotTransforms(FineVoxelInstances, OutIndices, Transforms, false))
        {
            ++FineSlotBatchesReused;
            bTouchedFine = true;
            return true;
        }

        OutIndices.Reset();
    }

    if (AllocateSlotBatch(FineVoxelInstances, Transforms, OutIndices))
    {
        bTouchedFine = true;
        return true;
    }

    return false;
}

void ACicadaVoxelVolumeActor::ReleaseFineSlotBatch(
    const FIntVector& ChunkCoord,
    int32 BlockIndex,
    TArray<int32>& FineIndices,
    bool& bTouchedFine)
{
    if (FineIndices.Num() != CVX::VoxelsPerBlock)
    {
        FineIndices.Reset();
        return;
    }

    TArray<FTransform> HiddenTransforms;
    BuildFineSlotTransforms(ChunkCoord, BlockIndex, 0ull, HiddenTransforms);
    if (UpdateSlotTransforms(FineVoxelInstances, FineIndices, HiddenTransforms, false))
    {
        bTouchedFine = true;
    }

    FreeFineSlotBatches.Add(MoveTemp(FineIndices));
    FineIndices.Reset();
}

void ACicadaVoxelVolumeActor::ResetVisualSlotCaches()
{
    CoarseInstanceByChunk.Reset();
    FrontierInstanceByChunk.Reset();
    HotChunkVisualSlots.Reset();
    FreeFineSlotBatches.Reset();
    PendingVisualChunkQueue.Reset();
    PendingVisualChunkReadIndex = 0;
    PendingVisualChunkSet.Reset();
    PendingVisualBlockMasks.Reset();
    PendingVisualChunkCount = 0;
    PendingVisualBlockCount = 0;
    LastVisualBlocksProcessed = 0;
    LastRefinedSurfaceBlocksProcessed = 0;
    FineSlotBatchesReused = 0;
    bCoarseTreeBuildPending = false;
    VisualAllocatedSlots = 0;
    bVisualBudgetHit = false;
}

UInstancedStaticMeshComponent* ACicadaVoxelVolumeActor::GetActiveColdShellComponent() const
{
    if ((IsPageNativeRuntimeActive() || IsExactSurfaceRuntimeActive())
        && bSingleOwnerRuntimeSurface
        && ExactColdShellInstances)
    {
        return ExactColdShellInstances;
    }

    return CoarseChunkInstances;
}

void ACicadaVoxelVolumeActor::FlushActiveColdShellChanges(bool bTouchedColdShell)
{
    if (!bTouchedColdShell)
    {
        return;
    }

    if ((IsPageNativeRuntimeActive() || IsExactSurfaceRuntimeActive())
        && bSingleOwnerRuntimeSurface
        && ExactColdShellInstances)
    {
        ExactColdShellInstances->MarkRenderInstancesDirty();
    }
    else
    {
        bCoarseTreeBuildPending = true;
    }
}

void ACicadaVoxelVolumeActor::EnsureCoarseChunkVisual(const FIntVector& ChunkCoord, bool bVisible, bool& bTouchedCoarse)
{
    if (!IsChunkCoordInBounds(ChunkCoord))
    {
        return;
    }

    UInstancedStaticMeshComponent* ColdShell = GetActiveColdShellComponent();
    if (!ColdShell)
    {
        return;
    }

    if (int32* ExistingIndex = CoarseInstanceByChunk.Find(ChunkCoord))
    {
        // Batch cold-shell ownership changes. Marking the render state dirty once per
        // 16 mm proxy caused the rectangular flicker seen around edited pages.
        ColdShell->UpdateInstanceTransform(
            *ExistingIndex,
            MakeCoarseChunkTransform(ChunkCoord, bVisible),
            false,
            false,
            false);
        bTouchedCoarse = true;
        return;
    }

    if (!bVisible || VisualAllocatedSlots + 1 > MaxVisualInstances)
    {
        if (bVisible)
        {
            bVisualBudgetHit = true;
        }
        return;
    }

    ColdShell->PreAllocateInstancesMemory(1);
    const int32 NewIndex = ColdShell->AddInstance(MakeCoarseChunkTransform(ChunkCoord, true), false);
    if (NewIndex != INDEX_NONE)
    {
        CoarseInstanceByChunk.Add(ChunkCoord, NewIndex);
        ++VisualAllocatedSlots;
        bTouchedCoarse = true;
    }
}

void ACicadaVoxelVolumeActor::EnsureFrontierChunkVisual(
    const FIntVector& ChunkCoord,
    bool bVisible,
    bool& bTouchedFrontier)
{
    if (!FrontierChunkInstances || !IsChunkCoordInBounds(ChunkCoord))
    {
        return;
    }

    if (bVisible && Volume.IsChunkDirty(ChunkCoord))
    {
        bVisible = false;
    }

    if (int32* ExistingIndex = FrontierInstanceByChunk.Find(ChunkCoord))
    {
        FrontierChunkInstances->UpdateInstanceTransform(
            *ExistingIndex,
            MakeCoarseChunkTransform(ChunkCoord, bVisible),
            false,
            false,
            false);
        bTouchedFrontier = true;
        return;
    }

    if (!bVisible || VisualAllocatedSlots + 1 > MaxVisualInstances)
    {
        if (bVisible)
        {
            bVisualBudgetHit = true;
        }
        return;
    }

    FrontierChunkInstances->PreAllocateInstancesMemory(1);
    const int32 NewIndex = FrontierChunkInstances->AddInstance(MakeCoarseChunkTransform(ChunkCoord, true), false);
    if (NewIndex != INDEX_NONE)
    {
        FrontierInstanceByChunk.Add(ChunkCoord, NewIndex);
        ++VisualAllocatedSlots;
        bTouchedFrontier = true;
    }
}

void ACicadaVoxelVolumeActor::EnsureSolidFrontierAroundChunk(
    const FIntVector& ChunkCoord,
    bool& bTouchedFrontier)
{
    const int32 Depth = FMath::Clamp(SolidFrontierDepthChunks, 1, 2);

    for (int32 DZ = -Depth; DZ <= Depth; ++DZ)
    {
        for (int32 DY = -Depth; DY <= Depth; ++DY)
        {
            for (int32 DX = -Depth; DX <= Depth; ++DX)
            {
                if (DX == 0 && DY == 0 && DZ == 0)
                {
                    continue;
                }

                const FIntVector Candidate = ChunkCoord + FIntVector(DX, DY, DZ);
                if (IsChunkCoordInBounds(Candidate) && !Volume.IsChunkDirty(Candidate))
                {
                    EnsureFrontierChunkVisual(Candidate, true, bTouchedFrontier);
                }
            }
        }
    }
}

bool ACicadaVoxelVolumeActor::EnsureHotChunkSlots(const FIntVector& ChunkCoord, bool& bTouchedBlocks)
{
    if (!IsChunkCoordInBounds(ChunkCoord))
    {
        return false;
    }

    FCVXChunkVisualSlots& Slots = HotChunkVisualSlots.FindOrAdd(ChunkCoord);
    if (Slots.FineIndicesByBlock.Num() != CVX::BlocksPerChunk)
    {
        Slots.FineIndicesByBlock.SetNum(CVX::BlocksPerChunk);
    }

    if (Slots.BlockIndices.Num() == CVX::BlocksPerChunk)
    {
        return true;
    }

    TArray<FTransform> BlockTransforms;
    BuildBlockSlotTransforms(ChunkCoord, BlockTransforms);
    TArray<int32> NewIndices;
    if (!AllocateSlotBatch(FullBlockInstances, BlockTransforms, NewIndices))
    {
        return false;
    }

    Slots.BlockIndices = MoveTemp(NewIndices);
    bTouchedBlocks = true;
    return true;
}

void ACicadaVoxelVolumeActor::UpdateHotBlockVisual(
    const FIntVector& ChunkCoord,
    int32 BlockIndex,
    bool& bTouchedBlocks,
    bool& bTouchedFine)
{
    if (BlockIndex < 0 || BlockIndex >= CVX::BlocksPerChunk)
    {
        return;
    }

    if (!EnsureHotChunkSlots(ChunkCoord, bTouchedBlocks))
    {
        return;
    }

    FCVXChunkVisualSlots& Slots = HotChunkVisualSlots.FindChecked(ChunkCoord);
    TArray<int32>& FineIndices = Slots.FineIndicesByBlock[BlockIndex];
    const CVX::EBlockState State = Volume.GetBlockStateForVisualization(ChunkCoord, BlockIndex);
    const bool bRefinedFull = ShouldRefineFullBlockTo1mm(ChunkCoord, BlockIndex);
    const bool bNeedsFine = State == CVX::EBlockState::Mixed || bRefinedFull;

    // The 4 mm proxy is visible only for buried full/procedural blocks. Any block that is
    // actually exposed to a cavity is rendered at exact 1 mm resolution instead.
    FTransform BlockTransform = BuildBlockSlotTransform(ChunkCoord, BlockIndex);
    if (bNeedsFine)
    {
        const FIntVector ChunkOrigin(ChunkCoord.X * CVXChunkMM, ChunkCoord.Y * CVXChunkMM, ChunkCoord.Z * CVXChunkMM);
        const int32 BX = BlockIndex % CVX::ChunkEdgeBlocks;
        const int32 BY = (BlockIndex / CVX::ChunkEdgeBlocks) % CVX::ChunkEdgeBlocks;
        const int32 BZ = BlockIndex / (CVX::ChunkEdgeBlocks * CVX::ChunkEdgeBlocks);
        const FIntVector BlockOrigin = ChunkOrigin + FIntVector(BX * CVXBlockMM, BY * CVXBlockMM, BZ * CVXBlockMM);
        BlockTransform = MakeHiddenTransform(FVector(double(BlockOrigin.X) + 2.0, double(BlockOrigin.Y) + 2.0, double(BlockOrigin.Z) + 2.0));
    }

    FullBlockInstances->UpdateInstanceTransform(
        Slots.BlockIndices[BlockIndex],
        BlockTransform,
        false,
        false,
        false);
    bTouchedBlocks = true;

    if (bNeedsFine)
    {
        uint64 FineMask = CVX::FullBlockMask;
        if (State == CVX::EBlockState::Mixed)
        {
            FineMask = Volume.GetBlockMaskForVisualization(ChunkCoord, BlockIndex);
        }
        else if (bRefinedFull)
        {
            ++LastRefinedSurfaceBlocksProcessed;
        }

        TArray<FTransform> FineTransforms;
        BuildFineSlotTransforms(ChunkCoord, BlockIndex, FineMask, FineTransforms);

        if (FineIndices.Num() != CVX::VoxelsPerBlock)
        {
            TArray<int32> NewFineIndices;
            if (AcquireFineSlotBatch(FineTransforms, NewFineIndices, bTouchedFine))
            {
                FineIndices = MoveTemp(NewFineIndices);
            }
        }
        else
        {
            bTouchedFine |= UpdateSlotTransforms(FineVoxelInstances, FineIndices, FineTransforms, false);
        }
        return;
    }

    if (FineIndices.Num() == CVX::VoxelsPerBlock)
    {
        ReleaseFineSlotBatch(ChunkCoord, BlockIndex, FineIndices, bTouchedFine);
    }
}

void ACicadaVoxelVolumeActor::UpdateHotChunkVisual(const FIntVector& ChunkCoord, bool& bTouchedBlocks, bool& bTouchedFine)
{
    if (!EnsureHotChunkSlots(ChunkCoord, bTouchedBlocks))
    {
        return;
    }

    for (int32 BlockIndex = 0; BlockIndex < CVX::BlocksPerChunk; ++BlockIndex)
    {
        UpdateHotBlockVisual(ChunkCoord, BlockIndex, bTouchedBlocks, bTouchedFine);
    }
}

void ACicadaVoxelVolumeActor::RefreshImpactSurfaceNow(const FVector& CenterMM)
{
    const double StartSeconds = FPlatformTime::Seconds();

    const FIntVector GlobalBlock(
        FMath::FloorToInt(CenterMM.X / double(CVXBlockMM)),
        FMath::FloorToInt(CenterMM.Y / double(CVXBlockMM)),
        FMath::FloorToInt(CenterMM.Z / double(CVXBlockMM)));

    const FIntVector ChunkCoord(
        GlobalBlock.X / CVX::ChunkEdgeBlocks,
        GlobalBlock.Y / CVX::ChunkEdgeBlocks,
        GlobalBlock.Z / CVX::ChunkEdgeBlocks);

    const int32 LocalX = GlobalBlock.X & (CVX::ChunkEdgeBlocks - 1);
    const int32 LocalY = GlobalBlock.Y & (CVX::ChunkEdgeBlocks - 1);
    const int32 LocalZ = GlobalBlock.Z & (CVX::ChunkEdgeBlocks - 1);
    const int32 CenterBlockIndex = CVX::LocalBlockIndex(LocalX, LocalY, LocalZ);

    bool bTouchedCoarse = false;
    bool bTouchedFrontier = false;
    bool bTouchedBlocks = false;
    bool bTouchedFine = false;

    auto RefreshOne = [this, &bTouchedCoarse, &bTouchedFrontier, &bTouchedBlocks, &bTouchedFine](
        const FIntVector& InChunkCoord,
        int32 InBlockIndex)
    {
        if (!IsChunkCoordInBounds(InChunkCoord))
        {
            return;
        }

        EnsureHotChunkSlots(InChunkCoord, bTouchedBlocks);
        UpdateHotBlockVisual(InChunkCoord, InBlockIndex, bTouchedBlocks, bTouchedFine);
        EnsureSolidFrontierAroundChunk(InChunkCoord, bTouchedFrontier);
        EnsureCoarseChunkVisual(InChunkCoord, false, bTouchedCoarse);
        EnsureFrontierChunkVisual(InChunkCoord, false, bTouchedFrontier);
    };

    if (IsChunkCoordInBounds(ChunkCoord))
    {
        RefreshOne(ChunkCoord, CenterBlockIndex);

        for (const FIntVector& Delta : CVXNeighbours)
        {
            FIntVector NeighbourChunk;
            int32 NeighbourBlock = 0;
            if (GetNeighbourBlockAddress(ChunkCoord, CenterBlockIndex, Delta, NeighbourChunk, NeighbourBlock))
            {
                RefreshOne(NeighbourChunk, NeighbourBlock);
            }
        }
    }

    if (bTouchedBlocks)
    {
        FullBlockInstances->MarkRenderInstancesDirty();
    }
    if (bTouchedFine)
    {
        FineVoxelInstances->MarkRenderInstancesDirty();
    }
    if (bTouchedFrontier)
    {
        FrontierChunkInstances->MarkRenderInstancesDirty();
    }
    FlushActiveColdShellChanges(bTouchedCoarse);

    LastImmediateSurfaceMS = float((FPlatformTime::Seconds() - StartSeconds) * 1000.0);
}

void ACicadaVoxelVolumeActor::QueueVisualBlock(const FIntVector& ChunkCoord, int32 BlockIndex)
{
    if (!IsChunkCoordInBounds(ChunkCoord) || BlockIndex < 0 || BlockIndex >= CVX::BlocksPerChunk)
    {
        return;
    }

    uint64& Mask = PendingVisualBlockMasks.FindOrAdd(ChunkCoord);
    const uint64 Bit = uint64(1) << BlockIndex;
    if ((Mask & Bit) == 0ull)
    {
        Mask |= Bit;
        ++PendingVisualBlockCount;
    }

    if (!PendingVisualChunkSet.Contains(ChunkCoord))
    {
        PendingVisualChunkSet.Add(ChunkCoord);
        PendingVisualChunkQueue.Add(ChunkCoord);
    }

    PendingVisualChunkCount = FMath::Max(0, PendingVisualChunkQueue.Num() - PendingVisualChunkReadIndex);
}

void ACicadaVoxelVolumeActor::QueueVisualBlockAndSurfaceNeighbours(
    const FIntVector& ChunkCoord,
    int32 BlockIndex)
{
    QueueVisualBlock(ChunkCoord, BlockIndex);

    static const FIntVector BlockNeighbours[6] =
    {
        FIntVector(1, 0, 0), FIntVector(-1, 0, 0),
        FIntVector(0, 1, 0), FIntVector(0, -1, 0),
        FIntVector(0, 0, 1), FIntVector(0, 0, -1)
    };

    for (const FIntVector& Delta : BlockNeighbours)
    {
        FIntVector NeighbourChunk;
        int32 NeighbourBlock = 0;
        if (GetNeighbourBlockAddress(ChunkCoord, BlockIndex, Delta, NeighbourChunk, NeighbourBlock))
        {
            QueueVisualBlock(NeighbourChunk, NeighbourBlock);
        }
    }
}

void ACicadaVoxelVolumeActor::QueueVisualBlocks(const TArray<FCVXChangedBlock>& ChangedBlocks)
{
    for (const FCVXChangedBlock& Changed : ChangedBlocks)
    {
        QueueVisualBlockAndSurfaceNeighbours(Changed.ChunkCoord, Changed.BlockIndex);
    }
}

void ACicadaVoxelVolumeActor::QueueVisualChunk(const FIntVector& ChunkCoord)
{
    for (int32 BlockIndex = 0; BlockIndex < CVX::BlocksPerChunk; ++BlockIndex)
    {
        QueueVisualBlock(ChunkCoord, BlockIndex);
    }
}

void ACicadaVoxelVolumeActor::QueueVisualChunks(const TArray<FIntVector>& ChunkCoords)
{
    for (const FIntVector& ChunkCoord : ChunkCoords)
    {
        QueueVisualChunk(ChunkCoord);
    }
}

void ACicadaVoxelVolumeActor::ProcessPendingVisualUpdates()
{
    if (PendingVisualBlockCount <= 0 || PendingVisualChunkReadIndex >= PendingVisualChunkQueue.Num())
    {
        PendingVisualChunkQueue.Reset();
        PendingVisualChunkReadIndex = 0;
        PendingVisualChunkSet.Reset();
        PendingVisualBlockMasks.Reset();
        PendingVisualChunkCount = 0;
        PendingVisualBlockCount = 0;
        LastVisualBlocksProcessed = 0;
        LastRefinedSurfaceBlocksProcessed = 0;
        return;
    }

    const double StartSeconds = FPlatformTime::Seconds();
    const double BudgetSeconds = double(FMath::Clamp(VisualUpdateBudgetMS, 0.10f, 8.0f)) * 0.001;
    const int32 MaxBlocks = FMath::Clamp(MaxVisualBlocksPerFrame, 1, 256);
    const int32 MaxChunks = FMath::Clamp(VisualChunkBudgetPerFrame, 1, 64);

    int32 ProcessedBlocks = 0;
    int32 ProcessedChunks = 0;
    LastRefinedSurfaceBlocksProcessed = 0;
    bool bTouchedCoarse = false;
    bool bTouchedFrontier = false;
    bool bTouchedBlocks = false;
    bool bTouchedFine = false;

    // FIFO rather than the old LIFO stack. Sustained automatic fire can no longer starve
    // older dirty chunks and leave stale low-resolution patches behind indefinitely.
    while (ProcessedBlocks < MaxBlocks
        && ProcessedChunks < MaxChunks
        && PendingVisualChunkReadIndex < PendingVisualChunkQueue.Num())
    {
        const FIntVector ChunkCoord = PendingVisualChunkQueue[PendingVisualChunkReadIndex++];
        PendingVisualChunkSet.Remove(ChunkCoord);

        uint64 BlockMask = 0ull;
        if (uint64* ExistingMask = PendingVisualBlockMasks.Find(ChunkCoord))
        {
            BlockMask = *ExistingMask;
            PendingVisualBlockMasks.Remove(ChunkCoord);
        }

        if (BlockMask == 0ull)
        {
            ++ProcessedChunks;
            continue;
        }

        const bool bWasHot = HotChunkVisualSlots.Contains(ChunkCoord);
        if (!bWasHot)
        {
            // If this chunk was part of the static exterior HISM, hide that one slot. Interior
            // solid support is now a plain ISM frontier and never waits for HISM tree rebuilds.
            EnsureCoarseChunkVisual(ChunkCoord, false, bTouchedCoarse);
            EnsureFrontierChunkVisual(ChunkCoord, false, bTouchedFrontier);
            EnsureSolidFrontierAroundChunk(ChunkCoord, bTouchedFrontier);
            EnsureHotChunkSlots(ChunkCoord, bTouchedBlocks);
        }
        else
        {
            EnsureSolidFrontierAroundChunk(ChunkCoord, bTouchedFrontier);
        }

        uint64 RemainingMask = BlockMask;
        for (int32 BlockIndex = 0; BlockIndex < CVX::BlocksPerChunk; ++BlockIndex)
        {
            const uint64 Bit = uint64(1) << BlockIndex;
            if ((RemainingMask & Bit) == 0ull)
            {
                continue;
            }

            UpdateHotBlockVisual(ChunkCoord, BlockIndex, bTouchedBlocks, bTouchedFine);
            RemainingMask &= ~Bit;
            PendingVisualBlockCount = FMath::Max(0, PendingVisualBlockCount - 1);
            ++ProcessedBlocks;

            const bool bTimeBudgetHit = (FPlatformTime::Seconds() - StartSeconds) >= BudgetSeconds;
            if (ProcessedBlocks >= MaxBlocks || bTimeBudgetHit)
            {
                break;
            }
        }

        if (RemainingMask != 0ull)
        {
            PendingVisualBlockMasks.Add(ChunkCoord, RemainingMask);
            PendingVisualChunkSet.Add(ChunkCoord);
            PendingVisualChunkQueue.Add(ChunkCoord);
        }

        ++ProcessedChunks;

        if ((FPlatformTime::Seconds() - StartSeconds) >= BudgetSeconds)
        {
            break;
        }
    }

    // Compact consumed queue storage occasionally without doing an O(n) remove every chunk.
    if (PendingVisualChunkReadIndex > 256
        && PendingVisualChunkReadIndex * 2 > PendingVisualChunkQueue.Num())
    {
        PendingVisualChunkQueue.RemoveAt(0, PendingVisualChunkReadIndex, EAllowShrinking::No);
        PendingVisualChunkReadIndex = 0;
    }

    PendingVisualChunkCount = FMath::Max(0, PendingVisualChunkQueue.Num() - PendingVisualChunkReadIndex);
    LastVisualBlocksProcessed = ProcessedBlocks;

    if (bTouchedBlocks)
    {
        FullBlockInstances->MarkRenderInstancesDirty();
    }
    if (bTouchedFine)
    {
        FineVoxelInstances->MarkRenderInstancesDirty();
    }
    if (bTouchedFrontier)
    {
        FrontierChunkInstances->MarkRenderInstancesDirty();
    }
    FlushActiveColdShellChanges(bTouchedCoarse);

    LastVisualUpdateMS = float((FPlatformTime::Seconds() - StartSeconds) * 1000.0);
    UpdatePerformanceString(ProcessedChunks);
    ShowRuntimeDebugMessage();
}

void ACicadaVoxelVolumeActor::KickTreeBuild(UHierarchicalInstancedStaticMeshComponent* Component, bool bForceUpdate)
{
    if (!Component)
    {
        return;
    }

    Component->BuildTreeIfOutdated(bAsyncHISMTreeBuilds, bForceUpdate);
}

void ACicadaVoxelVolumeActor::MaintainAsyncVisualTrees()
{
    if ((IsPageNativeRuntimeActive() || IsExactSurfaceRuntimeActive()) && bSingleOwnerRuntimeSurface)
    {
        bCoarseTreeBuildPending = false;
        return;
    }
    if (!bCoarseTreeBuildPending)
    {
        return;
    }

    // Only the cold 16 mm shell is hierarchical now. Hot 4 mm / 1 mm instances are plain ISMs,
    // so destructive edits no longer kick HISM cluster-tree rebuilds.
    KickTreeBuild(CoarseChunkInstances, false);
    bCoarseTreeBuildPending = false;
}

bool ACicadaVoxelVolumeActor::IsExactSurfaceRuntimeActive() const
{
    return !bUsePageNativeRuntime
        && bUseExactSurfaceMeshRuntime
        && GetWorld()
        && GetWorld()->IsGameWorld();
}


bool ACicadaVoxelVolumeActor::IsRoboticsDirectFieldRuntimeActive() const
{
    // CVX-004A R4:
    // Direct-field rendering is now mandatory in game/PIE worlds.
    //
    // Do NOT gate this on a serialized actor property. Existing level instances created
    // by older CVX builds can preserve an old/default false value and silently fall back
    // to the retired page/DynamicMesh renderer. That is exactly the outer-skin failure
    // seen in the R3 test video.
    const UWorld* World = GetWorld();
    return World && World->IsGameWorld();
}

void ACicadaVoxelVolumeActor::DisableLegacyRuntimeVisualsForRobotics()
{
    // CVX-004A R4: hard-disable every retired mesh/instance surface in game worlds.
    // There is no opt-out in this build. One authoritative field must own the pixels.
    //
    // Destroy/clear the competing render representations. This is the actual fix for
    // the persistent rectangular outer-skin bug: no handoff exists in robotics mode.
    if (CoarseChunkInstances)
    {
        CoarseChunkInstances->ClearInstances();
        CoarseChunkInstances->SetVisibility(false, true);
    }
    if (ExactColdShellInstances)
    {
        ExactColdShellInstances->ClearInstances();
        ExactColdShellInstances->SetVisibility(false, true);
    }
    if (FrontierChunkInstances)
    {
        FrontierChunkInstances->ClearInstances();
        FrontierChunkInstances->SetVisibility(false, true);
    }
    if (FullBlockInstances)
    {
        FullBlockInstances->ClearInstances();
        FullBlockInstances->SetVisibility(false, true);
    }
    if (FineVoxelInstances)
    {
        FineVoxelInstances->ClearInstances();
        FineVoxelInstances->SetVisibility(false, true);
    }

    ClearSurfaceMeshRuntime();
    ClearPageSurfaceRuntime();
    ResetVisualSlotCaches();

    PendingVisualChunkQueue.Reset();
    PendingVisualChunkSet.Reset();
    PendingVisualBlockMasks.Reset();
    PendingSurfaceMeshQueue.Reset();
    PendingSurfaceMeshSet.Reset();
    PendingSurfacePageQueue.Reset();
    PendingSurfacePageSet.Reset();
    PendingExteriorSurfacePageSet.Reset();
}

void ACicadaVoxelVolumeActor::MarkRoboticsFieldDirty()
{
    bRoboticsFieldDirty = true;
}

void ACicadaVoxelVolumeActor::FlushRoboticsFieldSnapshot(float DeltaSeconds, bool bForce)
{
    if (!IsRoboticsDirectFieldRuntimeActive())
    {
        return;
    }

    if (!bRoboticsFieldDirty && !bForce)
    {
        RoboticsFieldSnapshotAccumulator = 0.0f;
        return;
    }

    RoboticsFieldSnapshotAccumulator += FMath::Max(0.0f, DeltaSeconds);
    const float Interval = 1.0f / FMath::Clamp(RoboticsFieldSnapshotHz, 10.0f, 240.0f);

    if (!bForce && RoboticsFieldSnapshotAccumulator < Interval)
    {
        return;
    }

    RoboticsFieldSnapshotAccumulator = 0.0f;
    SubmitRoboticsFieldSnapshot();
    bRoboticsFieldDirty = false;
}

TSharedPtr<const FCVXRoboticsFieldSnapshot, ESPMode::ThreadSafe>
ACicadaVoxelVolumeActor::BuildRoboticsFieldSnapshot()
{
    const double StartSeconds = FPlatformTime::Seconds();

    TSharedPtr<FCVXRoboticsFieldSnapshot, ESPMode::ThreadSafe> Snapshot =
        MakeShared<FCVXRoboticsFieldSnapshot, ESPMode::ThreadSafe>();

    Snapshot->Generation = Volume.GetGeneration();
    Snapshot->OwnerId = uint32(GetUniqueID());
    Snapshot->bActive = true;
    Snapshot->WorldToLocal = GetActorTransform().Inverse();
    Snapshot->LogicalSizeMM = Volume.GetLogicalSizeMM();
    Snapshot->PageCounts = FIntVector(
        FMath::DivideAndRoundUp(Snapshot->LogicalSizeMM.X, FCVXRoboticsFieldSnapshot::PageEdgeMM),
        FMath::DivideAndRoundUp(Snapshot->LogicalSizeMM.Y, FCVXRoboticsFieldSnapshot::PageEdgeMM),
        FMath::DivideAndRoundUp(Snapshot->LogicalSizeMM.Z, FCVXRoboticsFieldSnapshot::PageEdgeMM));

    const FCVXGPUPersistentFieldStats GPUStats =
        FCVXGPUPageUploadBridge::GetPersistentFieldStats();

    RoboticsFieldPartialPages = int32(GPUStats.AllocatedPages);
    RoboticsFieldExplicitChunks = 0;
    RoboticsFieldMixedBlocks = 0;
    LastRoboticsFieldSnapshotBytes = 0;
    LastRoboticsFieldSnapshotMS =
        float((FPlatformTime::Seconds() - StartSeconds) * 1000.0);

    constexpr double PersistentBytes =
        double(FCVXGPUPageUploadBridge::PageHashWords
            + FCVXGPUPageUploadBridge::PageMetaWords
            + FCVXGPUPageUploadBridge::ChunkStateWords
            + FCVXGPUPageUploadBridge::BlockMaskWords)
        * double(sizeof(uint32));

    LastRoboticsFieldStats = FString::Printf(
        TEXT("CVX-004B PERSISTENT GPU FIELD | meta=%.3fms | residentPages=%u/%u | pendingPatches=%u | coalesced=%llu | cacheFull=%u | fixedVRAM=%.1fMB | legacySkin=HARD_OFF | fullWorldUploads=%lld"),
        LastRoboticsFieldSnapshotMS,
        GPUStats.AllocatedPages,
        GPUStats.PageCapacity,
        GPUStats.PendingPatches,
        GPUStats.CoalescedChunkReplacements,
        GPUStats.CacheFullEvents,
        PersistentBytes / (1024.0 * 1024.0),
        FullWorldHotUploadCount);

    return StaticCastSharedPtr<const FCVXRoboticsFieldSnapshot>(Snapshot);
}

void ACicadaVoxelVolumeActor::SubmitPersistentGPUStateBootstrap()
{
    if (!IsRoboticsDirectFieldRuntimeActive())
    {
        return;
    }

    TArray<FIntVector> EmptyPages;
    Volume.GetFullyEmptyPageCoords64MM(EmptyPages);

    TArray<FIntVector> DirtyChunks;
    Volume.GetDirtyChunkCoords(DirtyChunks);

    FCVXEditDelta Delta;
    Delta.ClearedPages64MM = EmptyPages;
    Delta.ChangedBlocks.Reserve(DirtyChunks.Num());

    for (const FIntVector& ChunkCoord : DirtyChunks)
    {
        // BuildEditDeltaWords only needs the unique chunk coordinate; block index is
        // irrelevant because the packet serializes the complete latest chunk state.
        Delta.ChangedBlocks.Emplace(ChunkCoord, 0);
    }

    if (!Delta.IsEmpty())
    {
        SubmitGPUPageDelta(Delta);
    }
}

void ACicadaVoxelVolumeActor::SubmitRoboticsFieldSnapshot()
{
    TSharedPtr<const FCVXRoboticsFieldSnapshot, ESPMode::ThreadSafe> Snapshot =
        BuildRoboticsFieldSnapshot();

    FCVXRoboticsFieldBridge::SubmitSnapshot(MoveTemp(Snapshot));
    ScheduleRuntimeStatsPublish();
}

bool ACicadaVoxelVolumeActor::IsPageNativeRuntimeActive() const
{
    return bUsePageNativeRuntime
        && GetWorld()
        && GetWorld()->IsGameWorld();
}

void ACicadaVoxelVolumeActor::ClearSurfaceMeshRuntime()
{
    for (UDynamicMeshComponent* Component : SurfaceMeshComponents)
    {
        if (Component)
        {
            Component->DestroyComponent();
        }
    }

    SurfaceMeshComponents.Reset();
    SurfaceMeshComponentByChunk.Reset();
    SurfaceMeshQuadsByChunk.Reset();
    SurfaceMeshTrianglesByChunk.Reset();
    PendingSurfaceMeshQueue.Reset();
    PendingSurfaceMeshReadIndex = 0;
    PendingSurfaceMeshSet.Reset();
    PendingSurfaceMeshChunkCount = 0;
    ActiveSurfaceMeshChunks = 0;
    LastSurfaceMeshQuads = 0;
    LastSurfaceMeshTriangles = 0;
    TotalSurfaceMeshQuads = 0;
    TotalSurfaceMeshTriangles = 0;
    SurfaceHandoffGuards = 0;
    SurfaceImmediateNeighbourRebuilds = 0;
}

void ACicadaVoxelVolumeActor::ApplyExactSurfaceComponentSettings(UDynamicMeshComponent* Component)
{
    if (!Component)
    {
        return;
    }

    // DynamicMesh supports explicit two-sided rendering in UE 5.8. Keep this enabled
    // for the temporary exact-surface bridge so a winding mistake can never turn valid
    // material into a see-through face while we continue toward the GPU renderer.
    Component->SetTwoSided(bTwoSidedExactSurface);
    Component->SetShadowsEnabled(bExactSurfaceShadows);
}

UDynamicMeshComponent* ACicadaVoxelVolumeActor::GetOrCreateSurfaceMeshComponent(const FIntVector& ChunkCoord)
{
    if (const int32* ExistingIndex = SurfaceMeshComponentByChunk.Find(ChunkCoord))
    {
        if (SurfaceMeshComponents.IsValidIndex(*ExistingIndex))
        {
            return SurfaceMeshComponents[*ExistingIndex];
        }
    }

    UDynamicMeshComponent* Component = NewObject<UDynamicMeshComponent>(this);
    if (!Component)
    {
        return nullptr;
    }

    Component->SetupAttachment(SceneRoot);
    Component->SetMobility(EComponentMobility::Movable);
    Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Component->SetCanEverAffectNavigation(false);
    Component->SetGenerateOverlapEvents(false);
    ApplyExactSurfaceComponentSettings(Component);
    if (CoarseChunkInstances && CoarseChunkInstances->GetMaterial(0))
    {
        Component->SetMaterial(0, CoarseChunkInstances->GetMaterial(0));
    }
    Component->SetRelativeLocation(FVector(
        double(ChunkCoord.X * CVXChunkMM) * 0.1,
        double(ChunkCoord.Y * CVXChunkMM) * 0.1,
        double(ChunkCoord.Z * CVXChunkMM) * 0.1));

    AddInstanceComponent(Component);
    Component->RegisterComponent();

    const int32 NewIndex = SurfaceMeshComponents.Add(Component);
    SurfaceMeshComponentByChunk.Add(ChunkCoord, NewIndex);
    return Component;
}

void ACicadaVoxelVolumeActor::QueueSurfaceMeshChunk(const FIntVector& ChunkCoord)
{
    if (!IsChunkCoordInBounds(ChunkCoord) || PendingSurfaceMeshSet.Contains(ChunkCoord))
    {
        return;
    }

    PendingSurfaceMeshSet.Add(ChunkCoord);
    PendingSurfaceMeshQueue.Add(ChunkCoord);
    PendingSurfaceMeshChunkCount = FMath::Max(0, PendingSurfaceMeshQueue.Num() - PendingSurfaceMeshReadIndex);
}

void ACicadaVoxelVolumeActor::QueueSurfaceMeshChunkAndNeighbours(const FIntVector& ChunkCoord)
{
    QueueSurfaceMeshChunk(ChunkCoord);
    for (const FIntVector& Delta : CVXNeighbours)
    {
        QueueSurfaceMeshChunk(ChunkCoord + Delta);
    }
}

void ACicadaVoxelVolumeActor::QueueSurfaceMeshFromChangedBlocks(const TArray<FCVXChangedBlock>& ChangedBlocks)
{
    TSet<FIntVector> ChangedChunks;
    ChangedChunks.Reserve(ChangedBlocks.Num());
    for (const FCVXChangedBlock& Changed : ChangedBlocks)
    {
        ChangedChunks.Add(Changed.ChunkCoord);
    }

    for (const FIntVector& ChunkCoord : ChangedChunks)
    {
        QueueSurfaceMeshChunkAndNeighbours(ChunkCoord);
    }
}

void ACicadaVoxelVolumeActor::QueueSurfaceMeshFromDirtyChunks()
{
    TArray<FIntVector> DirtyChunks;
    Volume.GetDirtyChunkCoords(DirtyChunks);
    for (const FIntVector& ChunkCoord : DirtyChunks)
    {
        QueueSurfaceMeshChunkAndNeighbours(ChunkCoord);
    }
}

void ACicadaVoxelVolumeActor::UpdateSurfaceMeshStats()
{
    int32 VisibleChunks = 0;
    int64 Quads = 0;
    int64 Triangles = 0;

    for (const TPair<FIntVector, int32>& Pair : SurfaceMeshComponentByChunk)
    {
        if (!SurfaceMeshComponents.IsValidIndex(Pair.Value))
        {
            continue;
        }

        UDynamicMeshComponent* Component = SurfaceMeshComponents[Pair.Value];
        if (Component && Component->IsVisible())
        {
            ++VisibleChunks;
            Quads += SurfaceMeshQuadsByChunk.FindRef(Pair.Key);
            Triangles += SurfaceMeshTrianglesByChunk.FindRef(Pair.Key);
        }
    }

    ActiveSurfaceMeshChunks = VisibleChunks;
    TotalSurfaceMeshQuads = Quads;
    TotalSurfaceMeshTriangles = Triangles;
}

void ACicadaVoxelVolumeActor::RebuildSurfaceMeshChunk(const FIntVector& ChunkCoord)
{
    if (!IsChunkCoordInBounds(ChunkCoord))
    {
        return;
    }

    FCVXSurfaceMeshData MeshData;
    FCVXSurfaceMesher::BuildChunkSurface(Volume, ChunkCoord, bGreedySurfaceMeshing, MeshData);

    LastSurfaceMeshQuads = MeshData.QuadCount;
    LastSurfaceMeshTriangles = MeshData.Triangles.Num() / 3;

    UDynamicMeshComponent* Component = nullptr;
    if (const int32* ExistingIndex = SurfaceMeshComponentByChunk.Find(ChunkCoord))
    {
        if (SurfaceMeshComponents.IsValidIndex(*ExistingIndex))
        {
            Component = SurfaceMeshComponents[*ExistingIndex];
        }
    }

    const bool bChunkCompletelyEmpty = Volume.IsChunkCompletelyEmpty(ChunkCoord);

    if (MeshData.Triangles.Num() == 0)
    {
        if (Component)
        {
            UE::Geometry::FDynamicMesh3 EmptyMesh(true, false, false, false);
            Component->SetMesh(MoveTemp(EmptyMesh));
            Component->SetVisibility(false, true);
        }

        SurfaceMeshQuadsByChunk.Add(ChunkCoord, 0);
        SurfaceMeshTrianglesByChunk.Add(ChunkCoord, 0);

        // Only remove the old proxy when truth proves the whole chunk is empty. A solid
        // chunk with no extracted faces is an interior chunk and does not require a new
        // surface. More importantly, we never blank an existing cold-shell chunk merely
        // because a queued rebuild produced no replacement geometry.
        if (bChunkCompletelyEmpty)
        {
            bool bTouchedCoarse = false;
            EnsureCoarseChunkVisual(ChunkCoord, false, bTouchedCoarse);
            FlushActiveColdShellChanges(bTouchedCoarse);
        }
        else if (bSafeSurfaceHandoff && CoarseInstanceByChunk.Contains(ChunkCoord))
        {
            ++SurfaceHandoffGuards;
        }
        return;
    }

    if (!Component)
    {
        Component = GetOrCreateSurfaceMeshComponent(ChunkCoord);
        if (!Component)
        {
            // Preserve any cold proxy if we failed to allocate the replacement mesh.
            if (bSafeSurfaceHandoff)
            {
                ++SurfaceHandoffGuards;
            }
            return;
        }
    }

    ApplyExactSurfaceComponentSettings(Component);

    UE::Geometry::FDynamicMesh3 DynamicMesh(true, false, false, false);
    DynamicMesh.EnableVertexNormals(FVector3f(0.0f, 0.0f, 1.0f));

    TArray<int32> VertexIDs;
    VertexIDs.Reserve(MeshData.VerticesCM.Num());

    for (int32 Index = 0; Index < MeshData.VerticesCM.Num(); ++Index)
    {
        const int32 VertexID = DynamicMesh.AppendVertex(MeshData.VerticesCM[Index]);
        DynamicMesh.SetVertexNormal(VertexID, MeshData.Normals.IsValidIndex(Index)
            ? MeshData.Normals[Index]
            : FVector3f(0.0f, 0.0f, 1.0f));
        VertexIDs.Add(VertexID);
    }

    for (int32 Index = 0; Index + 2 < MeshData.Triangles.Num(); Index += 3)
    {
        const int32 A = MeshData.Triangles[Index + 0];
        const int32 B = MeshData.Triangles[Index + 1];
        const int32 C = MeshData.Triangles[Index + 2];
        if (VertexIDs.IsValidIndex(A) && VertexIDs.IsValidIndex(B) && VertexIDs.IsValidIndex(C))
        {
            DynamicMesh.AppendTriangle(VertexIDs[A], VertexIDs[B], VertexIDs[C]);
        }
    }

    // Install and expose the exact replacement FIRST. Only after a valid renderable mesh
    // exists do we retire the cold 16 mm proxy. This makes the coarse->exact transition a
    // handoff rather than a momentary hole in the world.
    Component->SetMesh(MoveTemp(DynamicMesh));
    Component->SetVisibility(true, true);

    bool bTouchedCoarse = false;
    EnsureCoarseChunkVisual(ChunkCoord, false, bTouchedCoarse);
    if (bTouchedCoarse)
    {
        ++SurfaceProxyRetirements;
    }
    FlushActiveColdShellChanges(bTouchedCoarse);

    SurfaceMeshQuadsByChunk.Add(ChunkCoord, MeshData.QuadCount);
    SurfaceMeshTrianglesByChunk.Add(ChunkCoord, MeshData.Triangles.Num() / 3);
}

void ACicadaVoxelVolumeActor::RebuildImpactSurfaceMeshNow(const FVector& CenterMM)
{
    const FIntVector ChunkCoord(
        FMath::FloorToInt(CenterMM.X / double(CVXChunkMM)),
        FMath::FloorToInt(CenterMM.Y / double(CVXChunkMM)),
        FMath::FloorToInt(CenterMM.Z / double(CVXChunkMM)));

    const double StartSeconds = FPlatformTime::Seconds();

    TArray<FIntVector, TInlineAllocator<7>> ImmediateChunks;
    ImmediateChunks.Add(ChunkCoord);
    if (bImmediateImpactNeighbourMeshes)
    {
        for (const FIntVector& Delta : CVXNeighbours)
        {
            const FIntVector Candidate = ChunkCoord + Delta;
            if (IsChunkCoordInBounds(Candidate))
            {
                ImmediateChunks.Add(Candidate);
            }
        }
    }

    int32 Rebuilt = 0;
    for (const FIntVector& Candidate : ImmediateChunks)
    {
        if (!IsChunkCoordInBounds(Candidate))
        {
            continue;
        }

        RebuildSurfaceMeshChunk(Candidate);
        PendingSurfaceMeshSet.Remove(Candidate);
        ++Rebuilt;
    }

    SurfaceImmediateNeighbourRebuilds += FMath::Max(0, Rebuilt - 1);
    PendingSurfaceMeshChunkCount = PendingSurfaceMeshSet.Num();
    UpdateSurfaceMeshStats();
    LastImmediateSurfaceMS = float((FPlatformTime::Seconds() - StartSeconds) * 1000.0);
}

void ACicadaVoxelVolumeActor::ProcessPendingSurfaceMeshUpdates()
{
    if (PendingSurfaceMeshReadIndex >= PendingSurfaceMeshQueue.Num())
    {
        PendingSurfaceMeshQueue.Reset();
        PendingSurfaceMeshReadIndex = 0;
        PendingSurfaceMeshSet.Reset();
        PendingSurfaceMeshChunkCount = 0;
        LastSurfaceMeshChunksProcessed = 0;
        return;
    }

    const double StartSeconds = FPlatformTime::Seconds();
    const double BudgetSeconds = double(FMath::Clamp(SurfaceMeshBudgetMS, 0.10f, 8.0f)) * 0.001;
    const int32 MaxChunks = FMath::Clamp(MaxSurfaceMeshChunksPerFrame, 1, 32);
    int32 Processed = 0;

    while (Processed < MaxChunks && PendingSurfaceMeshReadIndex < PendingSurfaceMeshQueue.Num())
    {
        const FIntVector ChunkCoord = PendingSurfaceMeshQueue[PendingSurfaceMeshReadIndex++];

        // Immediate impact refresh may already have consumed this request.
        if (!PendingSurfaceMeshSet.Remove(ChunkCoord))
        {
            continue;
        }

        RebuildSurfaceMeshChunk(ChunkCoord);
        ++Processed;

        if ((FPlatformTime::Seconds() - StartSeconds) >= BudgetSeconds)
        {
            break;
        }
    }

    if (PendingSurfaceMeshReadIndex > 256
        && PendingSurfaceMeshReadIndex * 2 > PendingSurfaceMeshQueue.Num())
    {
        PendingSurfaceMeshQueue.RemoveAt(0, PendingSurfaceMeshReadIndex, EAllowShrinking::No);
        PendingSurfaceMeshReadIndex = 0;
    }

    PendingSurfaceMeshChunkCount = PendingSurfaceMeshSet.Num();
    LastSurfaceMeshChunksProcessed = Processed;
    LastSurfaceMeshUpdateMS = float((FPlatformTime::Seconds() - StartSeconds) * 1000.0);
    UpdateSurfaceMeshStats();
    UpdatePerformanceString(Processed);
    ShowRuntimeDebugMessage();
}


void ACicadaVoxelVolumeActor::ClearPageSurfaceRuntime()
{
    for (UDynamicMeshComponent* Component : SurfacePageComponents)
    {
        if (Component)
        {
            Component->DestroyComponent();
        }
    }

    SurfacePageComponents.Reset();
    SurfacePageComponentByCoord.Reset();
    SurfacePageQuadsByCoord.Reset();
    PendingSurfacePageQueue.Reset();
    PendingSurfacePageReadIndex = 0;
    PendingSurfacePageSet.Reset();
    PendingSurfacePageCount = 0;
    LastSurfacePagesProcessed = 0;
    ActiveSurfacePageCount = 0;
    LastSurfacePageMS = 0.0f;
    LastSurfacePageQuads = 0;
    SurfacePageJobsInFlightSet.Reset();
    SurfacePageRerunSet.Reset();
    SurfacePageGeneration.Reset();
    PendingExteriorSurfacePageSet.Reset();
    AsyncSurfacePageJobsInFlight = 0;
    AsyncSurfacePagesCompleted = 0;
    AsyncSurfaceStaleResults = 0;
    LastAsyncSurfaceWorkerMS = 0.0f;
    LastExteriorSurfaceMS = 0.0f;
    LastExteriorPagesUpdated = 0;
    SurfacePageLastTouchSerial.Reset();
    SurfacePageEstimatedBytesByCoord.Reset();
    SurfacePageLastExteriorBuildSeconds.Reset();
    SurfacePageTouchSerial = 0;
    EstimatedReferenceSurfaceBytes = 0;
    EstimatedReferenceSurfaceMB = 0.0f;
    ResidentReferenceSurfacePages = 0;
    ReferenceSurfaceEvictions = 0;
    ReferenceSurfaceComplexityRejects = 0;
}

FIntVector ACicadaVoxelVolumeActor::ChunkToSurfacePage(const FIntVector& ChunkCoord) const
{
    return FCVXSurfaceMesher::ChunkToPageCoord(ChunkCoord);
}

bool ACicadaVoxelVolumeActor::IsSurfacePageInBounds(const FIntVector& PageCoord) const
{
    const FIntVector Size = Volume.GetLogicalSizeMM();
    const FIntVector PageCounts(
        FMath::DivideAndRoundUp(Size.X, FCVXSurfaceMesher::PageEdgeMM),
        FMath::DivideAndRoundUp(Size.Y, FCVXSurfaceMesher::PageEdgeMM),
        FMath::DivideAndRoundUp(Size.Z, FCVXSurfaceMesher::PageEdgeMM));

    return PageCoord.X >= 0 && PageCoord.Y >= 0 && PageCoord.Z >= 0
        && PageCoord.X < PageCounts.X
        && PageCoord.Y < PageCounts.Y
        && PageCoord.Z < PageCounts.Z;
}

void ACicadaVoxelVolumeActor::QueueSurfacePage(const FIntVector& PageCoord)
{
    if (!IsSurfacePageInBounds(PageCoord))
    {
        return;
    }

    uint32& Generation = SurfacePageGeneration.FindOrAdd(PageCoord);
    ++Generation;

    if (SurfacePageJobsInFlightSet.Contains(PageCoord))
    {
        SurfacePageRerunSet.Add(PageCoord);
        PendingSurfacePageCount = PendingSurfacePageSet.Num()
            + SurfacePageJobsInFlightSet.Num()
            + SurfacePageRerunSet.Num();
        return;
    }

    if (!PendingSurfacePageSet.Contains(PageCoord))
    {
        PendingSurfacePageSet.Add(PageCoord);
        PendingSurfacePageQueue.Add(PageCoord);
    }

    PendingSurfacePageCount = PendingSurfacePageSet.Num()
        + SurfacePageJobsInFlightSet.Num()
        + SurfacePageRerunSet.Num();
}

void ACicadaVoxelVolumeActor::QueueSurfacePagesFromChangedBlocks(const TArray<FCVXChangedBlock>& ChangedBlocks)
{
    TSet<FIntVector> Pages;
    Pages.Reserve(ChangedBlocks.Num() * 2);

    for (const FCVXChangedBlock& Changed : ChangedBlocks)
    {
        const FIntVector PageCoord = ChunkToSurfacePage(Changed.ChunkCoord);
        Pages.Add(PageCoord);

        const int32 LBX = Changed.BlockIndex % CVX::ChunkEdgeBlocks;
        const int32 LBY = (Changed.BlockIndex / CVX::ChunkEdgeBlocks) % CVX::ChunkEdgeBlocks;
        const int32 LBZ = Changed.BlockIndex / (CVX::ChunkEdgeBlocks * CVX::ChunkEdgeBlocks);
        const FIntVector GlobalBlock = Changed.ChunkCoord * CVX::ChunkEdgeBlocks + FIntVector(LBX, LBY, LBZ);
        const FIntVector LocalInPage(
            GlobalBlock.X & 15,
            GlobalBlock.Y & 15,
            GlobalBlock.Z & 15);

        if (LocalInPage.X == 0) Pages.Add(PageCoord + FIntVector(-1,0,0));
        if (LocalInPage.X == 15) Pages.Add(PageCoord + FIntVector(1,0,0));
        if (LocalInPage.Y == 0) Pages.Add(PageCoord + FIntVector(0,-1,0));
        if (LocalInPage.Y == 15) Pages.Add(PageCoord + FIntVector(0,1,0));
        if (LocalInPage.Z == 0) Pages.Add(PageCoord + FIntVector(0,0,-1));
        if (LocalInPage.Z == 15) Pages.Add(PageCoord + FIntVector(0,0,1));
    }

    for (const FIntVector& PageCoord : Pages)
    {
        QueueSurfacePage(PageCoord);
    }
}


void ACicadaVoxelVolumeActor::QueueSurfacePagesFromEditDelta(const FCVXEditDelta& Delta)
{
    TSet<FIntVector> Pages;
    Pages.Reserve(Delta.ClearedPages64MM.Num() * 7 + Delta.ClearedChunks16MM.Num() * 2 + Delta.ChangedBlocks.Num() * 2);

    for (const FIntVector& PageCoord : Delta.ClearedPages64MM)
    {
        Pages.Add(PageCoord);
        for (const FIntVector& N : CVXNeighbours) Pages.Add(PageCoord + N);
    }

    for (const FIntVector& ChunkCoord : Delta.ClearedChunks16MM)
    {
        const FIntVector PageCoord = ChunkToSurfacePage(ChunkCoord);
        Pages.Add(PageCoord);
        const FIntVector Local(ChunkCoord.X & 3, ChunkCoord.Y & 3, ChunkCoord.Z & 3);
        if (Local.X == 0) Pages.Add(PageCoord + FIntVector(-1,0,0));
        if (Local.X == 3) Pages.Add(PageCoord + FIntVector(1,0,0));
        if (Local.Y == 0) Pages.Add(PageCoord + FIntVector(0,-1,0));
        if (Local.Y == 3) Pages.Add(PageCoord + FIntVector(0,1,0));
        if (Local.Z == 0) Pages.Add(PageCoord + FIntVector(0,0,-1));
        if (Local.Z == 3) Pages.Add(PageCoord + FIntVector(0,0,1));
    }

    for (const FCVXChangedBlock& Changed : Delta.ChangedBlocks)
    {
        const FIntVector PageCoord = ChunkToSurfacePage(Changed.ChunkCoord);
        Pages.Add(PageCoord);
        const int32 LBX = Changed.BlockIndex % CVX::ChunkEdgeBlocks;
        const int32 LBY = (Changed.BlockIndex / CVX::ChunkEdgeBlocks) % CVX::ChunkEdgeBlocks;
        const int32 LBZ = Changed.BlockIndex / (CVX::ChunkEdgeBlocks * CVX::ChunkEdgeBlocks);
        const FIntVector GlobalBlock = Changed.ChunkCoord * CVX::ChunkEdgeBlocks + FIntVector(LBX, LBY, LBZ);
        const FIntVector Local(GlobalBlock.X & 15, GlobalBlock.Y & 15, GlobalBlock.Z & 15);
        if (Local.X == 0) Pages.Add(PageCoord + FIntVector(-1,0,0));
        if (Local.X == 15) Pages.Add(PageCoord + FIntVector(1,0,0));
        if (Local.Y == 0) Pages.Add(PageCoord + FIntVector(0,-1,0));
        if (Local.Y == 15) Pages.Add(PageCoord + FIntVector(0,1,0));
        if (Local.Z == 0) Pages.Add(PageCoord + FIntVector(0,0,-1));
        if (Local.Z == 15) Pages.Add(PageCoord + FIntVector(0,0,1));
    }

    for (const FIntVector& PageCoord : Pages)
    {
        QueueSurfacePage(PageCoord);
    }
}

void ACicadaVoxelVolumeActor::QueueSurfacePagesFromDirtyChunks()
{
    TArray<FIntVector> DirtyChunks;
    Volume.GetDirtyChunkCoords(DirtyChunks);

    TSet<FIntVector> Pages;
    Pages.Reserve(DirtyChunks.Num());
    for (const FIntVector& ChunkCoord : DirtyChunks)
    {
        Pages.Add(ChunkToSurfacePage(ChunkCoord));
        for (const FIntVector& Delta : CVXNeighbours)
        {
            Pages.Add(ChunkToSurfacePage(ChunkCoord + Delta));
        }
    }

    for (const FIntVector& PageCoord : Pages)
    {
        QueueSurfacePage(PageCoord);
    }
}


int64 ACicadaVoxelVolumeActor::EstimateReferenceSurfaceBytes(const FCVXSurfaceMeshData& MeshData) const
{
    // Conservative approximation, deliberately larger than the raw arrays. DynamicMesh keeps
    // CPU topology plus the render proxy owns position/tangent/index buffers and allocator slack.
    // The governor exists to stay comfortably below the hardware limit, not to win a byte-counting contest.
    constexpr int64 EstimatedBytesPerVertex = 72;
    constexpr int64 EstimatedBytesPerIndex = 4;
    constexpr int64 PerPageOverhead = 64 * 1024;
    return int64(MeshData.VerticesCM.Num()) * EstimatedBytesPerVertex
        + int64(MeshData.Triangles.Num()) * EstimatedBytesPerIndex
        + PerPageOverhead;
}

void ACicadaVoxelVolumeActor::TouchReferenceSurfacePage(const FIntVector& PageCoord)
{
    SurfacePageLastTouchSerial.Add(PageCoord, ++SurfacePageTouchSerial);
}

void ACicadaVoxelVolumeActor::RestoreColdChunksForPage(const FIntVector& PageCoord)
{
    const FIntVector ChunkOrigin = FCVXSurfaceMesher::PageToChunkOrigin(PageCoord);
    bool bTouchedCold = false;
    for (int32 Z = 0; Z < FCVXSurfaceMesher::PageEdgeChunks; ++Z)
    {
        for (int32 Y = 0; Y < FCVXSurfaceMesher::PageEdgeChunks; ++Y)
        {
            for (int32 X = 0; X < FCVXSurfaceMesher::PageEdgeChunks; ++X)
            {
                const FIntVector ChunkCoord = ChunkOrigin + FIntVector(X, Y, Z);
                if (IsChunkCoordInBounds(ChunkCoord))
                {
                    EnsureCoarseChunkVisual(ChunkCoord, true, bTouchedCold);
                }
            }
        }
    }
    FlushActiveColdShellChanges(bTouchedCold);
}

bool ACicadaVoxelVolumeActor::EvictReferenceSurfacePage(const FIntVector& PageCoord)
{
    const int32* ExistingIndex = SurfacePageComponentByCoord.Find(PageCoord);
    if (!ExistingIndex || !SurfacePageComponents.IsValidIndex(*ExistingIndex))
    {
        SurfacePageLastTouchSerial.Remove(PageCoord);
        SurfacePageEstimatedBytesByCoord.Remove(PageCoord);
        SurfacePageQuadsByCoord.Remove(PageCoord);
        return false;
    }

    UDynamicMeshComponent* Component = SurfacePageComponents[*ExistingIndex];
    if (Component)
    {
        UE::Geometry::FDynamicMesh3 EmptyMesh(true, false, false, false);
        Component->SetMesh(MoveTemp(EmptyMesh));
        Component->SetVisibility(false, true);
    }

    if (const int64* Bytes = SurfacePageEstimatedBytesByCoord.Find(PageCoord))
    {
        EstimatedReferenceSurfaceBytes = FMath::Max<int64>(0, EstimatedReferenceSurfaceBytes - *Bytes);
    }

    SurfacePageEstimatedBytesByCoord.Remove(PageCoord);
    SurfacePageQuadsByCoord.Remove(PageCoord);
    SurfacePageLastTouchSerial.Remove(PageCoord);

    // This is only a temporary reference renderer. If a page must leave the cache, restore the
    // cheap coarse shell so the world never becomes visually absent. Exact truth remains in Volume
    // and will be remeshed when the page becomes resident again.
    RestoreColdChunksForPage(PageCoord);
    ++ReferenceSurfaceEvictions;
    return true;
}

void ACicadaVoxelVolumeActor::EnforceReferenceSurfaceBudget()
{
    if (!bEnableReferenceSurfaceMemoryGovernor)
    {
        return;
    }

    const int32 MaxPages = FMath::Clamp(MaxResidentReferenceSurfacePages, 4, 256);
    const int64 MaxBytes = int64(FMath::Clamp(ReferenceSurfaceBudgetMB, 16.0f, 1024.0f) * 1024.0f * 1024.0f);

    auto CountResident = [this]() -> int32
    {
        int32 Count = 0;
        for (const TPair<FIntVector, int64>& Pair : SurfacePageEstimatedBytesByCoord)
        {
            if (Pair.Value > 0) ++Count;
        }
        return Count;
    };

    int32 Resident = CountResident();
    while ((Resident > MaxPages || EstimatedReferenceSurfaceBytes > MaxBytes)
        && SurfacePageLastTouchSerial.Num() > 0)
    {
        FIntVector Victim = FIntVector::ZeroValue;
        uint64 OldestSerial = MAX_uint64;
        bool bFound = false;

        // LRU is intentional: actively edited pages continuously touch themselves, while old
        // destruction is the first thing demoted back to the cheap coarse reference shell.
        for (const TPair<FIntVector, uint64>& Pair : SurfacePageLastTouchSerial)
        {
            if (SurfacePageJobsInFlightSet.Contains(Pair.Key)) continue;
            if (Pair.Value < OldestSerial)
            {
                OldestSerial = Pair.Value;
                Victim = Pair.Key;
                bFound = true;
            }
        }

        if (!bFound || !EvictReferenceSurfacePage(Victim))
        {
            break;
        }

        Resident = CountResident();
    }

    ResidentReferenceSurfacePages = Resident;
    EstimatedReferenceSurfaceMB = float(double(EstimatedReferenceSurfaceBytes) / (1024.0 * 1024.0));
}

UDynamicMeshComponent* ACicadaVoxelVolumeActor::GetOrCreateSurfacePageComponent(const FIntVector& PageCoord)
{
    if (const int32* ExistingIndex = SurfacePageComponentByCoord.Find(PageCoord))
    {
        if (SurfacePageComponents.IsValidIndex(*ExistingIndex))
        {
            TouchReferenceSurfacePage(PageCoord);
            return SurfacePageComponents[*ExistingIndex];
        }
    }

    const FName ComponentName(*FString::Printf(
        TEXT("CVX_Page64_%d_%d_%d"),
        PageCoord.X,
        PageCoord.Y,
        PageCoord.Z));

    UDynamicMeshComponent* Component = NewObject<UDynamicMeshComponent>(this, ComponentName);
    if (!Component)
    {
        return nullptr;
    }

    Component->SetupAttachment(SceneRoot);
    Component->SetMobility(EComponentMobility::Movable);
    Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Component->SetCanEverAffectNavigation(false);
    Component->SetGenerateOverlapEvents(false);
    ApplyExactSurfaceComponentSettings(Component);

    if (CoarseChunkInstances && CoarseChunkInstances->GetMaterial(0))
    {
        Component->SetMaterial(0, CoarseChunkInstances->GetMaterial(0));
    }

    Component->SetRelativeLocation(FVector(
        double(PageCoord.X * FCVXSurfaceMesher::PageEdgeMM) * 0.1,
        double(PageCoord.Y * FCVXSurfaceMesher::PageEdgeMM) * 0.1,
        double(PageCoord.Z * FCVXSurfaceMesher::PageEdgeMM) * 0.1));

    AddInstanceComponent(Component);
    Component->RegisterComponent();

    const int32 NewIndex = SurfacePageComponents.Add(Component);
    SurfacePageComponentByCoord.Add(PageCoord, NewIndex);
    TouchReferenceSurfacePage(PageCoord);
    return Component;
}

void ACicadaVoxelVolumeActor::RetireColdChunksForPage(const FIntVector& PageCoord)
{
    const FIntVector ChunkOrigin = FCVXSurfaceMesher::PageToChunkOrigin(PageCoord);
    bool bTouchedCold = false;

    for (int32 Z = 0; Z < FCVXSurfaceMesher::PageEdgeChunks; ++Z)
    {
        for (int32 Y = 0; Y < FCVXSurfaceMesher::PageEdgeChunks; ++Y)
        {
            for (int32 X = 0; X < FCVXSurfaceMesher::PageEdgeChunks; ++X)
            {
                const FIntVector ChunkCoord = ChunkOrigin + FIntVector(X, Y, Z);
                if (IsChunkCoordInBounds(ChunkCoord))
                {
                    EnsureCoarseChunkVisual(ChunkCoord, false, bTouchedCold);
                }
            }
        }
    }

    FlushActiveColdShellChanges(bTouchedCold);
}

void ACicadaVoxelVolumeActor::ApplySurfacePageMesh(const FIntVector& PageCoord, FCVXSurfaceMeshData&& MeshData)
{
    LastSurfacePageQuads = MeshData.QuadCount;
    TouchReferenceSurfacePage(PageCoord);

    if (bEnableReferenceSurfaceMemoryGovernor
        && MeshData.QuadCount > FMath::Clamp(MaxReferencePageQuads, 1000, 250000))
    {
        // A pathological fragmented page is precisely where DynamicMesh stops being a sensible
        // renderer. Keep cheap coarse coverage and let the authoritative sparse/GPU page data live on.
        ++ReferenceSurfaceComplexityRejects;
        RestoreColdChunksForPage(PageCoord);
        EnforceReferenceSurfaceBudget();
        return;
    }

    UDynamicMeshComponent* Component = nullptr;
    if (const int32* ExistingIndex = SurfacePageComponentByCoord.Find(PageCoord))
    {
        if (SurfacePageComponents.IsValidIndex(*ExistingIndex)) Component = SurfacePageComponents[*ExistingIndex];
    }

    if (MeshData.Triangles.Num() == 0)
    {
        if (Component)
        {
            UE::Geometry::FDynamicMesh3 EmptyMesh(true, false, false, false);
            Component->SetMesh(MoveTemp(EmptyMesh));
            Component->SetVisibility(false, true);
        }
        if (const int64* OldBytes = SurfacePageEstimatedBytesByCoord.Find(PageCoord))
        {
            EstimatedReferenceSurfaceBytes = FMath::Max<int64>(0, EstimatedReferenceSurfaceBytes - *OldBytes);
        }
        SurfacePageEstimatedBytesByCoord.Remove(PageCoord);
        SurfacePageQuadsByCoord.Add(PageCoord, 0);
        RetireColdChunksForPage(PageCoord);
        EnforceReferenceSurfaceBudget();
        return;
    }

    if (!Component)
    {
        Component = GetOrCreateSurfacePageComponent(PageCoord);
        if (!Component) return;
    }

    UE::Geometry::FDynamicMesh3 DynamicMesh(true, false, false, false);
    DynamicMesh.EnableVertexNormals(FVector3f(0.0f, 0.0f, 1.0f));
    TArray<int32> VertexIDs;
    VertexIDs.Reserve(MeshData.VerticesCM.Num());
    for (int32 Index = 0; Index < MeshData.VerticesCM.Num(); ++Index)
    {
        const int32 VertexID = DynamicMesh.AppendVertex(MeshData.VerticesCM[Index]);
        DynamicMesh.SetVertexNormal(VertexID, MeshData.Normals.IsValidIndex(Index)
            ? MeshData.Normals[Index] : FVector3f(0.0f, 0.0f, 1.0f));
        VertexIDs.Add(VertexID);
    }
    for (int32 Index = 0; Index + 2 < MeshData.Triangles.Num(); Index += 3)
    {
        const int32 A = MeshData.Triangles[Index];
        const int32 B = MeshData.Triangles[Index + 1];
        const int32 C = MeshData.Triangles[Index + 2];
        if (VertexIDs.IsValidIndex(A) && VertexIDs.IsValidIndex(B) && VertexIDs.IsValidIndex(C))
            DynamicMesh.AppendTriangle(VertexIDs[A], VertexIDs[B], VertexIDs[C]);
    }

    const int64 NewEstimatedBytes = EstimateReferenceSurfaceBytes(MeshData);
    if (const int64* OldBytes = SurfacePageEstimatedBytesByCoord.Find(PageCoord))
    {
        EstimatedReferenceSurfaceBytes = FMath::Max<int64>(0, EstimatedReferenceSurfaceBytes - *OldBytes);
    }

    Component->SetMesh(MoveTemp(DynamicMesh));
    Component->SetVisibility(true, true);
    RetireColdChunksForPage(PageCoord);
    SurfacePageQuadsByCoord.Add(PageCoord, MeshData.QuadCount);
    SurfacePageEstimatedBytesByCoord.Add(PageCoord, NewEstimatedBytes);
    EstimatedReferenceSurfaceBytes += NewEstimatedBytes;
    TouchReferenceSurfacePage(PageCoord);
    EnforceReferenceSurfaceBudget();
}

void ACicadaVoxelVolumeActor::RebuildSurfacePage(const FIntVector& PageCoord)
{
    if (!IsSurfacePageInBounds(PageCoord)) return;
    FCVXSurfaceMeshData MeshData;
    FCVXSurfaceMesher::BuildPageSurface(Volume, PageCoord, bGreedySurfaceMeshing, MeshData);
    ApplySurfacePageMesh(PageCoord, MoveTemp(MeshData));
}

bool ACicadaVoxelVolumeActor::IsSurfacePageOnWorldBoundary(const FIntVector& PageCoord) const
{
    if (!IsSurfacePageInBounds(PageCoord)) return false;
    const FIntVector Size = Volume.GetLogicalSizeMM();
    const FIntVector Origin = PageCoord * FCVXSurfaceMesher::PageEdgeMM;
    const FIntVector End = Origin + FIntVector(FCVXSurfaceMesher::PageEdgeMM);
    return Origin.X == 0 || Origin.Y == 0 || Origin.Z == 0
        || End.X >= Size.X || End.Y >= Size.Y || End.Z >= Size.Z;
}

void ACicadaVoxelVolumeActor::QueueExteriorSurfacePage(const FIntVector& PageCoord)
{
    if (bRealtimeExteriorSurface && IsSurfacePageOnWorldBoundary(PageCoord))
        PendingExteriorSurfacePageSet.Add(PageCoord);
}

void ACicadaVoxelVolumeActor::QueueExteriorSurfacePagesFromChangedBlocks(const TArray<FCVXChangedBlock>& ChangedBlocks)
{
    for (const FCVXChangedBlock& Changed : ChangedBlocks)
        QueueExteriorSurfacePage(ChunkToSurfacePage(Changed.ChunkCoord));
}

void ACicadaVoxelVolumeActor::QueueExteriorSurfacePagesFromEditDelta(const FCVXEditDelta& Delta)
{
    for (const FIntVector& PageCoord : Delta.ClearedPages64MM) QueueExteriorSurfacePage(PageCoord);
    for (const FIntVector& ChunkCoord : Delta.ClearedChunks16MM) QueueExteriorSurfacePage(ChunkToSurfacePage(ChunkCoord));
    for (const FCVXChangedBlock& Changed : Delta.ChangedBlocks) QueueExteriorSurfacePage(ChunkToSurfacePage(Changed.ChunkCoord));
}

void ACicadaVoxelVolumeActor::RebuildExteriorSurfacePageNow(const FIntVector& PageCoord)
{
    if (!IsSurfacePageOnWorldBoundary(PageCoord)) return;
    FCVXSurfacePageSnapshot Snapshot;
    FCVXSurfaceMesher::BuildPageSnapshot(Volume, PageCoord, Snapshot);
    FCVXSurfaceMeshData MeshData;
    FCVXSurfaceMesher::BuildPageExteriorSurfaceFromSnapshot(Snapshot, MeshData);
    ApplySurfacePageMesh(PageCoord, MoveTemp(MeshData));
}

void ACicadaVoxelVolumeActor::ProcessPendingExteriorSurfaceUpdates()
{
    LastExteriorPagesUpdated = 0;
    LastExteriorSurfaceMS = 0.0f;
    if (!bRealtimeExteriorSurface || PendingExteriorSurfacePageSet.Num() == 0) return;
    const double Start = FPlatformTime::Seconds();
    const double Budget = double(FMath::Clamp(RealtimeExteriorBudgetMS, 0.1f, 8.0f)) * 0.001;
    TArray<FIntVector> Pages = PendingExteriorSurfacePageSet.Array();
    const double NowSeconds = FPlatformTime::Seconds();
    const double MinExteriorInterval = 1.0 / double(FMath::Clamp(RealtimeExteriorMaxHzPerPage, 2.0f, 60.0f));
    for (const FIntVector& PageCoord : Pages)
    {
        const double LastBuild = SurfacePageLastExteriorBuildSeconds.FindRef(PageCoord);
        if (LastBuild > 0.0 && (NowSeconds - LastBuild) < MinExteriorInterval)
        {
            continue;
        }

        PendingExteriorSurfacePageSet.Remove(PageCoord);
        SurfacePageLastExteriorBuildSeconds.Add(PageCoord, NowSeconds);
        RebuildExteriorSurfacePageNow(PageCoord);
        ++LastExteriorPagesUpdated;
        if (LastExteriorPagesUpdated >= FMath::Clamp(MaxRealtimeExteriorPagesPerFrame, 1, 16)) break;
        if ((FPlatformTime::Seconds() - Start) >= Budget) break;
    }
    LastExteriorSurfaceMS = float((FPlatformTime::Seconds() - Start) * 1000.0);
}

void ACicadaVoxelVolumeActor::LaunchAsyncSurfacePageJob(const FIntVector& PageCoord, uint32 Generation)
{
    FCVXSurfacePageSnapshot Snapshot;
    FCVXSurfaceMesher::BuildPageSnapshot(Volume, PageCoord, Snapshot);
    SurfacePageJobsInFlightSet.Add(PageCoord);
    AsyncSurfacePageJobsInFlight = SurfacePageJobsInFlightSet.Num();
    const bool bGreedy = bGreedySurfaceMeshing;
    TWeakObjectPtr<ACicadaVoxelVolumeActor> WeakThis(this);

    Async(EAsyncExecution::ThreadPool, [WeakThis, PageCoord, Generation, Snapshot = MoveTemp(Snapshot), bGreedy]() mutable
    {
        const double WorkerStart = FPlatformTime::Seconds();
        TSharedPtr<FCVXSurfaceMeshData, ESPMode::ThreadSafe> MeshData = MakeShared<FCVXSurfaceMeshData, ESPMode::ThreadSafe>();
        FCVXSurfaceMesher::BuildPageSurfaceFromSnapshot(Snapshot, bGreedy, *MeshData);
        const float WorkerMS = float((FPlatformTime::Seconds() - WorkerStart) * 1000.0);

        AsyncTask(ENamedThreads::GameThread, [WeakThis, PageCoord, Generation, MeshData, WorkerMS]() mutable
        {
            if (!WeakThis.IsValid()) return;
            ACicadaVoxelVolumeActor* Self = WeakThis.Get();
            Self->SurfacePageJobsInFlightSet.Remove(PageCoord);
            Self->AsyncSurfacePageJobsInFlight = Self->SurfacePageJobsInFlightSet.Num();
            Self->LastAsyncSurfaceWorkerMS = WorkerMS;

            const uint32 CurrentGeneration = Self->SurfacePageGeneration.FindRef(PageCoord);
            if (CurrentGeneration == Generation)
            {
                Self->ApplySurfacePageMesh(PageCoord, MoveTemp(*MeshData));
                ++Self->AsyncSurfacePagesCompleted;
            }
            else
            {
                ++Self->AsyncSurfaceStaleResults;
            }

            if (Self->SurfacePageRerunSet.Remove(PageCoord) > 0 || CurrentGeneration != Generation)
            {
                Self->QueueSurfacePage(PageCoord);
            }
            Self->UpdateSurfacePageStats();
            Self->ScheduleRuntimeStatsPublish();
        });
    });
}


void ACicadaVoxelVolumeActor::UpdateSurfacePageStats()
{
    int32 Visible = 0;
    for (const TPair<FIntVector, int32>& Pair : SurfacePageComponentByCoord)
    {
        if (!SurfacePageComponents.IsValidIndex(Pair.Value))
        {
            continue;
        }
        UDynamicMeshComponent* Component = SurfacePageComponents[Pair.Value];
        if (Component && Component->IsVisible())
        {
            ++Visible;
        }
    }
    ActiveSurfacePageCount = Visible;
    ResidentReferenceSurfacePages = SurfacePageEstimatedBytesByCoord.Num();
    EstimatedReferenceSurfaceMB = float(double(EstimatedReferenceSurfaceBytes) / (1024.0 * 1024.0));
    PendingSurfacePageCount = PendingSurfacePageSet.Num() + SurfacePageJobsInFlightSet.Num() + SurfacePageRerunSet.Num();
}

void ACicadaVoxelVolumeActor::ProcessPendingSurfacePageUpdates()
{
    LastSurfacePagesProcessed = 0;
    const double StartSeconds = FPlatformTime::Seconds();

    if (bAsyncSurfacePageMeshing)
    {
        const int32 MaxJobs = FMath::Clamp(MaxAsyncSurfacePageJobs, 1, 8);
        while (SurfacePageJobsInFlightSet.Num() < MaxJobs
            && PendingSurfacePageReadIndex < PendingSurfacePageQueue.Num())
        {
            const FIntVector PageCoord = PendingSurfacePageQueue[PendingSurfacePageReadIndex++];
            if (!PendingSurfacePageSet.Remove(PageCoord)) continue;
            if (SurfacePageJobsInFlightSet.Contains(PageCoord))
            {
                SurfacePageRerunSet.Add(PageCoord);
                continue;
            }
            const uint32 Generation = SurfacePageGeneration.FindRef(PageCoord);
            LaunchAsyncSurfacePageJob(PageCoord, Generation);
            ++LastSurfacePagesProcessed;
        }
    }
    else
    {
        const double BudgetSeconds = double(FMath::Clamp(SurfacePageBudgetMS, 0.25f, 16.0f)) * 0.001;
        const int32 MaxPages = FMath::Clamp(MaxSurfacePagesPerFrame, 1, 16);
        while (LastSurfacePagesProcessed < MaxPages && PendingSurfacePageReadIndex < PendingSurfacePageQueue.Num())
        {
            const FIntVector PageCoord = PendingSurfacePageQueue[PendingSurfacePageReadIndex++];
            if (!PendingSurfacePageSet.Remove(PageCoord)) continue;
            RebuildSurfacePage(PageCoord);
            ++LastSurfacePagesProcessed;
            if ((FPlatformTime::Seconds() - StartSeconds) >= BudgetSeconds) break;
        }
    }

    if (PendingSurfacePageReadIndex > 64 && PendingSurfacePageReadIndex * 2 > PendingSurfacePageQueue.Num())
    {
        PendingSurfacePageQueue.RemoveAt(0, PendingSurfacePageReadIndex, EAllowShrinking::No);
        PendingSurfacePageReadIndex = 0;
    }

    LastSurfacePageMS = float((FPlatformTime::Seconds() - StartSeconds) * 1000.0);
    PendingSurfacePageCount = PendingSurfacePageSet.Num()
        + SurfacePageJobsInFlightSet.Num()
        + SurfacePageRerunSet.Num();
    UpdateSurfacePageStats();
}

void ACicadaVoxelVolumeActor::SubmitGPUPageDelta(const TArray<FCVXChangedBlock>& ChangedBlocks)
{
    if (!bUploadChangedPagesToGPU || !bEnableVirtualPageSpine || ChangedBlocks.Num() == 0)
    {
        return;
    }

    FCVXEditDelta Delta;
    Delta.ChangedBlocks = ChangedBlocks;

    TArray<uint32> DeltaWords;
    VirtualPageSpine.BuildEditDeltaWords(Volume, Delta, DeltaWords);
    if (DeltaWords.Num() == 0)
    {
        return;
    }

    LastActualGPUDeltaBytes = int64(DeltaWords.Num()) * int64(sizeof(uint32));
    TotalActualGPUDeltaBytes += LastActualGPUDeltaBytes;
    ++GPUDeltaPacketCount;
    FCVXGPUPageUploadBridge::SubmitDeltaWords(DeltaWords);
}


void ACicadaVoxelVolumeActor::SubmitGPUPageDelta(const FCVXEditDelta& Delta)
{
    if (!bUploadChangedPagesToGPU || !bEnableVirtualPageSpine || Delta.IsEmpty())
    {
        return;
    }

    TArray<uint32> DeltaWords;
    VirtualPageSpine.BuildEditDeltaWords(Volume, Delta, DeltaWords);
    if (DeltaWords.Num() == 0)
    {
        return;
    }

    LastActualGPUDeltaBytes = int64(DeltaWords.Num()) * int64(sizeof(uint32));
    TotalActualGPUDeltaBytes += LastActualGPUDeltaBytes;
    ++GPUDeltaPacketCount;
    FCVXGPUPageUploadBridge::SubmitDeltaWords(DeltaWords);
}

void ACicadaVoxelVolumeActor::UpdatePerformanceString(int32 ProcessedChunks)
{
    if (IsPageNativeRuntimeActive())
    {
        LastPerformanceStats = FString::Printf(
            TEXT("CVX-003C REALTIME SURFACE | sparse=%.3fms | bulk[p=%d c=%d b=%d boundary=%d sub2=%d fineTests=%lld] | ray=%.3fms [16=%d 4=%d 1=%d] | pageKick=%.3fms pending=%d active=%d | exterior=%.3fms/%d | async[in=%d done=%d stale=%d worker=%.3fms] | gpuDelta=%lld B packets=%d | beam=%.1fmm carves/frame=%d hold=%d | throughput=%d deferMesh=%d | fullWorldUploads=%lld"),
            LastSparseEditMS,
            LastBulkPagesCleared,
            LastBulkChunksCleared,
            LastBulkBlocksCleared,
            LastBulkBoundaryBlocks,
            LastBulkSubBlocks2mmCleared,
            LastBulkFineVoxelTests,
            LastRayQueryMS,
            LastRayChunkSkips16mm,
            LastRayBlockSkips4mm,
            LastRayFineSteps1mm,
            LastSurfacePageMS,
            PendingSurfacePageCount,
            ActiveSurfacePageCount,
            LastExteriorSurfaceMS,
            LastExteriorPagesUpdated,
            AsyncSurfacePageJobsInFlight,
            AsyncSurfacePagesCompleted,
            AsyncSurfaceStaleResults,
            LastAsyncSurfaceWorkerMS,
            LastActualGPUDeltaBytes,
            GPUDeltaPacketCount,
            BeamCarveRadiusMM,
            LastBeamCarvesThisFrame,
            BeamCarvesThisHold,
            bThroughputFirstEditing ? 1 : 0,
            bDeferReferenceSurfaceWhileBeam ? 1 : 0,
            FullWorldHotUploadCount);
    }
    else if (IsExactSurfaceRuntimeActive())
    {
        LastPerformanceStats = FString::Printf(
            TEXT("CVX SINGLE OWNER SURFACE | sparse=%.3fms | ray=%.3fms [16=%d 4=%d 1=%d] | immediateMesh=%.3fms | mesh=%.3fms | meshChunks=%d | pendingMesh=%d | activeMesh=%d | lastQuads=%d lastTris=%d | totalQuads=%lld totalTris=%lld | singleOwner=%d proxyRetired=%d | handoffGuard=%d | immediateNeighbours=%d | beamRadius=%.1fmm beamCarves=%d throttled=%d | fullWorldUploads=%lld"),
            LastSparseEditMS,
            LastRayQueryMS,
            LastRayChunkSkips16mm,
            LastRayBlockSkips4mm,
            LastRayFineSteps1mm,
            LastImmediateSurfaceMS,
            LastSurfaceMeshUpdateMS,
            LastSurfaceMeshChunksProcessed,
            PendingSurfaceMeshChunkCount,
            ActiveSurfaceMeshChunks,
            LastSurfaceMeshQuads,
            LastSurfaceMeshTriangles,
            TotalSurfaceMeshQuads,
            TotalSurfaceMeshTriangles,
            bSingleOwnerRuntimeSurface ? 1 : 0,
            SurfaceProxyRetirements,
            SurfaceHandoffGuards,
            SurfaceImmediateNeighbourRebuilds,
            BeamCarveRadiusMM,
            BeamCarvesThisHold,
            BeamThrottleFrames,
            FullWorldHotUploadCount);
    }
    else
    {
        LastPerformanceStats = FString::Printf(
            TEXT("CVX BEAM + SURFACE PATH | sparse=%.3fms | ray=%.3fms [16=%d 4=%d 1=%d] | immediate=%.3fms | visual=%.3fms | blocks=%d | refined1mm=%d | chunks=%d | pending=%d | beamCarves=%d | throttled=%d | frontier=%d | reuse=%d | slots=%d/%d%s"),
            LastSparseEditMS,
            LastRayQueryMS,
            LastRayChunkSkips16mm,
            LastRayBlockSkips4mm,
            LastRayFineSteps1mm,
            LastImmediateSurfaceMS,
            LastVisualUpdateMS,
            LastVisualBlocksProcessed,
            LastRefinedSurfaceBlocksProcessed,
            ProcessedChunks,
            PendingVisualBlockCount,
            BeamCarvesThisHold,
            BeamThrottleFrames,
            FrontierInstanceByChunk.Num(),
            FineSlotBatchesReused,
            VisualAllocatedSlots,
            MaxVisualInstances,
            bVisualBudgetHit ? TEXT(" | SLOT BUDGET HIT") : TEXT(""));
    }

    UE_LOG(LogCicadaVoxelActor, Display, TEXT("%s"), *LastPerformanceStats);
}

void ACicadaVoxelVolumeActor::ShowRuntimeDebugMessage()
{
    if (GEngine && GetWorld() && GetWorld()->IsGameWorld())
    {
        const FString PageLine = bEnableVirtualPageSpine
            ? LastVirtualPageStats + TEXT("\n")
            : FString();

        FString RoboticsLine;
        if (IsRoboticsDirectFieldRuntimeActive())
        {
            const FCVXGPUPersistentFieldStats GPUStats =
                FCVXGPUPageUploadBridge::GetPersistentFieldStats();

            constexpr double PersistentBytes =
                double(FCVXGPUPageUploadBridge::PageHashWords
                    + FCVXGPUPageUploadBridge::PageMetaWords
                    + FCVXGPUPageUploadBridge::ChunkStateWords
                    + FCVXGPUPageUploadBridge::BlockMaskWords)
                * double(sizeof(uint32));

            RoboticsLine = FString::Printf(
                TEXT("CVX-004B PERSISTENT GPU | pages=%u/%u pending=%u coalesced=%llu cacheFull=%u submitted=%.2fMB fixedVRAM=%.1fMB fullWorldUploads=%lld\n"),
                GPUStats.AllocatedPages,
                GPUStats.PageCapacity,
                GPUStats.PendingPatches,
                GPUStats.CoalescedChunkReplacements,
                GPUStats.CacheFullEvents,
                double(GPUStats.SubmittedDeltaBytes) / (1024.0 * 1024.0),
                PersistentBytes / (1024.0 * 1024.0),
                FullWorldHotUploadCount);
        }

        GEngine->AddOnScreenDebugMessage(
            static_cast<uint64>(GetUniqueID()),
            8.0f,
            FColor::Cyan,
            LastStats + TEXT("\n") + LastVisualStats + TEXT("\n") + LastPerformanceStats + TEXT("\n") + RoboticsLine + PageLine + Last1mmWitness);
    }
}

void ACicadaVoxelVolumeActor::RebuildVisualization()
{
    EnsureInitialized();

    if (IsRoboticsDirectFieldRuntimeActive())
    {
        DisableLegacyRuntimeVisualsForRobotics();
        FCVXGPUPageUploadBridge::ResetPersistentField(uint32(GetUniqueID()));
        SubmitPersistentGPUStateBootstrap();
        MarkRoboticsFieldDirty();
        FlushRoboticsFieldSnapshot(0.0f, true);
        LastVisualStats = TEXT("CVX-004B persistent GPU sparse field | no per-frame field snapshot | NO legacy runtime skin");
        return;
    }

    if (!CoarseChunkInstances || !ExactColdShellInstances || !FrontierChunkInstances || !FullBlockInstances || !FineVoxelInstances)
    {
        return;
    }

    const double StartSeconds = FPlatformTime::Seconds();

    CoarseChunkInstances->ClearInstances();
    ExactColdShellInstances->ClearInstances();
    FrontierChunkInstances->ClearInstances();
    FullBlockInstances->ClearInstances();
    FineVoxelInstances->ClearInstances();
    ClearSurfaceMeshRuntime();
    ClearPageSurfaceRuntime();
    ResetVisualSlotCaches();

    // Reserve CPU-side instance storage for the hot representation. This does not create
    // instances; it prevents repeated array reallocations as new mixed 4 mm blocks expose
    // their 64 exact 1 mm cells during destruction.
    if (HotBlockReserveInstances > 0)
    {
        FullBlockInstances->PreAllocateInstancesMemory(FMath::Min(HotBlockReserveInstances, MaxVisualInstances));
    }
    if (HotFineReserveInstances > 0)
    {
        FineVoxelInstances->PreAllocateInstancesMemory(FMath::Min(HotFineReserveInstances, MaxVisualInstances));
    }

    const FIntVector Size = Volume.GetLogicalSizeMM();
    const FIntVector ChunkCounts(
        FMath::DivideAndRoundUp(Size.X, CVXChunkMM),
        FMath::DivideAndRoundUp(Size.Y, CVXChunkMM),
        FMath::DivideAndRoundUp(Size.Z, CVXChunkMM));

    TArray<FIntVector> DirtyChunks;
    Volume.GetDirtyChunkCoords(DirtyChunks);

    TSet<FIntVector> CoarseChunks;
    CoarseChunks.Reserve(
        2 * ChunkCounts.X * ChunkCounts.Y
        + 2 * ChunkCounts.X * ChunkCounts.Z
        + 2 * ChunkCounts.Y * ChunkCounts.Z
        + DirtyChunks.Num() * 6);

    // CVX-002A: enumerate only the six boundary faces instead of scanning every 16 mm chunk
    // inside the volume. A 1 m proof now performs O(surface) shell discovery, not O(volume).
    auto AddBoundaryIfUntouched = [this, &CoarseChunks](const FIntVector& Coord)
    {
        if (!Volume.IsChunkDirty(Coord))
        {
            CoarseChunks.Add(Coord);
        }
    };

    if (ChunkCounts.X > 0 && ChunkCounts.Y > 0 && ChunkCounts.Z > 0)
    {
        for (int32 Z = 0; Z < ChunkCounts.Z; ++Z)
        {
            for (int32 Y = 0; Y < ChunkCounts.Y; ++Y)
            {
                AddBoundaryIfUntouched(FIntVector(0, Y, Z));
                AddBoundaryIfUntouched(FIntVector(ChunkCounts.X - 1, Y, Z));
            }
        }

        for (int32 Z = 0; Z < ChunkCounts.Z; ++Z)
        {
            for (int32 X = 0; X < ChunkCounts.X; ++X)
            {
                AddBoundaryIfUntouched(FIntVector(X, 0, Z));
                AddBoundaryIfUntouched(FIntVector(X, ChunkCounts.Y - 1, Z));
            }
        }

        for (int32 Y = 0; Y < ChunkCounts.Y; ++Y)
        {
            for (int32 X = 0; X < ChunkCounts.X; ++X)
            {
                AddBoundaryIfUntouched(FIntVector(X, Y, 0));
                AddBoundaryIfUntouched(FIntVector(X, Y, ChunkCounts.Z - 1));
            }
        }
    }


    TArray<FTransform> CoarseTransforms;
    TArray<FIntVector> CoarseCoords;
    CoarseTransforms.Reserve(CoarseChunks.Num());
    CoarseCoords.Reserve(CoarseChunks.Num());

    for (const FIntVector& Coord : CoarseChunks)
    {
        if (VisualAllocatedSlots + CoarseTransforms.Num() >= MaxVisualInstances)
        {
            bVisualBudgetHit = true;
            break;
        }

        CoarseTransforms.Add(MakeCoarseChunkTransform(Coord, true));
        CoarseCoords.Add(Coord);
    }

    if (CoarseTransforms.Num() > 0)
    {
        UInstancedStaticMeshComponent* ColdShell = GetActiveColdShellComponent();
        if (ColdShell)
        {
            ColdShell->PreAllocateInstancesMemory(CoarseTransforms.Num());
            const TArray<int32> CoarseIndices = ColdShell->AddInstances(CoarseTransforms, true, false, false);
            VisualAllocatedSlots += CoarseIndices.Num();
            const int32 PairCount = FMath::Min(CoarseIndices.Num(), CoarseCoords.Num());
            for (int32 Index = 0; Index < PairCount; ++Index)
            {
                CoarseInstanceByChunk.Add(CoarseCoords[Index], CoarseIndices[Index]);
            }
            if ((IsPageNativeRuntimeActive() || IsExactSurfaceRuntimeActive()) && bSingleOwnerRuntimeSurface)
            {
                ColdShell->MarkRenderInstancesDirty();
            }
        }
    }

    if (IsPageNativeRuntimeActive())
    {
        // CVX-003A active runtime: the old per-16 mm hot DynamicMesh queue is no
        // longer the visible editing path. Exact surface ownership is promoted in
        // coherent 64 mm pages, matching the virtual-page / GPU-delta granularity.
        CoarseChunkInstances->ClearInstances();
        FrontierChunkInstances->ClearInstances();
        FullBlockInstances->ClearInstances();
        FineVoxelInstances->ClearInstances();

        QueueSurfacePagesFromDirtyChunks();

        // Build the small initial proof coherently. Runtime edits after BeginPlay are
        // page-budgeted and never trigger a whole-volume visual rebuild.
        int32 Safety = 0;
        while (PendingSurfacePageSet.Num() > 0 && Safety++ < 1024)
        {
            const int32 Before = PendingSurfacePageSet.Num();
            const float SavedBudget = SurfacePageBudgetMS;
            const int32 SavedMax = MaxSurfacePagesPerFrame;
            SurfacePageBudgetMS = 12.0f;
            MaxSurfacePagesPerFrame = 16;
            ProcessPendingSurfacePageUpdates();
            SurfacePageBudgetMS = SavedBudget;
            MaxSurfacePagesPerFrame = SavedMax;
            if (PendingSurfacePageSet.Num() >= Before)
            {
                break;
            }
        }

        UpdateSurfacePageStats();
        LastFullVisualRebuildMS = float((FPlatformTime::Seconds() - StartSeconds) * 1000.0);
        LastVisualStats = FString::Printf(
            TEXT("CVX-003A page-native reference | cold16=%d | pageMeshes=%d | pendingPages=%d | lastQuads=%d | fullRebuild=%.2fms | legacy16mmHot=OFF"),
            CoarseInstanceByChunk.Num(),
            ActiveSurfacePageCount,
            PendingSurfacePageCount,
            LastSurfacePageQuads,
            LastFullVisualRebuildMS);
        UE_LOG(LogCicadaVoxelActor, Display, TEXT("%s"), *LastVisualStats);
        UpdatePerformanceString(0);
        ShowRuntimeDebugMessage();
        return;
    }

    if (IsExactSurfaceRuntimeActive())
    {
        // Exact runtime representation: keep only the untouched outer shell in HISM.
        // Every edited chunk and its six-neighbour frontier is extracted directly from
        // the authoritative 1 mm field into tiny 16 mm dynamic-mesh components.
        // Single-owner runtime: the exact path never leaves the HISM cold shell active.
        // The plain ISM cold shell is the only coarse owner, and each chunk is retired once
        // its exact dynamic mesh is installed. This removes stale HISM cluster rectangles.
        CoarseChunkInstances->ClearInstances();
        FrontierChunkInstances->ClearInstances();
        FullBlockInstances->ClearInstances();
        FineVoxelInstances->ClearInstances();

        QueueSurfaceMeshFromDirtyChunks();

        // Initial proof is small enough to build immediately so the first visible frame
        // is coherent. Subsequent edits use the per-frame mesh budget.
        int32 Safety = 0;
        while (PendingSurfaceMeshSet.Num() > 0 && Safety++ < 4096)
        {
            const int32 Before = PendingSurfaceMeshSet.Num();
            const float SavedBudget = SurfaceMeshBudgetMS;
            const int32 SavedMax = MaxSurfaceMeshChunksPerFrame;
            SurfaceMeshBudgetMS = 8.0f;
            MaxSurfaceMeshChunksPerFrame = 32;
            ProcessPendingSurfaceMeshUpdates();
            SurfaceMeshBudgetMS = SavedBudget;
            MaxSurfaceMeshChunksPerFrame = SavedMax;
            if (PendingSurfaceMeshSet.Num() >= Before)
            {
                break;
            }
        }

        if (!(bSingleOwnerRuntimeSurface && ExactColdShellInstances))
        {
            KickTreeBuild(CoarseChunkInstances, true);
        }
        UpdateSurfaceMeshStats();
        LastFullVisualRebuildMS = float((FPlatformTime::Seconds() - StartSeconds) * 1000.0);
        LastVisualStats = FString::Printf(
            TEXT("CVX exact surface runtime | singleOwner=%d | coldShellISM=%d | exactMeshChunks=%d | quads=%lld | tris=%lld | fullRebuild=%.2fms"),
            bSingleOwnerRuntimeSurface ? 1 : 0,
            CoarseInstanceByChunk.Num(),
            ActiveSurfaceMeshChunks,
            TotalSurfaceMeshQuads,
            TotalSurfaceMeshTriangles,
            LastFullVisualRebuildMS);
        UE_LOG(LogCicadaVoxelActor, Display, TEXT("%s"), *LastVisualStats);
        UpdatePerformanceString(0);
        ShowRuntimeDebugMessage();
        return;
    }

    bool bTouchedFrontier = false;
    bool bTouchedBlocks = false;
    bool bTouchedFine = false;
    for (const FIntVector& Dirty : DirtyChunks)
    {
        EnsureSolidFrontierAroundChunk(Dirty, bTouchedFrontier);
        UpdateHotChunkVisual(Dirty, bTouchedBlocks, bTouchedFine);
    }

    if (bTouchedFrontier)
    {
        FrontierChunkInstances->MarkRenderInstancesDirty();
    }

    KickTreeBuild(CoarseChunkInstances, true);

    LastFullVisualRebuildMS = float((FPlatformTime::Seconds() - StartSeconds) * 1000.0);
    LastVisualStats = FString::Printf(
        TEXT("CVX solid visualizer | staticShellHISM + frontierISM + hotISM | allocated=%d/%d | coarseSlots=%d | frontier=%d | hotChunks=%d | freeFineBatches=%d | fullRebuild=%.2fms%s"),
        VisualAllocatedSlots,
        MaxVisualInstances,
        CoarseInstanceByChunk.Num(),
        FrontierInstanceByChunk.Num(),
        HotChunkVisualSlots.Num(),
        FreeFineSlotBatches.Num(),
        LastFullVisualRebuildMS,
        bVisualBudgetHit ? TEXT(" | SLOT BUDGET HIT") : TEXT(""));

    UE_LOG(LogCicadaVoxelActor, Display, TEXT("%s"), *LastVisualStats);
    UpdatePerformanceString(0);
    ShowRuntimeDebugMessage();
}

bool ACicadaVoxelVolumeActor::SegmentBoxEntry(
    const FVector& SegmentStart,
    const FVector& SegmentEnd,
    const FBox& Box,
    FVector& OutEntry) const
{
    const FVector Delta = SegmentEnd - SegmentStart;
    double TMin = 0.0;
    double TMax = 1.0;

    for (int32 Axis = 0; Axis < 3; ++Axis)
    {
        const double Start = SegmentStart[Axis];
        const double D = Delta[Axis];
        const double MinV = Box.Min[Axis];
        const double MaxV = Box.Max[Axis];

        if (FMath::Abs(D) < 1.0e-12)
        {
            if (Start < MinV || Start > MaxV)
            {
                return false;
            }
            continue;
        }

        const double InvD = 1.0 / D;
        double T1 = (MinV - Start) * InvD;
        double T2 = (MaxV - Start) * InvD;
        if (T1 > T2)
        {
            Swap(T1, T2);
        }

        TMin = FMath::Max(TMin, T1);
        TMax = FMath::Min(TMax, T2);
        if (TMin > TMax)
        {
            return false;
        }
    }

    OutEntry = SegmentStart + Delta * TMin;
    return true;
}

bool ACicadaVoxelVolumeActor::TryCarveFromPlayerView(float DeltaSeconds)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return false;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (!PC)
    {
        return false;
    }

    const FKey LeftMouseButtonKey(TEXT("LeftMouseButton"));
    const FKey MacroBlastKey(TEXT("B"));

    // Large one-shot benchmark. This deliberately uses the same authoritative edit
    // path as the beam so it measures the actual hierarchical stamp core.
    if (PC->WasInputKeyJustPressed(MacroBlastKey))
    {
        const bool bBlasted = CarveAtCurrentViewRay(PC, MacroBlastRadiusMM);
        if (bBlasted)
        {
            ++MacroBlastCount;
        }
        return bBlasted;
    }

    const bool bPressedThisFrame = PC->WasInputKeyJustPressed(LeftMouseButtonKey);
    const bool bDown = PC->IsInputKeyDown(LeftMouseButtonKey);

    LastBeamCarvesThisFrame = 0;

    if (!bDown)
    {
        BeamEditAccumulator = 0.0f;
        bBeamWasDownLastFrame = false;
        BeamCarvesThisHold = 0;
        return false;
    }

    float RadiusMM = FirstPersonCarveRadiusMM;
    int32 CarvesDue = bPressedThisFrame ? 1 : 0;

    if (bContinuousBeamDigging)
    {
        const float RequestedRate = FMath::Max(1.0f, BeamEditsPerSecond);
        const float EffectiveRate = bThroughputFirstEditing
            ? FMath::Max(RequestedRate, ThroughputBeamMinEditsPerSecond)
            : RequestedRate;
        const float Interval = 1.0f / EffectiveRate;
        RadiusMM = BeamCarveRadiusMM;

        if (!bBeamWasDownLastFrame)
        {
            BeamEditAccumulator = Interval;
            BeamCarvesThisHold = 0;
        }
        else
        {
            BeamEditAccumulator += FMath::Max(0.0f, DeltaSeconds);
        }

        const int32 AvailableCarves = FMath::FloorToInt(BeamEditAccumulator / Interval);
        CarvesDue = FMath::Max(CarvesDue, AvailableCarves);
        CarvesDue = FMath::Clamp(CarvesDue, 0, FMath::Max(1, MaxBeamCarvesPerFrame));
        BeamEditAccumulator = FMath::Max(0.0f, BeamEditAccumulator - float(CarvesDue) * Interval);
    }

    bBeamWasDownLastFrame = true;

    if (CarvesDue <= 0)
    {
        return false;
    }

    // CVX-003B throughput mode deliberately disconnects authoritative edits from the
    // temporary reference renderer. Large-world truth is not allowed to stall because
    // a CPU DynamicMesh page is late. The page queue simply catches up afterwards.
    if (!bThroughputFirstEditing)
    {
        if (IsPageNativeRuntimeActive())
        {
            const int32 PageBackpressure = FMath::Max(4, BeamSurfaceMeshBackpressureChunks / 4);
            if (PageBackpressure > 0 && PendingSurfacePageCount >= PageBackpressure)
            {
                ++BeamThrottleFrames;
                return false;
            }
        }
        else if (IsExactSurfaceRuntimeActive())
        {
            if (BeamSurfaceMeshBackpressureChunks > 0
                && PendingSurfaceMeshChunkCount >= BeamSurfaceMeshBackpressureChunks)
            {
                ++BeamThrottleFrames;
                return false;
            }
        }
        else if (BeamVisualBackpressureBlocks > 0
            && PendingVisualBlockCount >= BeamVisualBackpressureBlocks)
        {
            ++BeamThrottleFrames;
            return false;
        }
    }

    bool bAnyCarved = false;
    for (int32 Attempt = 0; Attempt < CarvesDue; ++Attempt)
    {
        if (!CarveAtCurrentViewRay(PC, RadiusMM))
        {
            break;
        }

        bAnyCarved = true;
        ++BeamCarvesThisHold;
        ++LastBeamCarvesThisFrame;
    }

    return bAnyCarved;
}

bool ACicadaVoxelVolumeActor::CarveAtCurrentViewRay(APlayerController* PC, float RadiusMM)
{
    if (!PC)
    {
        return false;
    }

    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    const FVector WorldEnd = CameraLocation + CameraRotation.Vector() * FirstPersonMaxDistanceCM;
    const FTransform ActorTransform = GetActorTransform();
    const FVector LocalStart = ActorTransform.InverseTransformPosition(CameraLocation);
    const FVector LocalEnd = ActorTransform.InverseTransformPosition(WorldEnd);

    const FVector SizeCM(
        double(LogicalSizeMM.X) * 0.1,
        double(LogicalSizeMM.Y) * 0.1,
        double(LogicalSizeMM.Z) * 0.1);
    const FBox LocalBox(FVector::ZeroVector, SizeCM);

    FVector EntryCM;
    if (!SegmentBoxEntry(LocalStart, LocalEnd, LocalBox, EntryCM))
    {
        return false;
    }

    const FVector LocalDir = (LocalEnd - LocalStart).GetSafeNormal();
    if (LocalDir.IsNearlyZero())
    {
        return false;
    }

    FIntVector HitVoxelMM = FIntVector::ZeroValue;
    FCVXRayTraversalStats RayStats;
    const double QueryStart = FPlatformTime::Seconds();
    bool bHit = false;

    if (bUseHierarchicalRayTraversal)
    {
        const FVector EntryMM = EntryCM * 10.0 + LocalDir * 0.01;
        const FVector EndMM = LocalEnd * 10.0;
        bHit = Volume.FindFirstSolidAlongSegmentMM(
            FVector3d(EntryMM.X, EntryMM.Y, EntryMM.Z),
            FVector3d(EndMM.X, EndMM.Y, EndMM.Z),
            HitVoxelMM,
            RayStats);
    }
    else
    {
        FVector ProbeCM = EntryCM + LocalDir * 0.05;
        const double StepCM = 0.1;
        const int32 MaxSteps = 65536;

        for (int32 Step = 0; Step < MaxSteps && LocalBox.IsInsideOrOn(ProbeCM); ++Step)
        {
            ++RayStats.FineSteps1mm;
            const FVector ProbeMM = ProbeCM * 10.0;
            const FIntVector VoxelMM(
                FMath::FloorToInt(ProbeMM.X),
                FMath::FloorToInt(ProbeMM.Y),
                FMath::FloorToInt(ProbeMM.Z));

            if (Volume.IsSolid(VoxelMM))
            {
                HitVoxelMM = VoxelMM;
                bHit = true;
                break;
            }

            ProbeCM += LocalDir * StepCM;
        }
    }

    LastRayQueryMS = float((FPlatformTime::Seconds() - QueryStart) * 1000.0);
    LastRayChunkSkips16mm = RayStats.ChunkSkips16mm;
    LastRayBlockSkips4mm = RayStats.BlockSkips4mm;
    LastRayFineSteps1mm = RayStats.FineSteps1mm;

    if (!bHit)
    {
        return false;
    }

    FVector CenterMM(
        double(HitVoxelMM.X) + 0.5,
        double(HitVoxelMM.Y) + 0.5,
        double(HitVoxelMM.Z) + 0.5);
    CenterMM += LocalDir * (RadiusMM * 0.55f);
    CenterMM.X = FMath::Clamp(CenterMM.X, 0.0, double(LogicalSizeMM.X));
    CenterMM.Y = FMath::Clamp(CenterMM.Y, 0.0, double(LogicalSizeMM.Y));
    CenterMM.Z = FMath::Clamp(CenterMM.Z, 0.0, double(LogicalSizeMM.Z));

    CarveSphereLocalMM(CenterMM, RadiusMM);
    return true;
}

