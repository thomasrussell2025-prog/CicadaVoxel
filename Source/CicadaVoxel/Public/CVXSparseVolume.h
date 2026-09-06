#pragma once

#include "CoreMinimal.h"
#include "CVXTypes.h"


struct CICADAVOXEL_API FCVXChangedBlock
{
    FIntVector ChunkCoord = FIntVector::ZeroValue;
    int32 BlockIndex = 0;

    FCVXChangedBlock() = default;
    FCVXChangedBlock(const FIntVector& InChunkCoord, int32 InBlockIndex)
        : ChunkCoord(InChunkCoord), BlockIndex(InBlockIndex)
    {
    }
};



struct CICADAVOXEL_API FCVXEditDelta
{
    // Entire 64 mm pages cleared by the hierarchical subtract fast path.
    TArray<FIntVector> ClearedPages64MM;

    // Entire 16 mm chunks cleared inside partially intersected pages.
    TArray<FIntVector> ClearedChunks16MM;

    // Only boundary 4 mm blocks that required exact mask updates.
    TArray<FCVXChangedBlock> ChangedBlocks;

    void Reset()
    {
        ClearedPages64MM.Reset();
        ClearedChunks16MM.Reset();
        ChangedBlocks.Reset();
    }

    bool IsEmpty() const
    {
        return ClearedPages64MM.Num() == 0
            && ClearedChunks16MM.Num() == 0
            && ChangedBlocks.Num() == 0;
    }
};

struct CICADAVOXEL_API FCVXBulkEditStats
{
    int32 PagesVisited = 0;
    int32 PagesCleared = 0;
    int32 ChunksVisited = 0;
    int32 ChunksCleared = 0;
    int32 BlocksVisited = 0;
    int32 BlocksCleared = 0;
    int32 BoundaryBlocks = 0;
    int32 SubBlocks2mmCleared = 0;
    int64 FineVoxelTests = 0;
    int64 VoxelBitsCleared = 0;

    FString ToString() const;
};

struct CICADAVOXEL_API FCVXRayTraversalStats
{
    int32 ChunkSkips16mm = 0;
    int32 BlockSkips4mm = 0;
    int32 FineSteps1mm = 0;
    int32 Samples = 0;
};

struct CICADAVOXEL_API FCVXVolumeStats
{
    int64 LogicalVoxelCount = 0;
    int32 DirtyChunkCount = 0;
    int32 MixedBlockCount = 0;
    int32 EmptyBlockCount = 0;
    int32 FullBlockCount = 0;
    uint64 CompactGPUBytes = 0;
    uint64 Dense16BitBytes = 0;

    FString ToString() const;
};

class CICADAVOXEL_API FCVXSparseVolume
{
public:
    FCVXSparseVolume();

    void Reset(const FIntVector& InLogicalSizeMM, uint16 InDefaultMaterial = 1);

    // Local coordinates are integer millimetre cells from the volume minimum corner.
    // CarveVoxel is the exact addressable 1 mm edit primitive: one call clears one logical cell only.
    bool CarveVoxel(const FIntVector& VoxelMM);
    void CarveSphere(const FVector3d& CenterMM, double RadiusMM);

    // Same subtraction, but reports only 16 mm chunks whose stored masks actually changed.
    void CarveSphere(const FVector3d& CenterMM, double RadiusMM, TArray<FIntVector>& OutChangedChunks);

    // CVX-002B hot path: report the exact 4 mm blocks that changed. The renderer can now
    // update one 4^3 block (and its 64 one-millimetre cells) instead of revisiting all
    // 64 blocks in every touched 16 mm chunk.
    void CarveSphere(const FVector3d& CenterMM, double RadiusMM, TArray<FCVXChangedBlock>& OutChangedBlocks);

    // CVX-003B: hierarchical subtract stamp. Large edits classify 64 mm pages first,
    // then 16 mm chunks, then 4 mm blocks. Only the geometric boundary falls back
    // to exact 1 mm occupancy tests. Interior volume is cleared in coarse O(1) records.
    void CarveSphereHierarchical(
        const FVector3d& CenterMM,
        double RadiusMM,
        FCVXEditDelta& OutDelta,
        FCVXBulkEditStats& OutStats);

    bool IsPageCompletelyEmpty64MM(const FIntVector& PageCoord) const;
    void GetFullyEmptyPageCoords64MM(TArray<FIntVector>& OutPages) const;

    bool IsSolid(const FIntVector& VoxelMM) const;

    // Hierarchical CPU traversal used by the temporary interaction path. Empty 16 mm chunks
    // are skipped first, then empty 4 mm blocks, and only Mixed blocks fall back to 1 mm steps.
    // The final custom GPU renderer will use the same 16 -> 4 -> 1 hierarchy with stronger
    // directional skip caches.
    bool FindFirstSolidAlongSegmentMM(
        const FVector3d& StartMM,
        const FVector3d& EndMM,
        FIntVector& OutVoxelMM,
        FCVXRayTraversalStats& OutStats) const;

    bool IsChunkCompletelyEmpty(const FIntVector& ChunkCoord) const;

    const FIntVector& GetLogicalSizeMM() const { return LogicalSizeMM; }
    uint16 GetDefaultMaterial() const { return DefaultMaterial; }
    uint64 GetGeneration() const { return Generation; }

    FCVXVolumeStats GetStats() const;

    // Lightweight read-only visualization/debug access. These do not allocate or mutate sparse data.
    void GetDirtyChunkCoords(TArray<FIntVector>& OutCoords) const;
    bool IsChunkDirty(const FIntVector& ChunkCoord) const;
    CVX::EBlockState GetBlockStateForVisualization(const FIntVector& ChunkCoord, int32 BlockIndex) const;
    uint64 GetBlockMaskForVisualization(const FIntVector& ChunkCoord, int32 BlockIndex) const;

    // GPU validation format: 3 uint4 rows per dirty chunk plus packed uint2 mixed masks.
    void BuildGPUData(TArray<CVX::FCVXGPUUint4>& OutChunkWords, TArray<CVX::FCVXGPUUint2>& OutMixedMasks) const;

private:
    struct FCVXChunk
    {
        // 64 block states, two bits each => 128 bits => four uint32 words.
        uint32 StateWords[4] = { 0x55555555u, 0x55555555u, 0x55555555u, 0x55555555u };

        // Bit N means block N has an entry in MixedMasks. Entries are kept in block-index order.
        uint64 MixedPresence = 0;
        TArray<uint64> MixedMasks;

        CVX::EBlockState GetState(int32 BlockIndex) const;
        void SetState(int32 BlockIndex, CVX::EBlockState NewState);
        uint64 GetMask(int32 BlockIndex) const;
        void SetMaskAndCompact(int32 BlockIndex, uint64 NewMask);
        int32 MixedArrayIndex(int32 BlockIndex) const;
        bool IsImplicitFull() const;
        bool IsCompletelyEmpty() const;
    };

    static constexpr int32 BulkPageEdgeChunks = 4;
    static constexpr int32 BulkPageEdgeVoxels = CVX::ChunkEdgeVoxels * BulkPageEdgeChunks; // 64 mm

    static uint32 PopCount64(uint64 Value);
    static double MinDistanceSquaredToAABB(const FVector3d& Point, const FVector3d& Min, const FVector3d& Max);
    static double MaxDistanceSquaredToAABB(const FVector3d& Point, const FVector3d& Min, const FVector3d& Max);
    static double DistanceToNextGridBoundaryMM(const FVector3d& PositionMM, const FVector3d& Direction, double CellSizeMM);

    FCVXChunk& FindOrAddChunk(const FIntVector& ChunkCoord);
    const FCVXChunk* FindChunk(const FIntVector& ChunkCoord) const;

    static FIntVector VoxelToChunk(const FIntVector& VoxelMM);
    static FIntVector VoxelToBulkPage64MM(const FIntVector& VoxelMM);
    static FIntVector ChunkToBulkPage64MM(const FIntVector& ChunkCoord);
    static FIntVector VoxelToLocalBlock(const FIntVector& VoxelMM);
    static FIntVector VoxelToLocalInBlock(const FIntVector& VoxelMM);

    FIntVector LogicalSizeMM = FIntVector(1000, 1000, 1000);
    uint16 DefaultMaterial = 1;
    uint64 Generation = 1;

    // Subtractive page tombstones. A fully cleared 64 mm page is one record instead of
    // 64 empty chunk records / 4096 empty block records. Since CVX currently supports
    // subtraction only, a tombstoned page can never need a finer representation later.
    TSet<FIntVector> FullyEmptyPages64MM;

    // Same subtract-only tombstone at 16 mm granularity for chunks that are fully
    // covered by a stamp inside a partially intersected 64 mm page.
    TSet<FIntVector> FullyEmptyChunks16MM;

    TMap<FIntVector, FCVXChunk> DirtyChunks;
};
