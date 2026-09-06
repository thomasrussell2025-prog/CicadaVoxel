#pragma once

#include "CoreMinimal.h"
#include "CVXTypes.h"

struct FCVXRenderSnapshot
{
    uint64 Generation = 0;
    TArray<CVX::FCVXGPUUint4> ChunkWords;
    TArray<CVX::FCVXGPUUint2> MixedMasks;

    int32 NumChunks() const { return ChunkWords.Num() / 3; }
};

class FCVXGPUBridge
{
public:
    static void Publish(uint64 Generation, const TArray<CVX::FCVXGPUUint4>& ChunkWords, const TArray<CVX::FCVXGPUUint2>& MixedMasks);
    static const FCVXRenderSnapshot& GetRenderThreadSnapshot();

private:
    static FCVXRenderSnapshot RenderThreadSnapshot;
};
