#include "CVXSparseVolume.h"

using namespace CVX;

FString FCVXVolumeStats::ToString() const
{
    const double DenseMiB = double(Dense16BitBytes) / (1024.0 * 1024.0);
    const double CompactKiB = double(CompactGPUBytes) / 1024.0;
    const double Ratio = CompactGPUBytes > 0 ? double(Dense16BitBytes) / double(CompactGPUBytes) : 0.0;

    return FString::Printf(
        TEXT("CVX logical=%lld voxels | dirtyChunks=%d | mixedBlocks=%d | emptyBlocks=%d | hotGPU=%.2f KiB | dense16=%.2f MiB | dense/hot=%.1fx"),
        LogicalVoxelCount,
        DirtyChunkCount,
        MixedBlockCount,
        EmptyBlockCount,
        CompactKiB,
        DenseMiB,
        Ratio);
}

FString FCVXBulkEditStats::ToString() const
{
    return FString::Printf(
        TEXT("CVX BULK EDIT | pages=%d cleared=%d | chunks=%d cleared=%d | blocks=%d cleared=%d boundary=%d | sub2mmCleared=%d | fineTests=%lld | clearedBits=%lld"),
        PagesVisited,
        PagesCleared,
        ChunksVisited,
        ChunksCleared,
        BlocksVisited,
        BlocksCleared,
        BoundaryBlocks,
        SubBlocks2mmCleared,
        FineVoxelTests,
        VoxelBitsCleared);
}

FCVXSparseVolume::FCVXSparseVolume()
{
    Reset(FIntVector(1000, 1000, 1000), 1);
}

void FCVXSparseVolume::Reset(const FIntVector& InLogicalSizeMM, uint16 InDefaultMaterial)
{
    LogicalSizeMM.X = FMath::Max(1, InLogicalSizeMM.X);
    LogicalSizeMM.Y = FMath::Max(1, InLogicalSizeMM.Y);
    LogicalSizeMM.Z = FMath::Max(1, InLogicalSizeMM.Z);
    DefaultMaterial = InDefaultMaterial;
    FullyEmptyPages64MM.Reset();
    FullyEmptyChunks16MM.Reset();
    DirtyChunks.Reset();
    ++Generation;
}

uint32 FCVXSparseVolume::PopCount64(uint64 Value)
{
    uint32 Count = 0;
    while (Value)
    {
        Value &= Value - 1;
        ++Count;
    }
    return Count;
}

EBlockState FCVXSparseVolume::FCVXChunk::GetState(int32 BlockIndex) const
{
    check(BlockIndex >= 0 && BlockIndex < BlocksPerChunk);
    const int32 Word = BlockIndex >> 4; // 16 states per uint32
    const int32 Shift = (BlockIndex & 15) * 2;
    return static_cast<EBlockState>((StateWords[Word] >> Shift) & 0x3u);
}

int32 FCVXSparseVolume::FCVXChunk::MixedArrayIndex(int32 BlockIndex) const
{
    const uint64 LowerMask = BlockIndex == 0 ? 0ull : ((uint64(1) << BlockIndex) - 1ull);
    return static_cast<int32>(FCVXSparseVolume::PopCount64(MixedPresence & LowerMask));
}

void FCVXSparseVolume::FCVXChunk::SetState(int32 BlockIndex, EBlockState NewState)
{
    check(BlockIndex >= 0 && BlockIndex < BlocksPerChunk);

    const EBlockState OldState = GetState(BlockIndex);
    if (OldState == EBlockState::Mixed && NewState != EBlockState::Mixed)
    {
        const uint64 Bit = uint64(1) << BlockIndex;
        if (MixedPresence & Bit)
        {
            const int32 Index = MixedArrayIndex(BlockIndex);
            MixedMasks.RemoveAt(Index, 1, EAllowShrinking::No);
            MixedPresence &= ~Bit;
        }
    }

    const int32 Word = BlockIndex >> 4;
    const int32 Shift = (BlockIndex & 15) * 2;
    const uint32 Mask = 0x3u << Shift;
    StateWords[Word] = (StateWords[Word] & ~Mask) | (static_cast<uint32>(NewState) << Shift);
}

uint64 FCVXSparseVolume::FCVXChunk::GetMask(int32 BlockIndex) const
{
    switch (GetState(BlockIndex))
    {
        case EBlockState::Empty:
            return 0ull;
        case EBlockState::Full:
        case EBlockState::Procedural:
            return FullBlockMask;
        case EBlockState::Mixed:
        {
            const uint64 Bit = uint64(1) << BlockIndex;
            check(MixedPresence & Bit);
            return MixedMasks[MixedArrayIndex(BlockIndex)];
        }
        default:
            return FullBlockMask;
    }
}

void FCVXSparseVolume::FCVXChunk::SetMaskAndCompact(int32 BlockIndex, uint64 NewMask)
{
    if (NewMask == 0ull)
    {
        SetState(BlockIndex, EBlockState::Empty);
        return;
    }

    if (NewMask == FullBlockMask)
    {
        SetState(BlockIndex, EBlockState::Full);
        return;
    }

    const uint64 Bit = uint64(1) << BlockIndex;
    if ((MixedPresence & Bit) == 0)
    {
        const int32 InsertIndex = MixedArrayIndex(BlockIndex);
        MixedMasks.Insert(NewMask, InsertIndex);
        MixedPresence |= Bit;
    }
    else
    {
        MixedMasks[MixedArrayIndex(BlockIndex)] = NewMask;
    }

    const int32 Word = BlockIndex >> 4;
    const int32 Shift = (BlockIndex & 15) * 2;
    const uint32 StateMask = 0x3u << Shift;
    StateWords[Word] = (StateWords[Word] & ~StateMask) | (static_cast<uint32>(EBlockState::Mixed) << Shift);
}

bool FCVXSparseVolume::FCVXChunk::IsImplicitFull() const
{
    return MixedPresence == 0ull &&
        StateWords[0] == 0x55555555u &&
        StateWords[1] == 0x55555555u &&
        StateWords[2] == 0x55555555u &&
        StateWords[3] == 0x55555555u;
}

bool FCVXSparseVolume::FCVXChunk::IsCompletelyEmpty() const
{
    return MixedPresence == 0ull
        && StateWords[0] == 0u
        && StateWords[1] == 0u
        && StateWords[2] == 0u
        && StateWords[3] == 0u;
}

double FCVXSparseVolume::MinDistanceSquaredToAABB(const FVector3d& Point, const FVector3d& Min, const FVector3d& Max)
{
    double Result = 0.0;
    for (int32 Axis = 0; Axis < 3; ++Axis)
    {
        double D = 0.0;
        if (Point[Axis] < Min[Axis]) D = Min[Axis] - Point[Axis];
        else if (Point[Axis] > Max[Axis]) D = Point[Axis] - Max[Axis];
        Result += D * D;
    }
    return Result;
}

double FCVXSparseVolume::MaxDistanceSquaredToAABB(const FVector3d& Point, const FVector3d& Min, const FVector3d& Max)
{
    double Result = 0.0;
    for (int32 Axis = 0; Axis < 3; ++Axis)
    {
        const double D0 = FMath::Abs(Point[Axis] - Min[Axis]);
        const double D1 = FMath::Abs(Point[Axis] - Max[Axis]);
        const double D = FMath::Max(D0, D1);
        Result += D * D;
    }
    return Result;
}

FCVXSparseVolume::FCVXChunk& FCVXSparseVolume::FindOrAddChunk(const FIntVector& ChunkCoord)
{
    return DirtyChunks.FindOrAdd(ChunkCoord);
}

const FCVXSparseVolume::FCVXChunk* FCVXSparseVolume::FindChunk(const FIntVector& ChunkCoord) const
{
    return DirtyChunks.Find(ChunkCoord);
}

FIntVector FCVXSparseVolume::VoxelToChunk(const FIntVector& VoxelMM)
{
    return FIntVector(
        VoxelMM.X / ChunkEdgeVoxels,
        VoxelMM.Y / ChunkEdgeVoxels,
        VoxelMM.Z / ChunkEdgeVoxels);
}

FIntVector FCVXSparseVolume::VoxelToBulkPage64MM(const FIntVector& VoxelMM)
{
    return FIntVector(
        VoxelMM.X / BulkPageEdgeVoxels,
        VoxelMM.Y / BulkPageEdgeVoxels,
        VoxelMM.Z / BulkPageEdgeVoxels);
}

FIntVector FCVXSparseVolume::ChunkToBulkPage64MM(const FIntVector& ChunkCoord)
{
    return FIntVector(
        ChunkCoord.X / BulkPageEdgeChunks,
        ChunkCoord.Y / BulkPageEdgeChunks,
        ChunkCoord.Z / BulkPageEdgeChunks);
}

FIntVector FCVXSparseVolume::VoxelToLocalBlock(const FIntVector& VoxelMM)
{
    return FIntVector(
        (VoxelMM.X / BlockEdgeVoxels) & (ChunkEdgeBlocks - 1),
        (VoxelMM.Y / BlockEdgeVoxels) & (ChunkEdgeBlocks - 1),
        (VoxelMM.Z / BlockEdgeVoxels) & (ChunkEdgeBlocks - 1));
}

FIntVector FCVXSparseVolume::VoxelToLocalInBlock(const FIntVector& VoxelMM)
{
    return FIntVector(
        VoxelMM.X & (BlockEdgeVoxels - 1),
        VoxelMM.Y & (BlockEdgeVoxels - 1),
        VoxelMM.Z & (BlockEdgeVoxels - 1));
}

bool FCVXSparseVolume::IsSolid(const FIntVector& VoxelMM) const
{
    if (VoxelMM.X < 0 || VoxelMM.Y < 0 || VoxelMM.Z < 0 ||
        VoxelMM.X >= LogicalSizeMM.X || VoxelMM.Y >= LogicalSizeMM.Y || VoxelMM.Z >= LogicalSizeMM.Z)
    {
        return false;
    }

    if (FullyEmptyPages64MM.Contains(VoxelToBulkPage64MM(VoxelMM)))
    {
        return false;
    }

    const FIntVector ChunkCoord = VoxelToChunk(VoxelMM);
    if (FullyEmptyChunks16MM.Contains(ChunkCoord))
    {
        return false;
    }

    const FCVXChunk* Chunk = FindChunk(ChunkCoord);
    if (!Chunk)
    {
        return true; // implicit solid procedural base volume
    }

    const FIntVector LocalBlock = VoxelToLocalBlock(VoxelMM);
    const int32 BlockIndex = CVX::LocalBlockIndex(LocalBlock.X, LocalBlock.Y, LocalBlock.Z);
    const EBlockState State = Chunk->GetState(BlockIndex);

    if (State == EBlockState::Empty) return false;
    if (State == EBlockState::Full || State == EBlockState::Procedural) return true;

    const FIntVector LocalVoxel = VoxelToLocalInBlock(VoxelMM);
    const int32 Bit = CVX::LocalVoxelBit(LocalVoxel.X, LocalVoxel.Y, LocalVoxel.Z);
    return (Chunk->GetMask(BlockIndex) & (uint64(1) << Bit)) != 0ull;
}

bool FCVXSparseVolume::IsChunkCompletelyEmpty(const FIntVector& ChunkCoord) const
{
    if (FullyEmptyPages64MM.Contains(ChunkToBulkPage64MM(ChunkCoord))
        || FullyEmptyChunks16MM.Contains(ChunkCoord))
    {
        return true;
    }

    const FCVXChunk* Chunk = FindChunk(ChunkCoord);
    if (!Chunk)
    {
        return false; // absent chunks are implicit full material
    }

    return Chunk->IsCompletelyEmpty();
}

double FCVXSparseVolume::DistanceToNextGridBoundaryMM(
    const FVector3d& PositionMM,
    const FVector3d& Direction,
    double CellSizeMM)
{
    double Best = TNumericLimits<double>::Max();
    constexpr double AxisEpsilon = 1.0e-12;

    for (int32 Axis = 0; Axis < 3; ++Axis)
    {
        const double D = Direction[Axis];
        if (FMath::Abs(D) <= AxisEpsilon)
        {
            continue;
        }

        const double Cell = FMath::Floor(PositionMM[Axis] / CellSizeMM);
        double Boundary = 0.0;
        if (D > 0.0)
        {
            Boundary = (Cell + 1.0) * CellSizeMM;
        }
        else
        {
            Boundary = Cell * CellSizeMM;
            if (FMath::IsNearlyEqual(PositionMM[Axis], Boundary, 1.0e-9))
            {
                Boundary -= CellSizeMM;
            }
        }

        const double T = (Boundary - PositionMM[Axis]) / D;
        if (T > AxisEpsilon)
        {
            Best = FMath::Min(Best, T);
        }
    }

    return Best;
}

bool FCVXSparseVolume::FindFirstSolidAlongSegmentMM(
    const FVector3d& StartMM,
    const FVector3d& EndMM,
    FIntVector& OutVoxelMM,
    FCVXRayTraversalStats& OutStats) const
{
    OutStats = FCVXRayTraversalStats{};

    const FVector3d Delta = EndMM - StartMM;
    const double SegmentLength = Delta.Size();
    if (SegmentLength <= 1.0e-9)
    {
        return false;
    }

    const FVector3d Direction = Delta / SegmentLength;
    constexpr double AdvanceEpsilonMM = 0.001;
    FVector3d Position = StartMM + Direction * AdvanceEpsilonMM;
    double Travelled = AdvanceEpsilonMM;

    // This is deliberately a safety bound rather than the normal work amount. Empty space is
    // skipped in 16/4 mm strides, so a long tunnel should not approach this in ordinary use.
    const int32 MaxIterations = 131072;

    for (int32 Iteration = 0; Iteration < MaxIterations && Travelled <= SegmentLength; ++Iteration)
    {
        ++OutStats.Samples;

        const FIntVector VoxelMM(
            FMath::FloorToInt(Position.X),
            FMath::FloorToInt(Position.Y),
            FMath::FloorToInt(Position.Z));

        if (VoxelMM.X < 0 || VoxelMM.Y < 0 || VoxelMM.Z < 0
            || VoxelMM.X >= LogicalSizeMM.X || VoxelMM.Y >= LogicalSizeMM.Y || VoxelMM.Z >= LogicalSizeMM.Z)
        {
            return false;
        }

        if (IsSolid(VoxelMM))
        {
            OutVoxelMM = VoxelMM;
            return true;
        }

        const FIntVector ChunkCoord = VoxelToChunk(VoxelMM);
        double StepMM = 1.0;

        if (IsChunkCompletelyEmpty(ChunkCoord))
        {
            StepMM = DistanceToNextGridBoundaryMM(Position, Direction, double(ChunkEdgeVoxels));
            ++OutStats.ChunkSkips16mm;
        }
        else
        {
            const FIntVector LocalBlock = VoxelToLocalBlock(VoxelMM);
            const int32 BlockIndex = CVX::LocalBlockIndex(LocalBlock.X, LocalBlock.Y, LocalBlock.Z);
            const EBlockState State = GetBlockStateForVisualization(ChunkCoord, BlockIndex);

            if (State == EBlockState::Empty)
            {
                StepMM = DistanceToNextGridBoundaryMM(Position, Direction, double(BlockEdgeVoxels));
                ++OutStats.BlockSkips4mm;
            }
            else
            {
                // Only Mixed cells should reach this branch after IsSolid() returned false.
                // Walk exactly to the next one-millimetre voxel boundary.
                StepMM = DistanceToNextGridBoundaryMM(Position, Direction, 1.0);
                ++OutStats.FineSteps1mm;
            }
        }

        if (!FMath::IsFinite(StepMM) || StepMM <= 0.0)
        {
            StepMM = 1.0;
        }

        StepMM += AdvanceEpsilonMM;
        Position += Direction * StepMM;
        Travelled += StepMM;
    }

    return false;
}

bool FCVXSparseVolume::CarveVoxel(const FIntVector& VoxelMM)
{
    if (VoxelMM.X < 0 || VoxelMM.Y < 0 || VoxelMM.Z < 0 ||
        VoxelMM.X >= LogicalSizeMM.X || VoxelMM.Y >= LogicalSizeMM.Y || VoxelMM.Z >= LogicalSizeMM.Z)
    {
        return false;
    }

    if (FullyEmptyPages64MM.Contains(VoxelToBulkPage64MM(VoxelMM)))
    {
        return false;
    }

    const FIntVector ChunkCoord = VoxelToChunk(VoxelMM);
    if (FullyEmptyChunks16MM.Contains(ChunkCoord))
    {
        return false;
    }

    FCVXChunk& Chunk = FindOrAddChunk(ChunkCoord);

    const FIntVector LocalBlock = VoxelToLocalBlock(VoxelMM);
    const int32 BlockIndex = CVX::LocalBlockIndex(LocalBlock.X, LocalBlock.Y, LocalBlock.Z);
    uint64 Mask = Chunk.GetMask(BlockIndex);

    const FIntVector LocalVoxel = VoxelToLocalInBlock(VoxelMM);
    const int32 Bit = CVX::LocalVoxelBit(LocalVoxel.X, LocalVoxel.Y, LocalVoxel.Z);
    const uint64 VoxelBit = uint64(1) << Bit;

    // Exact edit idempotence: carving an already-empty cell is a no-op.
    if ((Mask & VoxelBit) == 0ull)
    {
        return false;
    }

    Mask &= ~VoxelBit;
    Chunk.SetMaskAndCompact(BlockIndex, Mask);
    ++Generation;
    return true;
}

void FCVXSparseVolume::CarveSphere(const FVector3d& CenterMM, double RadiusMM)
{
    TArray<FCVXChangedBlock> IgnoredChangedBlocks;
    CarveSphere(CenterMM, RadiusMM, IgnoredChangedBlocks);
}

void FCVXSparseVolume::CarveSphere(
    const FVector3d& CenterMM,
    double RadiusMM,
    TArray<FIntVector>& OutChangedChunks)
{
    TArray<FCVXChangedBlock> ChangedBlocks;
    CarveSphere(CenterMM, RadiusMM, ChangedBlocks);

    OutChangedChunks.Reset();
    TSet<FIntVector> UniqueChunks;
    UniqueChunks.Reserve(ChangedBlocks.Num());
    for (const FCVXChangedBlock& Changed : ChangedBlocks)
    {
        UniqueChunks.Add(Changed.ChunkCoord);
    }

    OutChangedChunks.Reserve(UniqueChunks.Num());
    for (const FIntVector& Coord : UniqueChunks)
    {
        OutChangedChunks.Add(Coord);
    }
}

void FCVXSparseVolume::CarveSphere(
    const FVector3d& CenterMM,
    double RadiusMM,
    TArray<FCVXChangedBlock>& OutChangedBlocks)
{
    OutChangedBlocks.Reset();

    if (RadiusMM <= 0.0)
    {
        return;
    }

    const double RadiusSq = RadiusMM * RadiusMM;
    const FIntVector MinVoxel(
        FMath::Clamp(FMath::FloorToInt(CenterMM.X - RadiusMM), 0, LogicalSizeMM.X - 1),
        FMath::Clamp(FMath::FloorToInt(CenterMM.Y - RadiusMM), 0, LogicalSizeMM.Y - 1),
        FMath::Clamp(FMath::FloorToInt(CenterMM.Z - RadiusMM), 0, LogicalSizeMM.Z - 1));
    const FIntVector MaxVoxel(
        FMath::Clamp(FMath::CeilToInt(CenterMM.X + RadiusMM), 0, LogicalSizeMM.X - 1),
        FMath::Clamp(FMath::CeilToInt(CenterMM.Y + RadiusMM), 0, LogicalSizeMM.Y - 1),
        FMath::Clamp(FMath::CeilToInt(CenterMM.Z + RadiusMM), 0, LogicalSizeMM.Z - 1));

    const FIntVector MinBlock(MinVoxel.X / BlockEdgeVoxels, MinVoxel.Y / BlockEdgeVoxels, MinVoxel.Z / BlockEdgeVoxels);
    const FIntVector MaxBlock(MaxVoxel.X / BlockEdgeVoxels, MaxVoxel.Y / BlockEdgeVoxels, MaxVoxel.Z / BlockEdgeVoxels);

    TSet<FIntVector> ChangedChunks;

    for (int32 BZ = MinBlock.Z; BZ <= MaxBlock.Z; ++BZ)
    {
        for (int32 BY = MinBlock.Y; BY <= MaxBlock.Y; ++BY)
        {
            for (int32 BX = MinBlock.X; BX <= MaxBlock.X; ++BX)
            {
                const FVector3d BlockMin(double(BX * BlockEdgeVoxels), double(BY * BlockEdgeVoxels), double(BZ * BlockEdgeVoxels));
                const FVector3d BlockMax = BlockMin + FVector3d(BlockEdgeVoxels, BlockEdgeVoxels, BlockEdgeVoxels);

                if (MinDistanceSquaredToAABB(CenterMM, BlockMin, BlockMax) > RadiusSq)
                {
                    continue;
                }

                const FIntVector ChunkCoord(BX / ChunkEdgeBlocks, BY / ChunkEdgeBlocks, BZ / ChunkEdgeBlocks);
                const int32 LBX = BX & (ChunkEdgeBlocks - 1);
                const int32 LBY = BY & (ChunkEdgeBlocks - 1);
                const int32 LBZ = BZ & (ChunkEdgeBlocks - 1);
                const int32 BlockIndex = CVX::LocalBlockIndex(LBX, LBY, LBZ);

                FCVXChunk& Chunk = FindOrAddChunk(ChunkCoord);
                const uint64 OldMask = Chunk.GetMask(BlockIndex);

                if (OldMask == 0ull)
                {
                    continue;
                }

                if (MaxDistanceSquaredToAABB(CenterMM, BlockMin, BlockMax) <= RadiusSq)
                {
                    Chunk.SetMaskAndCompact(BlockIndex, 0ull);
                    ChangedChunks.Add(ChunkCoord);
                    OutChangedBlocks.Emplace(ChunkCoord, BlockIndex);
                    continue;
                }

                uint64 NewMask = OldMask;

                for (int32 VZ = 0; VZ < BlockEdgeVoxels; ++VZ)
                {
                    for (int32 VY = 0; VY < BlockEdgeVoxels; ++VY)
                    {
                        for (int32 VX = 0; VX < BlockEdgeVoxels; ++VX)
                        {
                            const FIntVector GlobalVoxel(
                                BX * BlockEdgeVoxels + VX,
                                BY * BlockEdgeVoxels + VY,
                                BZ * BlockEdgeVoxels + VZ);

                            if (GlobalVoxel.X >= LogicalSizeMM.X || GlobalVoxel.Y >= LogicalSizeMM.Y || GlobalVoxel.Z >= LogicalSizeMM.Z)
                            {
                                continue;
                            }

                            const FVector3d VoxelCenter(
                                double(GlobalVoxel.X) + 0.5,
                                double(GlobalVoxel.Y) + 0.5,
                                double(GlobalVoxel.Z) + 0.5);

                            if ((VoxelCenter - CenterMM).SizeSquared() <= RadiusSq)
                            {
                                const int32 Bit = CVX::LocalVoxelBit(VX, VY, VZ);
                                NewMask &= ~(uint64(1) << Bit);
                            }
                        }
                    }
                }

                if (NewMask != OldMask)
                {
                    Chunk.SetMaskAndCompact(BlockIndex, NewMask);
                    ChangedChunks.Add(ChunkCoord);
                    OutChangedBlocks.Emplace(ChunkCoord, BlockIndex);
                }
            }
        }
    }

    for (const FIntVector& Coord : ChangedChunks)
    {
        if (FCVXChunk* Chunk = DirtyChunks.Find(Coord); Chunk && Chunk->IsImplicitFull())
        {
            DirtyChunks.Remove(Coord);
        }
    }

    if (OutChangedBlocks.Num() > 0)
    {
        ++Generation;
    }
}


void FCVXSparseVolume::CarveSphereHierarchical(
    const FVector3d& CenterMM,
    double RadiusMM,
    FCVXEditDelta& OutDelta,
    FCVXBulkEditStats& OutStats)
{
    OutDelta.Reset();
    OutStats = FCVXBulkEditStats{};

    if (RadiusMM <= 0.0)
    {
        return;
    }

    const double RadiusSq = RadiusMM * RadiusMM;
    const FIntVector MinVoxel(
        FMath::Clamp(FMath::FloorToInt(CenterMM.X - RadiusMM), 0, LogicalSizeMM.X - 1),
        FMath::Clamp(FMath::FloorToInt(CenterMM.Y - RadiusMM), 0, LogicalSizeMM.Y - 1),
        FMath::Clamp(FMath::FloorToInt(CenterMM.Z - RadiusMM), 0, LogicalSizeMM.Z - 1));
    const FIntVector MaxVoxel(
        FMath::Clamp(FMath::CeilToInt(CenterMM.X + RadiusMM), 0, LogicalSizeMM.X - 1),
        FMath::Clamp(FMath::CeilToInt(CenterMM.Y + RadiusMM), 0, LogicalSizeMM.Y - 1),
        FMath::Clamp(FMath::CeilToInt(CenterMM.Z + RadiusMM), 0, LogicalSizeMM.Z - 1));

    const FIntVector MinPage = VoxelToBulkPage64MM(MinVoxel);
    const FIntVector MaxPage = VoxelToBulkPage64MM(MaxVoxel);
    bool bAnyChange = false;

    auto ClearWholeChunk = [this, &OutDelta, &OutStats, &bAnyChange](const FIntVector& ChunkCoord)
    {
        if (FullyEmptyPages64MM.Contains(ChunkToBulkPage64MM(ChunkCoord)))
        {
            return;
        }

        if (FullyEmptyChunks16MM.Contains(ChunkCoord))
        {
            return;
        }

        if (FCVXChunk* Existing = DirtyChunks.Find(ChunkCoord))
        {
            if (Existing->IsCompletelyEmpty())
            {
                DirtyChunks.Remove(ChunkCoord);
                FullyEmptyChunks16MM.Add(ChunkCoord);
                return;
            }
        }

        DirtyChunks.Remove(ChunkCoord);
        FullyEmptyChunks16MM.Add(ChunkCoord);
        OutDelta.ClearedChunks16MM.Add(ChunkCoord);
        ++OutStats.ChunksCleared;
        bAnyChange = true;
    };

    for (int32 PZ = MinPage.Z; PZ <= MaxPage.Z; ++PZ)
    {
        for (int32 PY = MinPage.Y; PY <= MaxPage.Y; ++PY)
        {
            for (int32 PX = MinPage.X; PX <= MaxPage.X; ++PX)
            {
                const FIntVector PageCoord(PX, PY, PZ);
                ++OutStats.PagesVisited;

                if (FullyEmptyPages64MM.Contains(PageCoord))
                {
                    continue;
                }

                const FVector3d PageMin(
                    double(PX * BulkPageEdgeVoxels),
                    double(PY * BulkPageEdgeVoxels),
                    double(PZ * BulkPageEdgeVoxels));
                const FVector3d PageMax = PageMin + FVector3d(
                    BulkPageEdgeVoxels,
                    BulkPageEdgeVoxels,
                    BulkPageEdgeVoxels);

                if (MinDistanceSquaredToAABB(CenterMM, PageMin, PageMax) > RadiusSq)
                {
                    continue;
                }

                if (MaxDistanceSquaredToAABB(CenterMM, PageMin, PageMax) <= RadiusSq)
                {
                    // One 64 mm tombstone replaces 64 empty chunks / 4096 empty block states.
                    const FIntVector ChunkOrigin = PageCoord * BulkPageEdgeChunks;
                    for (int32 CZ = 0; CZ < BulkPageEdgeChunks; ++CZ)
                    {
                        for (int32 CY = 0; CY < BulkPageEdgeChunks; ++CY)
                        {
                            for (int32 CX = 0; CX < BulkPageEdgeChunks; ++CX)
                            {
                                const FIntVector ClearedChunk = ChunkOrigin + FIntVector(CX, CY, CZ);
                                DirtyChunks.Remove(ClearedChunk);
                                FullyEmptyChunks16MM.Remove(ClearedChunk);
                            }
                        }
                    }

                    FullyEmptyPages64MM.Add(PageCoord);
                    OutDelta.ClearedPages64MM.Add(PageCoord);
                    ++OutStats.PagesCleared;
                    bAnyChange = true;
                    continue;
                }

                const FIntVector ChunkOrigin = PageCoord * BulkPageEdgeChunks;
                for (int32 LCZ = 0; LCZ < BulkPageEdgeChunks; ++LCZ)
                {
                    for (int32 LCY = 0; LCY < BulkPageEdgeChunks; ++LCY)
                    {
                        for (int32 LCX = 0; LCX < BulkPageEdgeChunks; ++LCX)
                        {
                            const FIntVector ChunkCoord = ChunkOrigin + FIntVector(LCX, LCY, LCZ);
                            const FIntVector ChunkMinInt = ChunkCoord * ChunkEdgeVoxels;
                            if (ChunkMinInt.X >= LogicalSizeMM.X
                                || ChunkMinInt.Y >= LogicalSizeMM.Y
                                || ChunkMinInt.Z >= LogicalSizeMM.Z)
                            {
                                continue;
                            }

                            ++OutStats.ChunksVisited;
                            if (FullyEmptyChunks16MM.Contains(ChunkCoord))
                            {
                                continue;
                            }

                            const FVector3d ChunkMin(double(ChunkMinInt.X), double(ChunkMinInt.Y), double(ChunkMinInt.Z));
                            const FVector3d ChunkMax = ChunkMin + FVector3d(ChunkEdgeVoxels, ChunkEdgeVoxels, ChunkEdgeVoxels);

                            if (MinDistanceSquaredToAABB(CenterMM, ChunkMin, ChunkMax) > RadiusSq)
                            {
                                continue;
                            }

                            if (MaxDistanceSquaredToAABB(CenterMM, ChunkMin, ChunkMax) <= RadiusSq)
                            {
                                ClearWholeChunk(ChunkCoord);
                                continue;
                            }

                            for (int32 LBZ = 0; LBZ < ChunkEdgeBlocks; ++LBZ)
                            {
                                for (int32 LBY = 0; LBY < ChunkEdgeBlocks; ++LBY)
                                {
                                    for (int32 LBX = 0; LBX < ChunkEdgeBlocks; ++LBX)
                                    {
                                        ++OutStats.BlocksVisited;

                                        const int32 BlockIndex = CVX::LocalBlockIndex(LBX, LBY, LBZ);
                                        const FIntVector BlockMinInt = ChunkMinInt + FIntVector(
                                            LBX * BlockEdgeVoxels,
                                            LBY * BlockEdgeVoxels,
                                            LBZ * BlockEdgeVoxels);
                                        const FVector3d BlockMin(double(BlockMinInt.X), double(BlockMinInt.Y), double(BlockMinInt.Z));
                                        const FVector3d BlockMax = BlockMin + FVector3d(BlockEdgeVoxels, BlockEdgeVoxels, BlockEdgeVoxels);

                                        if (MinDistanceSquaredToAABB(CenterMM, BlockMin, BlockMax) > RadiusSq)
                                        {
                                            continue;
                                        }

                                        FCVXChunk& Chunk = FindOrAddChunk(ChunkCoord);
                                        const uint64 OldMask = Chunk.GetMask(BlockIndex);
                                        if (OldMask == 0ull)
                                        {
                                            continue;
                                        }

                                        if (MaxDistanceSquaredToAABB(CenterMM, BlockMin, BlockMax) <= RadiusSq)
                                        {
                                            Chunk.SetMaskAndCompact(BlockIndex, 0ull);
                                            OutDelta.ChangedBlocks.Emplace(ChunkCoord, BlockIndex);
                                            ++OutStats.BlocksCleared;
                                            OutStats.VoxelBitsCleared += int64(PopCount64(OldMask));
                                            bAnyChange = true;
                                            continue;
                                        }

                                        ++OutStats.BoundaryBlocks;
                                        uint64 NewMask = OldMask;

                                        // Temporary 2 mm edit accelerator. This is not a stored
                                        // resolution level: it only classifies 2^3 groups while
                                        // rasterizing the edit stamp. Entire 2 mm groups can be
                                        // cleared with one mask operation; only groups crossed by
                                        // the sphere boundary perform eight exact 1 mm tests.
                                        for (int32 SZ = 0; SZ < 2; ++SZ)
                                        {
                                            for (int32 SY = 0; SY < 2; ++SY)
                                            {
                                                for (int32 SX = 0; SX < 2; ++SX)
                                                {
                                                    const FIntVector SubMinInt = BlockMinInt + FIntVector(SX * 2, SY * 2, SZ * 2);
                                                    const FVector3d SubMin(double(SubMinInt.X), double(SubMinInt.Y), double(SubMinInt.Z));
                                                    const FVector3d SubMax = SubMin + FVector3d(2.0, 2.0, 2.0);

                                                    if (MinDistanceSquaredToAABB(CenterMM, SubMin, SubMax) > RadiusSq)
                                                    {
                                                        continue;
                                                    }

                                                    uint64 SubMask = 0ull;
                                                    for (int32 VZ = 0; VZ < 2; ++VZ)
                                                    {
                                                        for (int32 VY = 0; VY < 2; ++VY)
                                                        {
                                                            for (int32 VX = 0; VX < 2; ++VX)
                                                            {
                                                                const int32 LX = SX * 2 + VX;
                                                                const int32 LY = SY * 2 + VY;
                                                                const int32 LZ = SZ * 2 + VZ;
                                                                SubMask |= uint64(1) << CVX::LocalVoxelBit(LX, LY, LZ);
                                                            }
                                                        }
                                                    }

                                                    if (MaxDistanceSquaredToAABB(CenterMM, SubMin, SubMax) <= RadiusSq)
                                                    {
                                                        const uint64 Before = NewMask;
                                                        NewMask &= ~SubMask;
                                                        if (NewMask != Before)
                                                        {
                                                            ++OutStats.SubBlocks2mmCleared;
                                                        }
                                                        continue;
                                                    }

                                                    for (int32 VZ = 0; VZ < 2; ++VZ)
                                                    {
                                                        for (int32 VY = 0; VY < 2; ++VY)
                                                        {
                                                            for (int32 VX = 0; VX < 2; ++VX)
                                                            {
                                                                ++OutStats.FineVoxelTests;
                                                                const int32 LX = SX * 2 + VX;
                                                                const int32 LY = SY * 2 + VY;
                                                                const int32 LZ = SZ * 2 + VZ;
                                                                const FIntVector GlobalVoxel = BlockMinInt + FIntVector(LX, LY, LZ);
                                                                if (GlobalVoxel.X >= LogicalSizeMM.X
                                                                    || GlobalVoxel.Y >= LogicalSizeMM.Y
                                                                    || GlobalVoxel.Z >= LogicalSizeMM.Z)
                                                                {
                                                                    continue;
                                                                }

                                                                const FVector3d VoxelCenter(
                                                                    double(GlobalVoxel.X) + 0.5,
                                                                    double(GlobalVoxel.Y) + 0.5,
                                                                    double(GlobalVoxel.Z) + 0.5);

                                                                if ((VoxelCenter - CenterMM).SizeSquared() <= RadiusSq)
                                                                {
                                                                    NewMask &= ~(uint64(1) << CVX::LocalVoxelBit(LX, LY, LZ));
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        if (NewMask != OldMask)
                                        {
                                            Chunk.SetMaskAndCompact(BlockIndex, NewMask);
                                            OutDelta.ChangedBlocks.Emplace(ChunkCoord, BlockIndex);
                                            OutStats.VoxelBitsCleared += int64(PopCount64(OldMask) - PopCount64(NewMask));
                                            bAnyChange = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (bAnyChange)
    {
        ++Generation;
    }
}

FCVXVolumeStats FCVXSparseVolume::GetStats() const
{
    FCVXVolumeStats Stats;
    Stats.LogicalVoxelCount = int64(LogicalSizeMM.X) * int64(LogicalSizeMM.Y) * int64(LogicalSizeMM.Z);
    Stats.Dense16BitBytes = uint64(Stats.LogicalVoxelCount) * 2ull;
    Stats.DirtyChunkCount = DirtyChunks.Num() + FullyEmptyChunks16MM.Num();
    Stats.EmptyBlockCount += FullyEmptyChunks16MM.Num() * CVX::BlocksPerChunk;

    for (const TPair<FIntVector, FCVXChunk>& Pair : DirtyChunks)
    {
        const FCVXChunk& Chunk = Pair.Value;
        Stats.MixedBlockCount += Chunk.MixedMasks.Num();
        for (int32 Block = 0; Block < BlocksPerChunk; ++Block)
        {
            switch (Chunk.GetState(Block))
            {
                case EBlockState::Empty: ++Stats.EmptyBlockCount; break;
                case EBlockState::Full:
                case EBlockState::Procedural: ++Stats.FullBlockCount; break;
                default: break;
            }
        }
    }

    // 3 uint4 rows (48 bytes) per dirty chunk, 1 uint2 (8 bytes) per mixed block.
    Stats.CompactGPUBytes = uint64(Stats.DirtyChunkCount) * 48ull + uint64(Stats.MixedBlockCount) * 8ull;
    return Stats;
}

void FCVXSparseVolume::BuildGPUData(TArray<FCVXGPUUint4>& OutChunkWords, TArray<FCVXGPUUint2>& OutMixedMasks) const
{
    OutChunkWords.Reset(DirtyChunks.Num() * 3);
    OutMixedMasks.Reset();

    for (const TPair<FIntVector, FCVXChunk>& Pair : DirtyChunks)
    {
        const FIntVector& Coord = Pair.Key;
        const FCVXChunk& Chunk = Pair.Value;
        const uint32 MixedOffset = static_cast<uint32>(OutMixedMasks.Num());

        OutChunkWords.Add(FCVXGPUUint4{
            static_cast<uint32>(Coord.X),
            static_cast<uint32>(Coord.Y),
            static_cast<uint32>(Coord.Z),
            static_cast<uint32>(DefaultMaterial)});

        OutChunkWords.Add(FCVXGPUUint4{
            Chunk.StateWords[0],
            Chunk.StateWords[1],
            Chunk.StateWords[2],
            Chunk.StateWords[3]});

        OutChunkWords.Add(FCVXGPUUint4{
            static_cast<uint32>(Chunk.MixedPresence & 0xffffffffull),
            static_cast<uint32>(Chunk.MixedPresence >> 32),
            MixedOffset,
            static_cast<uint32>(Chunk.MixedMasks.Num())});

        for (uint64 Mask : Chunk.MixedMasks)
        {
            OutMixedMasks.Add(FCVXGPUUint2{
                static_cast<uint32>(Mask & 0xffffffffull),
                static_cast<uint32>(Mask >> 32)});
        }
    }
}

void FCVXSparseVolume::GetDirtyChunkCoords(TArray<FIntVector>& OutCoords) const
{
    OutCoords.Reset(DirtyChunks.Num() + FullyEmptyChunks16MM.Num());
    for (const TPair<FIntVector, FCVXChunk>& Pair : DirtyChunks)
    {
        OutCoords.Add(Pair.Key);
    }
    for (const FIntVector& ChunkCoord : FullyEmptyChunks16MM)
    {
        OutCoords.Add(ChunkCoord);
    }
}

bool FCVXSparseVolume::IsChunkDirty(const FIntVector& ChunkCoord) const
{
    return FullyEmptyPages64MM.Contains(ChunkToBulkPage64MM(ChunkCoord))
        || FullyEmptyChunks16MM.Contains(ChunkCoord)
        || DirtyChunks.Contains(ChunkCoord);
}

bool FCVXSparseVolume::IsPageCompletelyEmpty64MM(const FIntVector& PageCoord) const
{
    return FullyEmptyPages64MM.Contains(PageCoord);
}

void FCVXSparseVolume::GetFullyEmptyPageCoords64MM(TArray<FIntVector>& OutPages) const
{
    OutPages.Reset(FullyEmptyPages64MM.Num());
    for (const FIntVector& PageCoord : FullyEmptyPages64MM)
    {
        OutPages.Add(PageCoord);
    }
}

CVX::EBlockState FCVXSparseVolume::GetBlockStateForVisualization(const FIntVector& ChunkCoord, int32 BlockIndex) const
{
    if (BlockIndex < 0 || BlockIndex >= CVX::BlocksPerChunk)
    {
        return CVX::EBlockState::Empty;
    }

    if (FullyEmptyPages64MM.Contains(ChunkToBulkPage64MM(ChunkCoord))
        || FullyEmptyChunks16MM.Contains(ChunkCoord))
    {
        return CVX::EBlockState::Empty;
    }

    const FCVXChunk* Chunk = FindChunk(ChunkCoord);
    return Chunk ? Chunk->GetState(BlockIndex) : CVX::EBlockState::Full;
}

uint64 FCVXSparseVolume::GetBlockMaskForVisualization(const FIntVector& ChunkCoord, int32 BlockIndex) const
{
    if (BlockIndex < 0 || BlockIndex >= CVX::BlocksPerChunk)
    {
        return 0ull;
    }

    if (FullyEmptyPages64MM.Contains(ChunkToBulkPage64MM(ChunkCoord))
        || FullyEmptyChunks16MM.Contains(ChunkCoord))
    {
        return 0ull;
    }

    const FCVXChunk* Chunk = FindChunk(ChunkCoord);
    if (!Chunk)
    {
        return CVX::FullBlockMask;
    }

    return Chunk->GetMask(BlockIndex);
}
