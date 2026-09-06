#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "CVXSparseVolume.h"
#include "CVXVirtualPageSpine.h"
#include "CVXSurfaceMesher.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXOneBillionLogicalTest,
    "CICADA.Voxel.CVX001A.OneBillionLogical",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXOneBillionLogicalTest::RunTest(const FString& Parameters)
{
    FCVXSparseVolume Volume;
    Volume.Reset(FIntVector(1000, 1000, 1000), 1);

    const FCVXVolumeStats Stats = Volume.GetStats();
    TestEqual(TEXT("Logical cell count"), Stats.LogicalVoxelCount, int64(1000000000));
    TestEqual(TEXT("Untouched volume allocates no dirty chunks"), Stats.DirtyChunkCount, 0);
    TestTrue(TEXT("Centre starts solid"), Volume.IsSolid(FIntVector(500, 500, 500)));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXSphereCarveTest,
    "CICADA.Voxel.CVX001A.SphereCarve",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXSphereCarveTest::RunTest(const FString& Parameters)
{
    FCVXSparseVolume Volume;
    Volume.Reset(FIntVector(1000, 1000, 1000), 1);
    Volume.CarveSphere(FVector3d(500.0, 500.0, 500.0), 25.0);

    TestFalse(TEXT("Sphere centre is carved"), Volume.IsSolid(FIntVector(500, 500, 500)));
    TestTrue(TEXT("Far voxel remains implicit solid"), Volume.IsSolid(FIntVector(100, 100, 100)));

    const FCVXVolumeStats First = Volume.GetStats();
    TestTrue(TEXT("Carve creates dirty chunks"), First.DirtyChunkCount > 0);
    TestTrue(TEXT("Carve creates mixed boundary blocks"), First.MixedBlockCount > 0);
    TestTrue(TEXT("Compact GPU payload stays below 1 MiB for proof cut"), First.CompactGPUBytes < 1024ull * 1024ull);

    // Idempotence: repeating the exact subtraction must not increase sparse representation size.
    Volume.CarveSphere(FVector3d(500.0, 500.0, 500.0), 25.0);
    const FCVXVolumeStats Second = Volume.GetStats();
    TestEqual(TEXT("Repeated carve dirty chunk count"), Second.DirtyChunkCount, First.DirtyChunkCount);
    TestEqual(TEXT("Repeated carve mixed block count"), Second.MixedBlockCount, First.MixedBlockCount);
    TestEqual(TEXT("Repeated carve payload bytes"), Second.CompactGPUBytes, First.CompactGPUBytes);
    return true;
}

#endif

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXVisualReadbackTest,
    "CICADA.Voxel.CVX001B.VisualReadback",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXVisualReadbackTest::RunTest(const FString& Parameters)
{
    FCVXSparseVolume Volume;
    Volume.Reset(FIntVector(1000, 1000, 1000), 1);

    TestFalse(TEXT("Untouched chunk is implicit"), Volume.IsChunkDirty(FIntVector(0, 31, 31)));
    TestEqual(TEXT("Implicit block reads full"),
        static_cast<uint8>(Volume.GetBlockStateForVisualization(FIntVector(0, 31, 31), 0)),
        static_cast<uint8>(CVX::EBlockState::Full));

    Volume.CarveSphere(FVector3d(18.0, 500.0, 500.0), 25.0);

    TArray<FIntVector> Dirty;
    Volume.GetDirtyChunkCoords(Dirty);
    TestTrue(TEXT("Visible crater creates dirty chunks"), Dirty.Num() > 0);
    TestFalse(TEXT("Crater centre is empty"), Volume.IsSolid(FIntVector(18, 500, 500)));
    TestTrue(TEXT("Unrelated voxel remains solid"), Volume.IsSolid(FIntVector(900, 500, 500)));
    return true;
}

#endif


#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXExactOneMillimetreEditTest,
    "CICADA.Voxel.CVX001C.ExactOneMillimetreEdit",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXExactOneMillimetreEditTest::RunTest(const FString& Parameters)
{
    FCVXSparseVolume Volume;
    Volume.Reset(FIntVector(1000, 1000, 1000), 1);

    const FIntVector Target(10, 10, 10);
    const FIntVector Neighbour(11, 10, 10);

    TestTrue(TEXT("Target starts solid"), Volume.IsSolid(Target));
    TestTrue(TEXT("Neighbour starts solid"), Volume.IsSolid(Neighbour));

    const uint64 GenerationBefore = Volume.GetGeneration();
    TestTrue(TEXT("Exact carve changes one cell"), Volume.CarveVoxel(Target));
    TestFalse(TEXT("Target is now empty"), Volume.IsSolid(Target));
    TestTrue(TEXT("Adjacent 1 mm cell remains solid"), Volume.IsSolid(Neighbour));
    TestEqual(TEXT("Exact edit increments generation once"), Volume.GetGeneration(), GenerationBefore + 1);

    const FCVXVolumeStats First = Volume.GetStats();
    TestEqual(TEXT("Single voxel edit dirties one 16 mm chunk"), First.DirtyChunkCount, 1);
    TestEqual(TEXT("Single voxel edit creates one mixed 4 mm block"), First.MixedBlockCount, 1);
    TestEqual(TEXT("Single voxel exact hot payload"), First.CompactGPUBytes, uint64(56));

    const uint64 GenerationAfter = Volume.GetGeneration();
    TestFalse(TEXT("Repeated exact carve is idempotent"), Volume.CarveVoxel(Target));
    TestEqual(TEXT("No-op exact carve keeps generation"), Volume.GetGeneration(), GenerationAfter);
    TestTrue(TEXT("Neighbour remains solid after repeat"), Volume.IsSolid(Neighbour));
    return true;
}

#endif


#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXChangedChunkReportingTest,
    "CICADA.Voxel.CVX002A.ChangedChunkReporting",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXChangedChunkReportingTest::RunTest(const FString& Parameters)
{
    FCVXSparseVolume Volume;
    Volume.Reset(FIntVector(1000, 1000, 1000), 1);

    TArray<FIntVector> Changed;
    const uint64 GenerationBefore = Volume.GetGeneration();
    Volume.CarveSphere(FVector3d(18.0, 500.0, 500.0), 12.0, Changed);

    TestTrue(TEXT("First subtraction reports changed chunks"), Changed.Num() > 0);
    TestEqual(TEXT("Changed subtraction increments generation once"), Volume.GetGeneration(), GenerationBefore + 1);

    TSet<FIntVector> Unique;
    for (const FIntVector& Coord : Changed)
    {
        Unique.Add(Coord);
        TestTrue(TEXT("Reported chunk is actually dirty"), Volume.IsChunkDirty(Coord));
    }
    TestEqual(TEXT("Changed chunk report contains no duplicates"), Unique.Num(), Changed.Num());

    const uint64 GenerationAfterFirst = Volume.GetGeneration();
    TArray<FIntVector> RepeatChanged;
    Volume.CarveSphere(FVector3d(18.0, 500.0, 500.0), 12.0, RepeatChanged);
    TestEqual(TEXT("Repeated identical carve reports no changed chunks"), RepeatChanged.Num(), 0);
    TestEqual(TEXT("Repeated identical carve keeps generation stable"), Volume.GetGeneration(), GenerationAfterFirst);
    return true;
}

#endif


#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXChangedBlockReportingTest,
    "CICADA.Voxel.CVX002B.ChangedBlockReporting",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXChangedBlockReportingTest::RunTest(const FString& Parameters)
{
    FCVXSparseVolume Volume;
    Volume.Reset(FIntVector(1000, 1000, 1000), 1);

    TArray<FCVXChangedBlock> ChangedBlocks;
    Volume.CarveSphere(FVector3d(18.0, 500.0, 500.0), 12.0, ChangedBlocks);

    TestTrue(TEXT("Subtraction reports changed 4 mm blocks"), ChangedBlocks.Num() > 0);

    TSet<uint64> UniqueKeys;
    for (const FCVXChangedBlock& Changed : ChangedBlocks)
    {
        TestTrue(TEXT("Block index is valid"), Changed.BlockIndex >= 0 && Changed.BlockIndex < CVX::BlocksPerChunk);
        TestTrue(TEXT("Changed block belongs to a dirty chunk"), Volume.IsChunkDirty(Changed.ChunkCoord));

        const uint64 CoordHash = uint64(GetTypeHash(Changed.ChunkCoord));
        const uint64 Key = (CoordHash << 8) ^ uint64(Changed.BlockIndex);
        UniqueKeys.Add(Key);
    }

    TestEqual(TEXT("Changed block report contains no duplicates"), UniqueKeys.Num(), ChangedBlocks.Num());

    TArray<FCVXChangedBlock> Repeat;
    Volume.CarveSphere(FVector3d(18.0, 500.0, 500.0), 12.0, Repeat);
    TestEqual(TEXT("Repeated identical carve reports zero blocks"), Repeat.Num(), 0);
    return true;
}

#endif


#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXVirtualPageAddressingTest,
    "CICADA.Voxel.CVX002D.VirtualPageAddressing",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXVirtualPageAddressingTest::RunTest(const FString& Parameters)
{
    TestEqual(TEXT("Hot page edge is 64 mm"), FCVXVirtualPageSpine::PageEdgeMM, 64);
    TestEqual(TEXT("64 chunks per hot page"), FCVXVirtualPageSpine::ChunksPerPage, 64);

    TestEqual(TEXT("Origin page"), FCVXVirtualPageSpine::ChunkToPageCoord(FIntVector(0, 0, 0)), FIntVector(0, 0, 0));
    TestEqual(TEXT("Chunk 3 stays in page 0"), FCVXVirtualPageSpine::ChunkToPageCoord(FIntVector(3, 3, 3)), FIntVector(0, 0, 0));
    TestEqual(TEXT("Chunk 4 moves to next page"), FCVXVirtualPageSpine::ChunkToPageCoord(FIntVector(4, 4, 4)), FIntVector(1, 1, 1));
    TestEqual(TEXT("Negative chunk floors correctly"), FCVXVirtualPageSpine::ChunkToPageCoord(FIntVector(-1, -4, -5)), FIntVector(-1, -1, -2));

    TestEqual(TEXT("Local chunk index origin"), FCVXVirtualPageSpine::ChunkToLocalIndex(FIntVector(0, 0, 0)), 0);
    TestEqual(TEXT("Local chunk index max"), FCVXVirtualPageSpine::ChunkToLocalIndex(FIntVector(3, 3, 3)), 63);
    TestEqual(TEXT("Negative local wraps"), FCVXVirtualPageSpine::ChunkToLocalIndex(FIntVector(-1, -1, -1)), 63);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXVirtualPageDeltaTest,
    "CICADA.Voxel.CVX002D.VirtualPageDelta",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXVirtualPageDeltaTest::RunTest(const FString& Parameters)
{
    FCVXSparseVolume Volume;
    Volume.Reset(FIntVector(1000, 1000, 1000), 1);

    FCVXVirtualPageSpine Spine;
    Spine.Reset();
    Spine.RebuildFromVolume(Volume);

    TestEqual(TEXT("Untouched billion-cell volume has zero edited pages"), Spine.GetStats().ResidentEditedPages, 0);
    TestEqual(TEXT("Untouched volume has zero resident payload"), Spine.GetStats().ResidentPayloadBytes, uint64(0));
    TestEqual(TEXT("Hot path never serializes full logical world"), Spine.GetStats().FullWorldUploadCount, uint64(0));

    TArray<FCVXChangedBlock> Changed;
    Volume.CarveSphere(FVector3d(18.0, 500.0, 500.0), 12.0, Changed);
    TestTrue(TEXT("Edit changes blocks"), Changed.Num() > 0);

    Spine.ApplyChangedBlocks(Volume, Changed);
    const FCVXVirtualPageStats First = Spine.GetStats();
    TestTrue(TEXT("Edit creates resident hot pages"), First.ResidentEditedPages > 0);
    TestTrue(TEXT("Edit creates explicit chunks"), First.ExplicitChunks > 0);
    TestTrue(TEXT("Delta transfer is non-zero"), First.LastDeltaBytes > 0);
    TestEqual(TEXT("Still zero full-world uploads"), First.FullWorldUploadCount, uint64(0));

    // A small edit should remain a sparse delta, nowhere near the 2 GB dense 16-bit world.
    TestTrue(TEXT("Small edit delta stays under 1 MiB"), First.LastDeltaBytes < 1024ull * 1024ull);

    TArray<FCVXChangedBlock> SecondChanged;
    Volume.CarveSphere(FVector3d(34.0, 500.0, 500.0), 8.0, SecondChanged);
    Spine.ApplyChangedBlocks(Volume, SecondChanged);
    const FCVXVirtualPageStats Second = Spine.GetStats();
    TestEqual(TEXT("Repeated hot updates keep full-world uploads at zero"), Second.FullWorldUploadCount, uint64(0));
    TestTrue(TEXT("Total delta accounting is monotonic"), Second.TotalDeltaBytes >= First.TotalDeltaBytes);
    return true;
}

#endif

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXHierarchicalRayTraversalTest,
    "CICADA.Voxel.CVX002E.HierarchicalRayTraversal",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXHierarchicalRayTraversalTest::RunTest(const FString& Parameters)
{
    // 16 mm empty-chunk skip.
    FCVXSparseVolume ChunkVolume;
    ChunkVolume.Reset(FIntVector(64, 32, 32), 1);
    for (int32 Z = 0; Z < 16; ++Z)
    {
        for (int32 Y = 0; Y < 16; ++Y)
        {
            for (int32 X = 0; X < 16; ++X)
            {
                ChunkVolume.CarveVoxel(FIntVector(X, Y, Z));
            }
        }
    }

    FIntVector HitVoxel;
    FCVXRayTraversalStats ChunkStats;
    TestTrue(TEXT("Ray finds solid material after empty 16 mm chunk"),
        ChunkVolume.FindFirstSolidAlongSegmentMM(
            FVector3d(0.5, 8.5, 8.5),
            FVector3d(63.5, 8.5, 8.5),
            HitVoxel,
            ChunkStats));
    TestEqual(TEXT("First solid voxel after empty chunk is x=16"), HitVoxel.X, 16);
    TestTrue(TEXT("Traversal used at least one 16 mm skip"), ChunkStats.ChunkSkips16mm > 0);

    // 4 mm empty-block skip.
    FCVXSparseVolume BlockVolume;
    BlockVolume.Reset(FIntVector(32, 16, 16), 1);
    for (int32 Z = 0; Z < 4; ++Z)
    {
        for (int32 Y = 0; Y < 4; ++Y)
        {
            for (int32 X = 0; X < 4; ++X)
            {
                BlockVolume.CarveVoxel(FIntVector(X, Y, Z));
            }
        }
    }

    FCVXRayTraversalStats BlockStats;
    TestTrue(TEXT("Ray finds solid material after empty 4 mm block"),
        BlockVolume.FindFirstSolidAlongSegmentMM(
            FVector3d(0.5, 1.5, 1.5),
            FVector3d(31.5, 1.5, 1.5),
            HitVoxel,
            BlockStats));
    TestEqual(TEXT("First solid voxel after empty block is x=4"), HitVoxel.X, 4);
    TestTrue(TEXT("Traversal used at least one 4 mm skip"), BlockStats.BlockSkips4mm > 0);

    // Mixed block falls to exact 1 mm stepping.
    FCVXSparseVolume FineVolume;
    FineVolume.Reset(FIntVector(16, 16, 16), 1);
    FineVolume.CarveVoxel(FIntVector(0, 1, 1));

    FCVXRayTraversalStats FineStats;
    TestTrue(TEXT("Mixed block traversal finds next exact 1 mm solid voxel"),
        FineVolume.FindFirstSolidAlongSegmentMM(
            FVector3d(0.5, 1.5, 1.5),
            FVector3d(15.5, 1.5, 1.5),
            HitVoxel,
            FineStats));
    TestEqual(TEXT("Next exact solid voxel is x=1"), HitVoxel.X, 1);
    TestTrue(TEXT("Mixed traversal performed 1 mm stepping"), FineStats.FineSteps1mm > 0);
    return true;
}

#endif


#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXExactSurfaceMesherSingleVoxelCavityTest,
    "CICADA.Voxel.CVX002F.ExactSurfaceMesher.SingleVoxelCavity",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXExactSurfaceMesherSingleVoxelCavityTest::RunTest(const FString& Parameters)
{
    FCVXSparseVolume Volume;
    Volume.Reset(FIntVector(64, 64, 64), 1);

    // Interior chunk so no world-boundary faces pollute the result.
    const FIntVector ChunkCoord(1, 1, 1);
    const FIntVector VoxelMM(24, 24, 24);
    TestTrue(TEXT("Exact centre voxel carve succeeds"), Volume.CarveVoxel(VoxelMM));

    FCVXSurfaceMeshData Mesh;
    FCVXSurfaceMesher::BuildChunkSurface(Volume, ChunkCoord, true, Mesh);

    TestEqual(TEXT("Single 1 mm cavity has six exact faces"), Mesh.QuadCount, 6);
    TestEqual(TEXT("Six quads produce twelve triangles"), Mesh.Triangles.Num() / 3, 12);
    TestEqual(TEXT("Mesher uses one 18 cubed halo snapshot"), Mesh.SampledCells, 18 * 18 * 18);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXExactSurfaceMesherGreedyTest,
    "CICADA.Voxel.CVX002F.ExactSurfaceMesher.GreedyMerge",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXExactSurfaceMesherGreedyTest::RunTest(const FString& Parameters)
{
    FCVXSparseVolume Volume;
    Volume.Reset(FIntVector(64, 64, 64), 1);

    // Two adjacent empty 1 mm cells form a 2x1x1 rectangular cavity. Exact topology is
    // still six rectangular faces when coplanar neighbours are greedily merged.
    TestTrue(TEXT("First exact carve"), Volume.CarveVoxel(FIntVector(24, 24, 24)));
    TestTrue(TEXT("Second adjacent exact carve"), Volume.CarveVoxel(FIntVector(25, 24, 24)));

    FCVXSurfaceMeshData Greedy;
    FCVXSurfaceMesher::BuildChunkSurface(Volume, FIntVector(1, 1, 1), true, Greedy);
    TestEqual(TEXT("Greedy 2x1x1 cavity collapses to six quads"), Greedy.QuadCount, 6);

    FCVXSurfaceMeshData Unmerged;
    FCVXSurfaceMesher::BuildChunkSurface(Volume, FIntVector(1, 1, 1), false, Unmerged);
    TestTrue(TEXT("Unmerged exact surface has more quads"), Unmerged.QuadCount > Greedy.QuadCount);
    TestEqual(TEXT("Both modes preserve exact triangle index validity"), Greedy.Triangles.Num() % 3, 0);
    return true;
}

#endif

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXHierarchicalBulkSphereExactnessTest,
    "CICADA.Voxel.CVX003B.HierarchicalBulkSphereExactness",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXHierarchicalBulkSphereExactnessTest::RunTest(const FString& Parameters)
{
    FCVXSparseVolume Legacy;
    FCVXSparseVolume Fast;
    Legacy.Reset(FIntVector(32, 32, 32), 1);
    Fast.Reset(FIntVector(32, 32, 32), 1);

    const FVector3d Center(15.25, 16.5, 14.75);
    const double Radius = 10.25;
    Legacy.CarveSphere(Center, Radius);

    FCVXEditDelta Delta;
    FCVXBulkEditStats BulkStats;
    Fast.CarveSphereHierarchical(Center, Radius, Delta, BulkStats);

    TestFalse(TEXT("Fast hierarchical edit reports a non-empty delta"), Delta.IsEmpty());
    TestTrue(TEXT("Hierarchical edit touches exact boundary blocks"), BulkStats.BoundaryBlocks > 0);

    int32 Mismatches = 0;
    for (int32 Z = 0; Z < 32; ++Z)
    {
        for (int32 Y = 0; Y < 32; ++Y)
        {
            for (int32 X = 0; X < 32; ++X)
            {
                const FIntVector Voxel(X, Y, Z);
                if (Legacy.IsSolid(Voxel) != Fast.IsSolid(Voxel))
                {
                    ++Mismatches;
                }
            }
        }
    }

    TestEqual(TEXT("Hierarchical stamp matches legacy exact 1 mm result"), Mismatches, 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCVXHierarchicalPageTombstoneTest,
    "CICADA.Voxel.CVX003B.PageTombstone",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCVXHierarchicalPageTombstoneTest::RunTest(const FString& Parameters)
{
    FCVXSparseVolume Volume;
    Volume.Reset(FIntVector(512, 512, 512), 1);

    FCVXEditDelta Delta;
    FCVXBulkEditStats BulkStats;
    Volume.CarveSphereHierarchical(FVector3d(256.0, 256.0, 256.0), 150.0, Delta, BulkStats);

    TestTrue(TEXT("Large edit clears at least one complete 64 mm page"), Delta.ClearedPages64MM.Num() > 0);
    TestTrue(TEXT("Large edit reports page-level acceleration"), BulkStats.PagesCleared > 0);
    TestFalse(TEXT("Sphere centre is empty"), Volume.IsSolid(FIntVector(256, 256, 256)));
    TestTrue(TEXT("Far corner remains solid"), Volume.IsSolid(FIntVector(10, 10, 10)));

    FCVXVirtualPageSpine Spine;
    Spine.Reset();
    Spine.ApplyEditDelta(Volume, Delta);
    TestTrue(TEXT("Virtual spine contains fully empty page records"), Spine.GetStats().FullyEmptyPages > 0);
    TestEqual(TEXT("Bulk edit never requires full-world upload"), Spine.GetStats().FullWorldUploadCount, uint64(0));

    TArray<uint32> Words;
    Spine.BuildEditDeltaWords(Volume, Delta, Words);
    TestTrue(TEXT("GPU edit delta packet exists"), Words.Num() >= 4);
    if (Words.Num() >= 2)
    {
        TestEqual(TEXT("CVX4 delta magic"), Words[0], uint32(0x34585643u));
        TestEqual(TEXT("CVX4 delta version"), Words[1], uint32(2u));
    }

    FCVXEditDelta RepeatDelta;
    FCVXBulkEditStats RepeatStats;
    Volume.CarveSphereHierarchical(FVector3d(256.0, 256.0, 256.0), 150.0, RepeatDelta, RepeatStats);
    TestTrue(TEXT("Repeated identical hierarchical subtract is idempotent"), RepeatDelta.IsEmpty());
    return true;
}

#endif
