#include "CVXVirtualPageSpine.h"

FString FCVXVirtualPageStats::ToString() const
{
    return FString::Printf(
        TEXT("CVX VIRTUAL PAGES | edge=64mm | pages=%d | emptyPages=%d | explicitChunks=%d | mixedBlocks=%d | resident=%.2f KiB | pageMap=%.2f KiB | chunks=%.2f KiB | masks=%.2f KiB | lastDelta=%llu B | changedPages=%d | changedChunks=%d | totalDelta=%.2f KiB | fullWorldUploads=%llu"),
        ResidentEditedPages,
        FullyEmptyPages,
        ExplicitChunks,
        MixedBlocks,
        double(ResidentPayloadBytes) / 1024.0,
        double(PageMapBytes) / 1024.0,
        double(ChunkRecordBytes) / 1024.0,
        double(MixedMaskBytes) / 1024.0,
        LastDeltaBytes,
        LastChangedPages,
        LastChangedChunks,
        double(TotalDeltaBytes) / 1024.0,
        FullWorldUploadCount);
}

int32 FCVXVirtualPageSpine::FloorDiv(int32 Value, int32 Divisor)
{
    check(Divisor > 0);
    int32 Q = Value / Divisor;
    const int32 R = Value % Divisor;
    if (R < 0)
    {
        --Q;
    }
    return Q;
}

int32 FCVXVirtualPageSpine::PositiveMod(int32 Value, int32 Divisor)
{
    check(Divisor > 0);
    int32 R = Value % Divisor;
    if (R < 0)
    {
        R += Divisor;
    }
    return R;
}

FIntVector FCVXVirtualPageSpine::ChunkToPageCoord(const FIntVector& ChunkCoord)
{
    return FIntVector(
        FloorDiv(ChunkCoord.X, PageEdgeChunks),
        FloorDiv(ChunkCoord.Y, PageEdgeChunks),
        FloorDiv(ChunkCoord.Z, PageEdgeChunks));
}

int32 FCVXVirtualPageSpine::ChunkToLocalIndex(const FIntVector& ChunkCoord)
{
    const int32 X = PositiveMod(ChunkCoord.X, PageEdgeChunks);
    const int32 Y = PositiveMod(ChunkCoord.Y, PageEdgeChunks);
    const int32 Z = PositiveMod(ChunkCoord.Z, PageEdgeChunks);
    return X + Y * PageEdgeChunks + Z * PageEdgeChunks * PageEdgeChunks;
}

void FCVXVirtualPageSpine::Reset()
{
    Pages.Reset();
    Stats = FCVXVirtualPageStats{};
}

FCVXVirtualPageSpine::FCVXVirtualChunkRecord FCVXVirtualPageSpine::PackChunk(
    const FCVXSparseVolume& Volume,
    const FIntVector& ChunkCoord)
{
    FCVXVirtualChunkRecord Result;

    for (int32 BlockIndex = 0; BlockIndex < CVX::BlocksPerChunk; ++BlockIndex)
    {
        const CVX::EBlockState State = Volume.GetBlockStateForVisualization(ChunkCoord, BlockIndex);
        const int32 Word = BlockIndex >> 4;
        const int32 Shift = (BlockIndex & 15) * 2;
        Result.StateWords[Word] |= (static_cast<uint32>(State) & 0x3u) << Shift;

        if (State == CVX::EBlockState::Mixed)
        {
            Result.MixedPresence |= uint64(1) << BlockIndex;
            Result.MixedMasks.Add(Volume.GetBlockMaskForVisualization(ChunkCoord, BlockIndex));
        }
    }

    return Result;
}

uint64 FCVXVirtualPageSpine::PackedChunkBytes(const FCVXVirtualChunkRecord& Chunk)
{
    return ChunkRecordBytes + uint64(Chunk.MixedMasks.Num()) * MixedMaskBytes;
}

void FCVXVirtualPageSpine::SetOrRemoveChunk(
    const FCVXSparseVolume& Volume,
    const FIntVector& ChunkCoord,
    uint64& InOutDeltaBytes,
    bool& bOutPageCreated)
{
    const FIntVector PageCoord = ChunkToPageCoord(ChunkCoord);
    const int32 LocalChunkIndex = ChunkToLocalIndex(ChunkCoord);

    FCVXVirtualPageRecord* ExistingPage = Pages.Find(PageCoord);

    if (ExistingPage && ExistingPage->bFullyEmpty)
    {
        return;
    }

    if (!Volume.IsChunkDirty(ChunkCoord))
    {
        if (!ExistingPage)
        {
            return;
        }

        if (ExistingPage->ExplicitChunks.Remove(LocalChunkIndex) > 0)
        {
            // One chunk-map entry changes back to implicit base material.
            InOutDeltaBytes += sizeof(uint32);
        }

        if (ExistingPage->ExplicitChunks.Num() == 0)
        {
            Pages.Remove(PageCoord);
            // Page table removal / descriptor invalidation. We count the descriptor only;
            // allocator bookkeeping is implementation-specific and intentionally excluded.
            InOutDeltaBytes += PageDescriptorBytes;
        }
        return;
    }

    bOutPageCreated = ExistingPage == nullptr;
    FCVXVirtualPageRecord& Page = Pages.FindOrAdd(PageCoord);
    Page.bFullyEmpty = false;
    if (bOutPageCreated)
    {
        // First residency of a page initializes its fixed descriptor and 64-entry chunk map.
        InOutDeltaBytes += PageDescriptorBytes + PageChunkMapBytes;
    }

    FCVXVirtualChunkRecord Packed = PackChunk(Volume, ChunkCoord);
    const bool bHadChunk = Page.ExplicitChunks.Contains(LocalChunkIndex);
    Page.ExplicitChunks.Add(LocalChunkIndex, Packed);

    // Hot GPU representation can update a chunk record plus only its compact 64-bit mixed masks.
    // If this is the first explicit version of the chunk, also patch one 32-bit page-map entry.
    InOutDeltaBytes += PackedChunkBytes(Packed);
    if (!bHadChunk)
    {
        InOutDeltaBytes += sizeof(uint32);
    }
}

void FCVXVirtualPageSpine::RebuildFromVolume(const FCVXSparseVolume& Volume)
{
    Pages.Reset();

    TArray<FIntVector> DirtyChunks;
    Volume.GetDirtyChunkCoords(DirtyChunks);

    uint64 BootstrapBytes = 0;
    TSet<FIntVector> NewPages;

    TArray<FIntVector> EmptyPages;
    Volume.GetFullyEmptyPageCoords64MM(EmptyPages);
    for (const FIntVector& PageCoord : EmptyPages)
    {
        FCVXVirtualPageRecord& Page = Pages.FindOrAdd(PageCoord);
        Page.bFullyEmpty = true;
        Page.ExplicitChunks.Reset();
        BootstrapBytes += PageDescriptorBytes;
        NewPages.Add(PageCoord);
    }

    for (const FIntVector& ChunkCoord : DirtyChunks)
    {
        bool bPageCreated = false;
        SetOrRemoveChunk(Volume, ChunkCoord, BootstrapBytes, bPageCreated);
        if (bPageCreated)
        {
            NewPages.Add(ChunkToPageCoord(ChunkCoord));
        }
    }

    RecalculateResidentStats();
    Stats.LastChangedPages = NewPages.Num();
    Stats.LastChangedChunks = DirtyChunks.Num();
    Stats.LastDeltaBytes = BootstrapBytes;
    Stats.TotalDeltaBytes += BootstrapBytes;
    // Deliberately leave FullWorldUploadCount at zero: only DirtyChunks were traversed.
}

void FCVXVirtualPageSpine::ApplyChangedBlocks(
    const FCVXSparseVolume& Volume,
    const TArray<FCVXChangedBlock>& ChangedBlocks)
{
    if (ChangedBlocks.Num() == 0)
    {
        Stats.LastChangedPages = 0;
        Stats.LastChangedChunks = 0;
        Stats.LastDeltaBytes = 0;
        return;
    }

    TSet<FIntVector> UniqueChunks;
    UniqueChunks.Reserve(ChangedBlocks.Num());
    for (const FCVXChangedBlock& Changed : ChangedBlocks)
    {
        UniqueChunks.Add(Changed.ChunkCoord);
    }

    TSet<FIntVector> ChangedPages;
    ChangedPages.Reserve(UniqueChunks.Num());

    uint64 DeltaBytes = 0;
    for (const FIntVector& ChunkCoord : UniqueChunks)
    {
        ChangedPages.Add(ChunkToPageCoord(ChunkCoord));
        bool bPageCreated = false;
        SetOrRemoveChunk(Volume, ChunkCoord, DeltaBytes, bPageCreated);
    }

    RecalculateResidentStats();
    Stats.LastChangedPages = ChangedPages.Num();
    Stats.LastChangedChunks = UniqueChunks.Num();
    Stats.LastDeltaBytes = DeltaBytes;
    Stats.TotalDeltaBytes += DeltaBytes;
}


void FCVXVirtualPageSpine::ApplyEditDelta(
    const FCVXSparseVolume& Volume,
    const FCVXEditDelta& Delta)
{
    if (Delta.IsEmpty())
    {
        Stats.LastChangedPages = 0;
        Stats.LastChangedChunks = 0;
        Stats.LastDeltaBytes = 0;
        return;
    }

    TSet<FIntVector> ChangedPages;
    TSet<FIntVector> ChangedChunks;
    uint64 DeltaBytes = 0;

    for (const FIntVector& PageCoord : Delta.ClearedPages64MM)
    {
        FCVXVirtualPageRecord& Page = Pages.FindOrAdd(PageCoord);
        Page.bFullyEmpty = true;
        Page.ExplicitChunks.Reset();
        ChangedPages.Add(PageCoord);
        // CLEAR_PAGE command: opcode + XYZ. One record replaces 262,144 logical voxels.
        DeltaBytes += 16ull;
    }

    for (const FIntVector& ChunkCoord : Delta.ClearedChunks16MM)
    {
        const FIntVector PageCoord = ChunkToPageCoord(ChunkCoord);
        if (Pages.Contains(PageCoord) && Pages.FindChecked(PageCoord).bFullyEmpty)
        {
            continue;
        }

        ChangedPages.Add(PageCoord);
        ChangedChunks.Add(ChunkCoord);
    }

    for (const FCVXChangedBlock& Changed : Delta.ChangedBlocks)
    {
        const FIntVector PageCoord = ChunkToPageCoord(Changed.ChunkCoord);
        if (Pages.Contains(PageCoord) && Pages.FindChecked(PageCoord).bFullyEmpty)
        {
            continue;
        }
        ChangedPages.Add(PageCoord);
        ChangedChunks.Add(Changed.ChunkCoord);
    }

    for (const FIntVector& ChunkCoord : ChangedChunks)
    {
        bool bPageCreated = false;
        SetOrRemoveChunk(Volume, ChunkCoord, DeltaBytes, bPageCreated);
    }

    RecalculateResidentStats();
    Stats.LastChangedPages = ChangedPages.Num();
    Stats.LastChangedChunks = ChangedChunks.Num();
    Stats.LastDeltaBytes = DeltaBytes;
    Stats.TotalDeltaBytes += DeltaBytes;
}

void FCVXVirtualPageSpine::RecalculateResidentStats()
{
    Stats.ResidentEditedPages = Pages.Num();
    Stats.ExplicitChunks = 0;
    Stats.FullyEmptyPages = 0;
    Stats.MixedBlocks = 0;
    Stats.PageMapBytes = 0;
    Stats.ChunkRecordBytes = 0;
    Stats.MixedMaskBytes = 0;

    for (const TPair<FIntVector, FCVXVirtualPageRecord>& PagePair : Pages)
    {
        const FCVXVirtualPageRecord& Page = PagePair.Value;
        if (Page.bFullyEmpty)
        {
            ++Stats.FullyEmptyPages;
            Stats.PageMapBytes += PageDescriptorBytes;
            continue;
        }

        Stats.PageMapBytes += PageDescriptorBytes + PageChunkMapBytes;
        Stats.ExplicitChunks += Page.ExplicitChunks.Num();

        for (const TPair<int32, FCVXVirtualChunkRecord>& ChunkPair : Page.ExplicitChunks)
        {
            const FCVXVirtualChunkRecord& Chunk = ChunkPair.Value;
            Stats.ChunkRecordBytes += ChunkRecordBytes;
            Stats.MixedBlocks += Chunk.MixedMasks.Num();
            Stats.MixedMaskBytes += uint64(Chunk.MixedMasks.Num()) * MixedMaskBytes;
        }
    }

    Stats.ResidentPayloadBytes = Stats.PageMapBytes + Stats.ChunkRecordBytes + Stats.MixedMaskBytes;
}


void FCVXVirtualPageSpine::BuildChangedChunkDeltaWords(
    const FCVXSparseVolume& Volume,
    const TArray<FCVXChangedBlock>& ChangedBlocks,
    TArray<uint32>& OutWords) const
{
    OutWords.Reset();
    if (ChangedBlocks.Num() == 0)
    {
        return;
    }

    TSet<FIntVector> UniqueChunks;
    UniqueChunks.Reserve(ChangedBlocks.Num());
    for (const FCVXChangedBlock& Changed : ChangedBlocks)
    {
        UniqueChunks.Add(Changed.ChunkCoord);
    }

    // Header: 'CVX3', version, unique chunk count, reserved.
    OutWords.Reserve(4 + UniqueChunks.Num() * 12);
    OutWords.Add(0x33585643u); // ASCII CVX3 little-endian
    OutWords.Add(1u);
    OutWords.Add(static_cast<uint32>(UniqueChunks.Num()));
    OutWords.Add(0u);

    for (const FIntVector& ChunkCoord : UniqueChunks)
    {
        uint32 StateWords[4] = { 0u, 0u, 0u, 0u };
        uint64 MixedPresence = 0ull;
        TArray<TPair<int32, uint64>, TInlineAllocator<16>> MixedMasks;

        for (int32 BlockIndex = 0; BlockIndex < CVX::BlocksPerChunk; ++BlockIndex)
        {
            const CVX::EBlockState State = Volume.GetBlockStateForVisualization(ChunkCoord, BlockIndex);
            const int32 Word = BlockIndex >> 4;
            const int32 Shift = (BlockIndex & 15) * 2;
            StateWords[Word] |= (static_cast<uint32>(State) & 0x3u) << Shift;

            if (State == CVX::EBlockState::Mixed)
            {
                MixedPresence |= uint64(1) << BlockIndex;
                MixedMasks.Emplace(BlockIndex, Volume.GetBlockMaskForVisualization(ChunkCoord, BlockIndex));
            }
        }

        OutWords.Add(static_cast<uint32>(ChunkCoord.X));
        OutWords.Add(static_cast<uint32>(ChunkCoord.Y));
        OutWords.Add(static_cast<uint32>(ChunkCoord.Z));
        OutWords.Add(StateWords[0]);
        OutWords.Add(StateWords[1]);
        OutWords.Add(StateWords[2]);
        OutWords.Add(StateWords[3]);
        OutWords.Add(static_cast<uint32>(MixedPresence & 0xffffffffull));
        OutWords.Add(static_cast<uint32>(MixedPresence >> 32));
        OutWords.Add(static_cast<uint32>(MixedMasks.Num()));

        for (const TPair<int32, uint64>& Pair : MixedMasks)
        {
            OutWords.Add(static_cast<uint32>(Pair.Key));
            OutWords.Add(static_cast<uint32>(Pair.Value & 0xffffffffull));
            OutWords.Add(static_cast<uint32>(Pair.Value >> 32));
        }
    }
}

void FCVXVirtualPageSpine::BuildEditDeltaWords(
    const FCVXSparseVolume& Volume,
    const FCVXEditDelta& Delta,
    TArray<uint32>& OutWords) const
{
    OutWords.Reset();
    if (Delta.IsEmpty())
    {
        return;
    }

    TSet<FIntVector> ClearedPageSet;
    ClearedPageSet.Reserve(Delta.ClearedPages64MM.Num());
    for (const FIntVector& PageCoord : Delta.ClearedPages64MM)
    {
        ClearedPageSet.Add(PageCoord);
    }

    TSet<FIntVector> UniqueChunks;
    UniqueChunks.Reserve(Delta.ClearedChunks16MM.Num() + Delta.ChangedBlocks.Num());
    for (const FIntVector& ChunkCoord : Delta.ClearedChunks16MM)
    {
        if (!ClearedPageSet.Contains(ChunkToPageCoord(ChunkCoord)))
        {
            UniqueChunks.Add(ChunkCoord);
        }
    }
    for (const FCVXChangedBlock& Changed : Delta.ChangedBlocks)
    {
        if (!ClearedPageSet.Contains(ChunkToPageCoord(Changed.ChunkCoord)))
        {
            UniqueChunks.Add(Changed.ChunkCoord);
        }
    }

    // Header: 'CVX4', version=2, clear-page count, update-chunk count.
    OutWords.Add(0x34585643u); // ASCII CVX4 little-endian
    OutWords.Add(2u);
    OutWords.Add(static_cast<uint32>(Delta.ClearedPages64MM.Num()));
    OutWords.Add(static_cast<uint32>(UniqueChunks.Num()));

    // CLEAR_PAGE command = opcode + XYZ = 16 bytes for 64^3 logical cells.
    for (const FIntVector& PageCoord : Delta.ClearedPages64MM)
    {
        OutWords.Add(1u);
        OutWords.Add(static_cast<uint32>(PageCoord.X));
        OutWords.Add(static_cast<uint32>(PageCoord.Y));
        OutWords.Add(static_cast<uint32>(PageCoord.Z));
    }

    for (const FIntVector& ChunkCoord : UniqueChunks)
    {
        uint32 StateWords[4] = { 0u, 0u, 0u, 0u };
        uint64 MixedPresence = 0ull;
        TArray<TPair<int32, uint64>, TInlineAllocator<16>> MixedMasks;

        for (int32 BlockIndex = 0; BlockIndex < CVX::BlocksPerChunk; ++BlockIndex)
        {
            const CVX::EBlockState State = Volume.GetBlockStateForVisualization(ChunkCoord, BlockIndex);
            const int32 Word = BlockIndex >> 4;
            const int32 Shift = (BlockIndex & 15) * 2;
            StateWords[Word] |= (static_cast<uint32>(State) & 0x3u) << Shift;

            if (State == CVX::EBlockState::Mixed)
            {
                MixedPresence |= uint64(1) << BlockIndex;
                MixedMasks.Emplace(BlockIndex, Volume.GetBlockMaskForVisualization(ChunkCoord, BlockIndex));
            }
        }

        OutWords.Add(2u); // UPDATE_CHUNK
        OutWords.Add(static_cast<uint32>(ChunkCoord.X));
        OutWords.Add(static_cast<uint32>(ChunkCoord.Y));
        OutWords.Add(static_cast<uint32>(ChunkCoord.Z));
        OutWords.Add(StateWords[0]);
        OutWords.Add(StateWords[1]);
        OutWords.Add(StateWords[2]);
        OutWords.Add(StateWords[3]);
        OutWords.Add(static_cast<uint32>(MixedPresence & 0xffffffffull));
        OutWords.Add(static_cast<uint32>(MixedPresence >> 32));
        OutWords.Add(static_cast<uint32>(MixedMasks.Num()));

        for (const TPair<int32, uint64>& Pair : MixedMasks)
        {
            OutWords.Add(static_cast<uint32>(Pair.Key));
            OutWords.Add(static_cast<uint32>(Pair.Value & 0xffffffffull));
            OutWords.Add(static_cast<uint32>(Pair.Value >> 32));
        }
    }
}

