#pragma once

#include "CoreMinimal.h"
#include "CVXSparseVolume.h"

// CVX-002D virtual hot-page model.
//
// This is deliberately independent from the temporary ISM/HISM renderer. It is the memory
// contract for the custom GPU renderer that follows:
//   1 mm voxels -> 4 mm blocks -> 16 mm chunks -> 64 mm hot pages.
//
// Untouched material remains implicit/procedural. Only edited 16 mm chunks are represented
// explicitly inside hot pages, so world memory scales with modification rather than world size.

struct CICADAVOXEL_API FCVXVirtualPageStats
{
    int32 ResidentEditedPages = 0;
    int32 ExplicitChunks = 0;
    int32 FullyEmptyPages = 0;
    int32 MixedBlocks = 0;

    uint64 PageMapBytes = 0;
    uint64 ChunkRecordBytes = 0;
    uint64 MixedMaskBytes = 0;
    uint64 ResidentPayloadBytes = 0;

    int32 LastChangedPages = 0;
    int32 LastChangedChunks = 0;
    uint64 LastDeltaBytes = 0;
    uint64 TotalDeltaBytes = 0;

    // This must remain zero on the hot-edit path. BootstrapFromVolume scans only the sparse
    // dirty set; it never serializes the logical world volume.
    uint64 FullWorldUploadCount = 0;

    FString ToString() const;
};

class CICADAVOXEL_API FCVXVirtualPageSpine
{
public:
    static constexpr int32 PageEdgeChunks = 4;
    static constexpr int32 PageEdgeMM = CVX::ChunkEdgeVoxels * PageEdgeChunks; // 64 mm
    static constexpr int32 ChunksPerPage = PageEdgeChunks * PageEdgeChunks * PageEdgeChunks; // 64

    // Proposed GPU memory contract. A resident page has a small fixed page descriptor plus a
    // 64-entry chunk map. Mixed/edited chunks then use compact records and 64-bit leaf masks.
    static constexpr uint64 PageDescriptorBytes = 16;
    static constexpr uint64 PageChunkMapBytes = ChunksPerPage * sizeof(uint32); // 256
    static constexpr uint64 ChunkRecordBytes = 32;
    static constexpr uint64 MixedMaskBytes = sizeof(uint64);

    void Reset();

    // Bootstrap only from the already-sparse dirty chunk set. This is intentionally not a
    // logical-world scan and therefore remains suitable even when the logical world is huge.
    void RebuildFromVolume(const FCVXSparseVolume& Volume);

    // Hot path. Changed blocks are coalesced to unique 16 mm chunks and then unique 64 mm
    // pages. The estimated transfer is bounded by the changed sparse payload, never world size.
    void ApplyChangedBlocks(const FCVXSparseVolume& Volume, const TArray<FCVXChangedBlock>& ChangedBlocks);

    // CVX-003B hierarchical edit contract. Fully cleared 64 mm pages are represented
    // as one tombstone each; cleared chunks and exact boundary blocks stay sparse.
    void ApplyEditDelta(const FCVXSparseVolume& Volume, const FCVXEditDelta& Delta);

    // CVX-003A real upload packet. Only changed 16 mm chunks are serialized. Each
    // chunk contains four packed 2-bit block-state words plus the mixed 4 mm masks
    // required for exact 1 mm occupancy. This packet is suitable for a GPU delta ring.
    void BuildChangedChunkDeltaWords(
        const FCVXSparseVolume& Volume,
        const TArray<FCVXChangedBlock>& ChangedBlocks,
        TArray<uint32>& OutWords) const;

    // CVX-003B packet format. CLEAR_PAGE commands are 16 bytes regardless of the
    // 262,144 logical 1 mm cells represented by that page. Remaining changed chunks
    // use the same exact 4 mm state / 1 mm mixed-mask payload.
    void BuildEditDeltaWords(
        const FCVXSparseVolume& Volume,
        const FCVXEditDelta& Delta,
        TArray<uint32>& OutWords) const;

    const FCVXVirtualPageStats& GetStats() const { return Stats; }

    static FIntVector ChunkToPageCoord(const FIntVector& ChunkCoord);
    static int32 ChunkToLocalIndex(const FIntVector& ChunkCoord);

private:
    struct FCVXVirtualChunkRecord
    {
        uint32 StateWords[4] = { 0, 0, 0, 0 };
        uint64 MixedPresence = 0;
        TArray<uint64> MixedMasks;
    };

    struct FCVXVirtualPageRecord
    {
        bool bFullyEmpty = false;
        TMap<int32, FCVXVirtualChunkRecord> ExplicitChunks;
    };

    static int32 FloorDiv(int32 Value, int32 Divisor);
    static int32 PositiveMod(int32 Value, int32 Divisor);

    static FCVXVirtualChunkRecord PackChunk(const FCVXSparseVolume& Volume, const FIntVector& ChunkCoord);
    static uint64 PackedChunkBytes(const FCVXVirtualChunkRecord& Chunk);

    void SetOrRemoveChunk(const FCVXSparseVolume& Volume, const FIntVector& ChunkCoord, uint64& InOutDeltaBytes, bool& bOutPageCreated);
    void RecalculateResidentStats();

    TMap<FIntVector, FCVXVirtualPageRecord> Pages;
    FCVXVirtualPageStats Stats;
};
