#include "CVXGPUBridge.h"
#include "RenderingThread.h"

FCVXRenderSnapshot FCVXGPUBridge::RenderThreadSnapshot;

void FCVXGPUBridge::Publish(uint64 Generation, const TArray<CVX::FCVXGPUUint4>& ChunkWords, const TArray<CVX::FCVXGPUUint2>& MixedMasks)
{
    TArray<CVX::FCVXGPUUint4> WordsCopy = ChunkWords;
    TArray<CVX::FCVXGPUUint2> MasksCopy = MixedMasks;

    ENQUEUE_RENDER_COMMAND(CVXPublishSparseSnapshot)(
        [Generation, Words = MoveTemp(WordsCopy), Masks = MoveTemp(MasksCopy)](FRHICommandListImmediate&) mutable
        {
            check(IsInRenderingThread());
            FCVXGPUBridge::RenderThreadSnapshot.Generation = Generation;
            FCVXGPUBridge::RenderThreadSnapshot.ChunkWords = MoveTemp(Words);
            FCVXGPUBridge::RenderThreadSnapshot.MixedMasks = MoveTemp(Masks);
        });
}

const FCVXRenderSnapshot& FCVXGPUBridge::GetRenderThreadSnapshot()
{
    check(IsInRenderingThread());
    return RenderThreadSnapshot;
}
