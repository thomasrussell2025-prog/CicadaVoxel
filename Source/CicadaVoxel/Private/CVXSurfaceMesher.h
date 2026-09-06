#pragma once

#include "CoreMinimal.h"
#include "CVXSparseVolume.h"



struct FCVXSurfaceBlockSnapshot
{
    CVX::EBlockState State = CVX::EBlockState::Empty;
    uint64 Mask = 0ull;
};

/**
 * Thread-safe immutable 64 mm page snapshot. The game thread captures only the
 * existing 4 mm hierarchy (+ one-block halo); worker threads can then extract
 * exact 1 mm contour geometry without touching FCVXSparseVolume while edits continue.
 */
struct FCVXSurfacePageSnapshot
{
    static constexpr int32 BlocksPerPageAxis = 16;
    static constexpr int32 HaloBlocksPerAxis = BlocksPerPageAxis + 2; // 18

    FIntVector PageCoord = FIntVector::ZeroValue;
    FIntVector LogicalSizeMM = FIntVector::ZeroValue;
    TArray<FCVXSurfaceBlockSnapshot> Blocks;

    void Reset()
    {
        PageCoord = FIntVector::ZeroValue;
        LogicalSizeMM = FIntVector::ZeroValue;
        Blocks.Reset();
    }

    static int32 Index(int32 BX, int32 BY, int32 BZ)
    {
        return (BX + 1) + HaloBlocksPerAxis * ((BY + 1) + HaloBlocksPerAxis * (BZ + 1));
    }

    const FCVXSurfaceBlockSnapshot& GetBlock(int32 BX, int32 BY, int32 BZ) const
    {
        static const FCVXSurfaceBlockSnapshot Empty;
        if (BX < -1 || BY < -1 || BZ < -1
            || BX > BlocksPerPageAxis || BY > BlocksPerPageAxis || BZ > BlocksPerPageAxis)
        {
            return Empty;
        }
        const int32 I = Index(BX, BY, BZ);
        return Blocks.IsValidIndex(I) ? Blocks[I] : Empty;
    }
};

struct FCVXSurfaceMeshData
{
    TArray<FVector3d> VerticesCM;
    TArray<FVector3f> Normals;
    TArray<int32> Triangles;
    int32 QuadCount = 0;
    int32 SampledCells = 0;

    void Reset()
    {
        VerticesCM.Reset();
        Normals.Reset();
        Triangles.Reset();
        QuadCount = 0;
        SampledCells = 0;
    }
};

/**
 * Exact 1 mm surface extractor for one 16 mm CVX chunk.
 *
 * The volume remains sparse/implicit. We snapshot an 18^3 one-cell halo around the
 * chunk, then emit only faces where SOLID touches EMPTY. With greedy merging enabled,
 * coplanar adjacent 1 mm faces collapse into larger quads without changing the exact
 * millimetre boundary of the material field.
 */
class FCVXSurfaceMesher
{
public:
    static constexpr int32 ChunkEdgeMM = CVX::ChunkEdgeVoxels; // 16
    static constexpr int32 HaloEdge = ChunkEdgeMM + 2;          // 18

    static void BuildChunkSurface(
        const FCVXSparseVolume& Volume,
        const FIntVector& ChunkCoord,
        bool bGreedyMerge,
        FCVXSurfaceMeshData& OutMesh);

    // CVX-003A page-native reference bridge. One 64 mm page contains 4^3 16 mm
    // chunks. The chunk surfaces are generated in parallel and merged into one
    // render mesh, replacing the old hundreds-of-small-mesh catch-up queue.
    static constexpr int32 PageEdgeChunks = 4;
    static constexpr int32 PageEdgeMM = ChunkEdgeMM * PageEdgeChunks; // 64

    static FIntVector ChunkToPageCoord(const FIntVector& ChunkCoord);
    static FIntVector PageToChunkOrigin(const FIntVector& PageCoord);

    static void BuildPageSnapshot(
        const FCVXSparseVolume& Volume,
        const FIntVector& PageCoord,
        FCVXSurfacePageSnapshot& OutSnapshot);

    static void BuildPageSurface(
        const FCVXSparseVolume& Volume,
        const FIntVector& PageCoord,
        bool bGreedyMerge,
        FCVXSurfaceMeshData& OutMesh);

    static void BuildPageSurfaceFromSnapshot(
        const FCVXSurfacePageSnapshot& Snapshot,
        bool bGreedyMerge,
        FCVXSurfaceMeshData& OutMesh);

    // Fast outer-skin correctness path. Only the world-boundary plane(s) of a
    // touched page are rebuilt synchronously, so visible holes update in the same
    // frame while the complete interior page mesh is generated on worker threads.
    static void BuildPageExteriorSurfaceFromSnapshot(
        const FCVXSurfacePageSnapshot& Snapshot,
        FCVXSurfaceMeshData& OutMesh);

private:
    static int32 HaloIndex(int32 X, int32 Y, int32 Z)
    {
        return X + HaloEdge * (Y + HaloEdge * Z);
    }

    static void EmitQuad(
        FCVXSurfaceMeshData& Mesh,
        int32 Axis,
        int32 Sign,
        int32 PlaneMM,
        int32 U0MM,
        int32 V0MM,
        int32 USizeMM,
        int32 VSizeMM);
};
