#include "CVXGPUPageUploadBridge.h"

#include "Containers/Map.h"
#include "Misc/ScopeLock.h"

namespace
{
    constexpr uint32 CVX4Magic = 0x34585643u;
    constexpr uint32 CVX4Version = 2u;
    constexpr uint32 InvalidIndex = 0xffffffffu;

    struct FCVXHashMirrorEntry
    {
        FIntVector Coord = FIntVector::ZeroValue;
        uint32 SlotPlusOne = 0;
    };

    struct FCVXPendingPatch
    {
        uint32 Words[FCVXGPUPageUploadBridge::PatchStrideWords] = {};
    };

    FCriticalSection GMutex;
    uint32 GOwnerId = 0;
    uint32 GResetSerial = 1;
    uint32 GNextPageSlot = 0;
    uint32 GCacheFullEvents = 0;

    uint64 GCoalescedChunkReplacements = 0;
    uint64 GSubmittedDeltaPackets = 0;
    uint64 GSubmittedDeltaBytes = 0;
    uint64 GDrainedPatchBytes = 0;

    TMap<FIntVector, uint32> GPageToSlot;
    TArray<FCVXHashMirrorEntry> GHashMirror;
    TArray<uint32> GSlotToHashIndex;

    // Coalescing unit is one 16 mm chunk. Hundreds of edits to the same chunk
    // within one frame become one GPU patch containing only the latest truth.
    TMap<uint64, FCVXPendingPatch> GPendingChunkPatches;
    TMap<uint32, FCVXPendingPatch> GPendingPageClears;

    uint32 HashPageCoord(const FIntVector& Coord)
    {
        const uint32 X = static_cast<uint32>(Coord.X);
        const uint32 Y = static_cast<uint32>(Coord.Y);
        const uint32 Z = static_cast<uint32>(Coord.Z);

        uint32 H = X * 73856093u;
        H ^= Y * 19349663u;
        H ^= Z * 83492791u;

        // Final avalanche so regular page grids do not produce embarrassing clusters.
        H ^= H >> 16;
        H *= 0x7feb352du;
        H ^= H >> 15;
        H *= 0x846ca68bu;
        H ^= H >> 16;
        return H;
    }

    int32 FloorDiv4(const int32 V)
    {
        if (V >= 0)
        {
            return V / 4;
        }
        return -(((-V) + 3) / 4);
    }

    FIntVector ChunkToPage(const FIntVector& ChunkCoord)
    {
        return FIntVector(
            FloorDiv4(ChunkCoord.X),
            FloorDiv4(ChunkCoord.Y),
            FloorDiv4(ChunkCoord.Z));
    }

    int32 PositiveMod4(const int32 V)
    {
        int32 R = V % 4;
        if (R < 0) R += 4;
        return R;
    }

    uint32 LocalChunkIndex(const FIntVector& ChunkCoord)
    {
        const uint32 X = uint32(PositiveMod4(ChunkCoord.X));
        const uint32 Y = uint32(PositiveMod4(ChunkCoord.Y));
        const uint32 Z = uint32(PositiveMod4(ChunkCoord.Z));
        return X + Y * 4u + Z * 16u;
    }

    void EnsureMirrors_Locked()
    {
        if (GHashMirror.Num() != int32(FCVXGPUPageUploadBridge::HashCapacity))
        {
            GHashMirror.SetNumZeroed(FCVXGPUPageUploadBridge::HashCapacity);
        }

        if (GSlotToHashIndex.Num() != int32(FCVXGPUPageUploadBridge::PageCapacity))
        {
            GSlotToHashIndex.Init(InvalidIndex, FCVXGPUPageUploadBridge::PageCapacity);
        }
    }

    bool GetOrAllocatePage_Locked(
        const FIntVector& PageCoord,
        uint32& OutSlot,
        uint32& OutHashIndex)
    {
        EnsureMirrors_Locked();

        if (const uint32* Existing = GPageToSlot.Find(PageCoord))
        {
            OutSlot = *Existing;
            OutHashIndex = GSlotToHashIndex[OutSlot];
            return OutHashIndex != InvalidIndex;
        }

        if (GNextPageSlot >= FCVXGPUPageUploadBridge::PageCapacity)
        {
            ++GCacheFullEvents;
            return false;
        }

        const uint32 HashMask = FCVXGPUPageUploadBridge::HashCapacity - 1u;
        uint32 Index = HashPageCoord(PageCoord) & HashMask;

        for (uint32 Probe = 0; Probe < FCVXGPUPageUploadBridge::HashCapacity; ++Probe)
        {
            FCVXHashMirrorEntry& Entry = GHashMirror[Index];
            if (Entry.SlotPlusOne == 0u)
            {
                const uint32 Slot = GNextPageSlot++;
                Entry.Coord = PageCoord;
                Entry.SlotPlusOne = Slot + 1u;

                GPageToSlot.Add(PageCoord, Slot);
                GSlotToHashIndex[Slot] = Index;

                OutSlot = Slot;
                OutHashIndex = Index;
                return true;
            }

            if (Entry.Coord == PageCoord)
            {
                OutSlot = Entry.SlotPlusOne - 1u;
                OutHashIndex = Index;
                return true;
            }

            Index = (Index + 1u) & HashMask;
        }

        ++GCacheFullEvents;
        return false;
    }

    void FillCommonPatchHeader(
        FCVXPendingPatch& Patch,
        uint32 Op,
        uint32 Slot,
        uint32 HashIndex,
        uint32 LocalChunk,
        const FIntVector& PageCoord)
    {
        Patch.Words[0] = Op;
        Patch.Words[1] = Slot;
        Patch.Words[2] = HashIndex;
        Patch.Words[3] = LocalChunk;
        Patch.Words[4] = static_cast<uint32>(PageCoord.X);
        Patch.Words[5] = static_cast<uint32>(PageCoord.Y);
        Patch.Words[6] = static_cast<uint32>(PageCoord.Z);
        Patch.Words[7] = 0u;
    }

    void QueueClearPage_Locked(const FIntVector& PageCoord)
    {
        uint32 Slot = 0, HashIndex = 0;
        if (!GetOrAllocatePage_Locked(PageCoord, Slot, HashIndex))
        {
            return;
        }

        // A page tombstone dominates any chunk updates queued earlier this frame.
        for (auto It = GPendingChunkPatches.CreateIterator(); It; ++It)
        {
            const uint32 PatchSlot = uint32(It.Key() >> 6u);
            if (PatchSlot == Slot)
            {
                It.RemoveCurrent();
            }
        }

        FCVXPendingPatch Patch;
        FillCommonPatchHeader(Patch, 1u, Slot, HashIndex, 0u, PageCoord);
        GPendingPageClears.Add(Slot, Patch);
    }

    bool DecodeUpdateChunk_Locked(
        const TArray<uint32>& Words,
        int32& Cursor)
    {
        // Fixed section after opcode:
        // XYZ, state[4], mixedPresence[2], mixedCount
        if (Cursor + 10 > Words.Num())
        {
            return false;
        }

        const FIntVector ChunkCoord(
            static_cast<int32>(Words[Cursor + 0]),
            static_cast<int32>(Words[Cursor + 1]),
            static_cast<int32>(Words[Cursor + 2]));

        const uint32 StateWords[4] =
        {
            Words[Cursor + 3],
            Words[Cursor + 4],
            Words[Cursor + 5],
            Words[Cursor + 6]
        };

        const uint32 MixedCount = Words[Cursor + 9];
        Cursor += 10;

        if (Cursor + int32(MixedCount) * 3 > Words.Num())
        {
            return false;
        }

        const FIntVector PageCoord = ChunkToPage(ChunkCoord);
        uint32 Slot = 0, HashIndex = 0;
        if (!GetOrAllocatePage_Locked(PageCoord, Slot, HashIndex))
        {
            Cursor += int32(MixedCount) * 3;
            return true;
        }

        // If this page is already fully cleared in the pending batch, subtraction-only
        // semantics mean no later chunk update can make it solid again.
        if (GPendingPageClears.Contains(Slot))
        {
            Cursor += int32(MixedCount) * 3;
            return true;
        }

        const uint32 LocalChunk = LocalChunkIndex(ChunkCoord);
        const uint64 PatchKey = (uint64(Slot) << 6u) | uint64(LocalChunk);

        FCVXPendingPatch Patch;
        FillCommonPatchHeader(
            Patch,
            2u,
            Slot,
            HashIndex,
            LocalChunk,
            PageCoord);

        Patch.Words[8] = StateWords[0];
        Patch.Words[9] = StateWords[1];
        Patch.Words[10] = StateWords[2];
        Patch.Words[11] = StateWords[3];

        // Expand to fixed exact masks on the CPU. This deliberately trades a tiny
        // bounded upload for O(1) GPU lookup and removes the variable mask heap.
        for (uint32 Block = 0; Block < 64u; ++Block)
        {
            const uint32 StateWord = StateWords[Block >> 4u];
            const uint32 Shift = (Block & 15u) * 2u;
            const uint32 State = (StateWord >> Shift) & 3u;

            uint32 Low = 0u;
            uint32 High = 0u;

            if (State == 1u || State == 3u) // Full or procedural/full
            {
                Low = 0xffffffffu;
                High = 0xffffffffu;
            }

            Patch.Words[12u + Block * 2u + 0u] = Low;
            Patch.Words[12u + Block * 2u + 1u] = High;
        }

        for (uint32 Mixed = 0; Mixed < MixedCount; ++Mixed)
        {
            const uint32 BlockIndex = Words[Cursor + 0];
            const uint32 Low = Words[Cursor + 1];
            const uint32 High = Words[Cursor + 2];
            Cursor += 3;

            if (BlockIndex < 64u)
            {
                Patch.Words[12u + BlockIndex * 2u + 0u] = Low;
                Patch.Words[12u + BlockIndex * 2u + 1u] = High;
            }
        }

        if (GPendingChunkPatches.Contains(PatchKey))
        {
            ++GCoalescedChunkReplacements;
        }

        GPendingChunkPatches.Add(PatchKey, Patch);
        return true;
    }

    uint32 PendingPatchCount_Locked()
    {
        return uint32(GPendingPageClears.Num() + GPendingChunkPatches.Num());
    }
}

void FCVXGPUPageUploadBridge::ResetPersistentField(uint32 OwnerId)
{
    FScopeLock Lock(&GMutex);

    GOwnerId = OwnerId;
    ++GResetSerial;
    if (GResetSerial == 0u) GResetSerial = 1u;

    GNextPageSlot = 0;
    GCacheFullEvents = 0;
    GPageToSlot.Reset();
    GHashMirror.Reset();
    GSlotToHashIndex.Reset();
    GPendingChunkPatches.Reset();
    GPendingPageClears.Reset();
}

void FCVXGPUPageUploadBridge::SubmitDeltaWords(const TArray<uint32>& Words)
{
    if (Words.Num() < 4)
    {
        return;
    }

    FScopeLock Lock(&GMutex);

    ++GSubmittedDeltaPackets;
    GSubmittedDeltaBytes += uint64(Words.Num()) * sizeof(uint32);

    if (Words[0] != CVX4Magic || Words[1] != CVX4Version)
    {
        // CVX-004B intentionally standardizes on the CVX4 edit stream.
        return;
    }

    const uint32 ClearPageCount = Words[2];
    const uint32 UpdateChunkCount = Words[3];

    int32 Cursor = 4;

    for (uint32 I = 0; I < ClearPageCount; ++I)
    {
        if (Cursor + 4 > Words.Num() || Words[Cursor] != 1u)
        {
            return;
        }

        const FIntVector PageCoord(
            static_cast<int32>(Words[Cursor + 1]),
            static_cast<int32>(Words[Cursor + 2]),
            static_cast<int32>(Words[Cursor + 3]));

        Cursor += 4;
        QueueClearPage_Locked(PageCoord);
    }

    for (uint32 I = 0; I < UpdateChunkCount; ++I)
    {
        if (Cursor >= Words.Num() || Words[Cursor] != 2u)
        {
            return;
        }

        ++Cursor; // consume opcode
        if (!DecodeUpdateChunk_Locked(Words, Cursor))
        {
            return;
        }
    }
}

void FCVXGPUPageUploadBridge::DrainCoalescedPatchWords(
    TArray<uint32>& OutWords,
    uint32& OutPatchCount)
{
    FScopeLock Lock(&GMutex);

    OutWords.Reset();
    OutPatchCount = PendingPatchCount_Locked();

    if (OutPatchCount == 0u)
    {
        return;
    }

    OutWords.Reserve(int32(OutPatchCount * PatchStrideWords));

    for (const TPair<uint32, FCVXPendingPatch>& Pair : GPendingPageClears)
    {
        OutWords.Append(Pair.Value.Words, PatchStrideWords);
    }

    for (const TPair<uint64, FCVXPendingPatch>& Pair : GPendingChunkPatches)
    {
        OutWords.Append(Pair.Value.Words, PatchStrideWords);
    }

    GDrainedPatchBytes += uint64(OutWords.Num()) * sizeof(uint32);

    GPendingPageClears.Reset();
    GPendingChunkPatches.Reset();
}

FCVXGPUPersistentFieldStats FCVXGPUPageUploadBridge::GetPersistentFieldStats()
{
    FScopeLock Lock(&GMutex);

    FCVXGPUPersistentFieldStats Stats;
    Stats.ResetSerial = GResetSerial;
    Stats.AllocatedPages = GNextPageSlot;
    Stats.PageCapacity = PageCapacity;
    Stats.HashCapacity = HashCapacity;
    Stats.PendingPatches = PendingPatchCount_Locked();
    Stats.CacheFullEvents = GCacheFullEvents;
    Stats.CoalescedChunkReplacements = GCoalescedChunkReplacements;
    Stats.SubmittedDeltaPackets = GSubmittedDeltaPackets;
    Stats.SubmittedDeltaBytes = GSubmittedDeltaBytes;
    Stats.DrainedPatchBytes = GDrainedPatchBytes;
    return Stats;
}
