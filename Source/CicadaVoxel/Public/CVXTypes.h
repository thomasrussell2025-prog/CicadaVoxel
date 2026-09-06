#pragma once

#include "CoreMinimal.h"

namespace CVX
{
    static constexpr int32 VoxelSizeMM = 1;
    static constexpr int32 BlockEdgeVoxels = 4;
    static constexpr int32 ChunkEdgeBlocks = 4;
    static constexpr int32 ChunkEdgeVoxels = BlockEdgeVoxels * ChunkEdgeBlocks; // 16
    static constexpr int32 VoxelsPerBlock = BlockEdgeVoxels * BlockEdgeVoxels * BlockEdgeVoxels; // 64
    static constexpr int32 BlocksPerChunk = ChunkEdgeBlocks * ChunkEdgeBlocks * ChunkEdgeBlocks; // 64
    static constexpr uint64 FullBlockMask = ~uint64(0);

    enum class EBlockState : uint8
    {
        Empty = 0,
        Full = 1,
        Mixed = 2,
        Procedural = 3
    };

    struct FCVXGPUUint4
    {
        uint32 X = 0;
        uint32 Y = 0;
        uint32 Z = 0;
        uint32 W = 0;
    };
    static_assert(sizeof(FCVXGPUUint4) == 16, "CVX GPU uint4 must remain 16 bytes.");

    struct FCVXGPUUint2
    {
        uint32 X = 0;
        uint32 Y = 0;
    };
    static_assert(sizeof(FCVXGPUUint2) == 8, "CVX GPU uint2 must remain 8 bytes.");

    FORCEINLINE int32 LocalBlockIndex(int32 X, int32 Y, int32 Z)
    {
        return X + Y * ChunkEdgeBlocks + Z * ChunkEdgeBlocks * ChunkEdgeBlocks;
    }

    FORCEINLINE int32 LocalVoxelBit(int32 X, int32 Y, int32 Z)
    {
        return X + Y * BlockEdgeVoxels + Z * BlockEdgeVoxels * BlockEdgeVoxels;
    }
}
