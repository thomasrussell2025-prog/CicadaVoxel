#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"
#include "RenderGraphResources.h"

class FCVXRoboticsFieldViewExtension final : public FSceneViewExtensionBase
{
public:
    FCVXRoboticsFieldViewExtension(const FAutoRegister& AutoRegister);

    virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}
    virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}
    virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {}

    virtual void SubscribeToPostProcessingPass(
        EPostProcessingPass Pass,
        const FSceneView& InView,
        FPostProcessingPassDelegateArray& InOutPassCallbacks,
        bool bIsPassEnabled) override;

private:
    struct FCVXPersistentBuffers
    {
        TRefCountPtr<FRDGPooledBuffer> PageHash;
        TRefCountPtr<FRDGPooledBuffer> PageMeta;
        TRefCountPtr<FRDGPooledBuffer> ChunkStates;
        TRefCountPtr<FRDGPooledBuffer> BlockMasks;
        bool bInitialized = false;
        uint32 LastResetSerial = 0;
    };

    void EnsurePersistentBuffers(
        FRDGBuilder& GraphBuilder,
        FRDGBufferRef& OutPageHash,
        FRDGBufferRef& OutPageMeta,
        FRDGBufferRef& OutChunkStates,
        FRDGBufferRef& OutBlockMasks);

    void ApplyPendingFieldPatches(
        FRDGBuilder& GraphBuilder,
        const FSceneView& View,
        FRDGBufferRef PageHash,
        FRDGBufferRef PageMeta,
        FRDGBufferRef ChunkStates,
        FRDGBufferRef BlockMasks);

    FScreenPassTexture RenderRoboticsField(
        FRDGBuilder& GraphBuilder,
        const FSceneView& View,
        const FPostProcessMaterialInputs& Inputs);

    FCVXPersistentBuffers Persistent;
};
