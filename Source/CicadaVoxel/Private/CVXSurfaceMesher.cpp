#include "CVXSurfaceMesher.h"

namespace
{
    constexpr double MMToCM = 0.1;
}

void FCVXSurfaceMesher::EmitQuad(
    FCVXSurfaceMeshData& Mesh,
    int32 Axis,
    int32 Sign,
    int32 PlaneMM,
    int32 U0MM,
    int32 V0MM,
    int32 USizeMM,
    int32 VSizeMM)
{
    // Cyclic basis chosen so Cross(U,V) = +Axis:
    // X: U=Y,V=Z  Y: U=Z,V=X  Z: U=X,V=Y.
    const int32 UAxis = (Axis + 1) % 3;
    const int32 VAxis = (Axis + 2) % 3;

    FVector3d P0(0.0, 0.0, 0.0);
    FVector3d PU(0.0, 0.0, 0.0);
    FVector3d PV(0.0, 0.0, 0.0);
    FVector3d PUV(0.0, 0.0, 0.0);

    P0[Axis] = double(PlaneMM);
    P0[UAxis] = double(U0MM);
    P0[VAxis] = double(V0MM);

    PU = P0;
    PU[UAxis] += double(USizeMM);

    PV = P0;
    PV[VAxis] += double(VSizeMM);

    PUV = PU;
    PUV[VAxis] += double(VSizeMM);

    P0 *= MMToCM;
    PU *= MMToCM;
    PV *= MMToCM;
    PUV *= MMToCM;

    const int32 BaseVertex = Mesh.VerticesCM.Num();
    Mesh.VerticesCM.Add(P0);
    Mesh.VerticesCM.Add(PU);
    Mesh.VerticesCM.Add(PUV);
    Mesh.VerticesCM.Add(PV);

    FVector3f Normal(0.0f, 0.0f, 0.0f);
    Normal[Axis] = Sign > 0 ? 1.0f : -1.0f;
    Mesh.Normals.Add(Normal);
    Mesh.Normals.Add(Normal);
    Mesh.Normals.Add(Normal);
    Mesh.Normals.Add(Normal);

    if (Sign > 0)
    {
        Mesh.Triangles.Add(BaseVertex + 0);
        Mesh.Triangles.Add(BaseVertex + 1);
        Mesh.Triangles.Add(BaseVertex + 2);
        Mesh.Triangles.Add(BaseVertex + 0);
        Mesh.Triangles.Add(BaseVertex + 2);
        Mesh.Triangles.Add(BaseVertex + 3);
    }
    else
    {
        Mesh.Triangles.Add(BaseVertex + 0);
        Mesh.Triangles.Add(BaseVertex + 3);
        Mesh.Triangles.Add(BaseVertex + 2);
        Mesh.Triangles.Add(BaseVertex + 0);
        Mesh.Triangles.Add(BaseVertex + 2);
        Mesh.Triangles.Add(BaseVertex + 1);
    }

    ++Mesh.QuadCount;
}

void FCVXSurfaceMesher::BuildChunkSurface(
    const FCVXSparseVolume& Volume,
    const FIntVector& ChunkCoord,
    bool bGreedyMerge,
    FCVXSurfaceMeshData& OutMesh)
{
    OutMesh.Reset();

    const FIntVector ChunkOrigin(
        ChunkCoord.X * ChunkEdgeMM,
        ChunkCoord.Y * ChunkEdgeMM,
        ChunkCoord.Z * ChunkEdgeMM);

    // One-cell halo makes all six neighbour queries local array reads during meshing.
    // 18^3 = 5,832 IsSolid() queries per rebuilt chunk instead of up to 24,576.
    TArray<uint8> Solid;
    Solid.SetNumUninitialized(HaloEdge * HaloEdge * HaloEdge);

    for (int32 Z = 0; Z < HaloEdge; ++Z)
    {
        for (int32 Y = 0; Y < HaloEdge; ++Y)
        {
            for (int32 X = 0; X < HaloEdge; ++X)
            {
                const FIntVector Voxel = ChunkOrigin + FIntVector(X - 1, Y - 1, Z - 1);
                Solid[HaloIndex(X, Y, Z)] = Volume.IsSolid(Voxel) ? 1u : 0u;
                ++OutMesh.SampledCells;
            }
        }
    }

    auto IsLocalSolid = [&Solid](int32 X, int32 Y, int32 Z) -> bool
    {
        return Solid[HaloIndex(X + 1, Y + 1, Z + 1)] != 0u;
    };

    // Each orientation is processed independently. A 16x16 mask exists for one slice.
    // Greedy rectangles reduce a flat 16x16 wall from 256 quads to one while every edge
    // still lies on an integer 1 mm coordinate.
    for (int32 Axis = 0; Axis < 3; ++Axis)
    {
        const int32 UAxis = (Axis + 1) % 3;
        const int32 VAxis = (Axis + 2) % 3;

        for (int32 SignIndex = 0; SignIndex < 2; ++SignIndex)
        {
            const int32 Sign = SignIndex == 0 ? -1 : 1;

            for (int32 Slice = 0; Slice < ChunkEdgeMM; ++Slice)
            {
                bool Mask[ChunkEdgeMM * ChunkEdgeMM] = {};

                for (int32 V = 0; V < ChunkEdgeMM; ++V)
                {
                    for (int32 U = 0; U < ChunkEdgeMM; ++U)
                    {
                        int32 C[3] = { 0, 0, 0 };
                        C[Axis] = Slice;
                        C[UAxis] = U;
                        C[VAxis] = V;

                        if (!IsLocalSolid(C[0], C[1], C[2]))
                        {
                            continue;
                        }

                        C[Axis] += Sign;
                        const bool bNeighbourSolid = IsLocalSolid(C[0], C[1], C[2]);
                        if (!bNeighbourSolid)
                        {
                            Mask[U + ChunkEdgeMM * V] = true;
                        }
                    }
                }

                for (int32 V = 0; V < ChunkEdgeMM; ++V)
                {
                    for (int32 U = 0; U < ChunkEdgeMM; )
                    {
                        const int32 MaskIndex = U + ChunkEdgeMM * V;
                        if (!Mask[MaskIndex])
                        {
                            ++U;
                            continue;
                        }

                        int32 Width = 1;
                        if (bGreedyMerge)
                        {
                            while (U + Width < ChunkEdgeMM
                                && Mask[(U + Width) + ChunkEdgeMM * V])
                            {
                                ++Width;
                            }
                        }

                        int32 Height = 1;
                        if (bGreedyMerge)
                        {
                            bool bCanGrow = true;
                            while (V + Height < ChunkEdgeMM && bCanGrow)
                            {
                                for (int32 DU = 0; DU < Width; ++DU)
                                {
                                    if (!Mask[(U + DU) + ChunkEdgeMM * (V + Height)])
                                    {
                                        bCanGrow = false;
                                        break;
                                    }
                                }
                                if (bCanGrow)
                                {
                                    ++Height;
                                }
                            }
                        }

                        for (int32 DV = 0; DV < Height; ++DV)
                        {
                            for (int32 DU = 0; DU < Width; ++DU)
                            {
                                Mask[(U + DU) + ChunkEdgeMM * (V + DV)] = false;
                            }
                        }

                        const int32 Plane = Sign > 0 ? Slice + 1 : Slice;
                        EmitQuad(OutMesh, Axis, Sign, Plane, U, V, Width, Height);
                        U += Width;
                    }
                }
            }
        }
    }
}


FIntVector FCVXSurfaceMesher::ChunkToPageCoord(const FIntVector& ChunkCoord)
{
    auto FloorDiv4 = [](int32 V) -> int32
    {
        int32 Q = V / PageEdgeChunks;
        const int32 R = V % PageEdgeChunks;
        if (R < 0)
        {
            --Q;
        }
        return Q;
    };

    return FIntVector(FloorDiv4(ChunkCoord.X), FloorDiv4(ChunkCoord.Y), FloorDiv4(ChunkCoord.Z));
}

FIntVector FCVXSurfaceMesher::PageToChunkOrigin(const FIntVector& PageCoord)
{
    return PageCoord * PageEdgeChunks;
}


void FCVXSurfaceMesher::BuildPageSnapshot(
    const FCVXSparseVolume& Volume,
    const FIntVector& PageCoord,
    FCVXSurfacePageSnapshot& OutSnapshot)
{
    OutSnapshot.Reset();
    OutSnapshot.PageCoord = PageCoord;
    OutSnapshot.LogicalSizeMM = Volume.GetLogicalSizeMM();
    OutSnapshot.Blocks.SetNumZeroed(
        FCVXSurfacePageSnapshot::HaloBlocksPerAxis
        * FCVXSurfacePageSnapshot::HaloBlocksPerAxis
        * FCVXSurfacePageSnapshot::HaloBlocksPerAxis);

    const FIntVector PageOriginMM = PageCoord * PageEdgeMM;
    const FIntVector PageBlockOrigin(
        PageOriginMM.X / CVX::BlockEdgeVoxels,
        PageOriginMM.Y / CVX::BlockEdgeVoxels,
        PageOriginMM.Z / CVX::BlockEdgeVoxels);
    const FIntVector LogicalSize = OutSnapshot.LogicalSizeMM;

    for (int32 BZ = -1; BZ <= FCVXSurfacePageSnapshot::BlocksPerPageAxis; ++BZ)
    {
        for (int32 BY = -1; BY <= FCVXSurfacePageSnapshot::BlocksPerPageAxis; ++BY)
        {
            for (int32 BX = -1; BX <= FCVXSurfacePageSnapshot::BlocksPerPageAxis; ++BX)
            {
                FCVXSurfaceBlockSnapshot& Dest = OutSnapshot.Blocks[
                    FCVXSurfacePageSnapshot::Index(BX, BY, BZ)];
                const FIntVector GlobalBlock = PageBlockOrigin + FIntVector(BX, BY, BZ);
                const FIntVector BlockMinMM = GlobalBlock * CVX::BlockEdgeVoxels;
                const FIntVector BlockMaxMM = BlockMinMM + FIntVector(CVX::BlockEdgeVoxels);

                const bool bOutside = BlockMaxMM.X <= 0 || BlockMaxMM.Y <= 0 || BlockMaxMM.Z <= 0
                    || BlockMinMM.X >= LogicalSize.X || BlockMinMM.Y >= LogicalSize.Y || BlockMinMM.Z >= LogicalSize.Z;
                if (bOutside)
                {
                    Dest.State = CVX::EBlockState::Empty;
                    Dest.Mask = 0ull;
                    continue;
                }

                const bool bPartial = BlockMinMM.X < 0 || BlockMinMM.Y < 0 || BlockMinMM.Z < 0
                    || BlockMaxMM.X > LogicalSize.X || BlockMaxMM.Y > LogicalSize.Y || BlockMaxMM.Z > LogicalSize.Z;
                if (bPartial)
                {
                    uint64 Mask = 0ull;
                    for (int32 Z = 0; Z < CVX::BlockEdgeVoxels; ++Z)
                    {
                        for (int32 Y = 0; Y < CVX::BlockEdgeVoxels; ++Y)
                        {
                            for (int32 X = 0; X < CVX::BlockEdgeVoxels; ++X)
                            {
                                const FIntVector Voxel = BlockMinMM + FIntVector(X, Y, Z);
                                if (Volume.IsSolid(Voxel))
                                {
                                    Mask |= uint64(1) << CVX::LocalVoxelBit(X, Y, Z);
                                }
                            }
                        }
                    }
                    Dest.Mask = Mask;
                    Dest.State = Mask == 0ull
                        ? CVX::EBlockState::Empty
                        : (Mask == CVX::FullBlockMask ? CVX::EBlockState::Full : CVX::EBlockState::Mixed);
                    continue;
                }

                const FIntVector ChunkCoord(
                    GlobalBlock.X / CVX::ChunkEdgeBlocks,
                    GlobalBlock.Y / CVX::ChunkEdgeBlocks,
                    GlobalBlock.Z / CVX::ChunkEdgeBlocks);
                const FIntVector LocalBlock(
                    GlobalBlock.X & (CVX::ChunkEdgeBlocks - 1),
                    GlobalBlock.Y & (CVX::ChunkEdgeBlocks - 1),
                    GlobalBlock.Z & (CVX::ChunkEdgeBlocks - 1));
                const int32 BlockIndex = CVX::LocalBlockIndex(LocalBlock.X, LocalBlock.Y, LocalBlock.Z);
                Dest.State = Volume.GetBlockStateForVisualization(ChunkCoord, BlockIndex);
                if (Dest.State == CVX::EBlockState::Procedural)
                {
                    Dest.State = CVX::EBlockState::Full;
                }
                Dest.Mask = Dest.State == CVX::EBlockState::Mixed
                    ? Volume.GetBlockMaskForVisualization(ChunkCoord, BlockIndex)
                    : (Dest.State == CVX::EBlockState::Full ? CVX::FullBlockMask : 0ull);
            }
        }
    }
}

void FCVXSurfaceMesher::BuildPageSurface(
    const FCVXSparseVolume& Volume,
    const FIntVector& PageCoord,
    bool bGreedyMerge,
    FCVXSurfaceMeshData& OutMesh)
{
    FCVXSurfacePageSnapshot Snapshot;
    BuildPageSnapshot(Volume, PageCoord, Snapshot);
    BuildPageSurfaceFromSnapshot(Snapshot, bGreedyMerge, OutMesh);
}

void FCVXSurfaceMesher::BuildPageSurfaceFromSnapshot(
    const FCVXSurfacePageSnapshot& Snapshot,
    bool bGreedyMerge,
    FCVXSurfaceMeshData& OutMesh)
{
    OutMesh.Reset();
    constexpr int32 BlocksPerPageAxis = FCVXSurfacePageSnapshot::BlocksPerPageAxis;
    const FIntVector PageOriginMM = Snapshot.PageCoord * PageEdgeMM;

    auto IsMixedVoxelSolid = [](uint64 Mask, int32 X, int32 Y, int32 Z) -> bool
    {
        return (Mask & (uint64(1) << CVX::LocalVoxelBit(X, Y, Z))) != 0ull;
    };

    auto IsVoxelSolidLocal = [&Snapshot, &IsMixedVoxelSolid](const FIntVector& LocalVoxel) -> bool
    {
        int32 BC[3];
        int32 VC[3];
        for (int32 Axis = 0; Axis < 3; ++Axis)
        {
            const int32 V = LocalVoxel[Axis];
            if (V < 0)
            {
                BC[Axis] = -1;
                VC[Axis] = CVX::BlockEdgeVoxels - 1;
            }
            else
            {
                BC[Axis] = V / CVX::BlockEdgeVoxels;
                VC[Axis] = V % CVX::BlockEdgeVoxels;
            }
        }

        const FCVXSurfaceBlockSnapshot& Block = Snapshot.GetBlock(BC[0], BC[1], BC[2]);
        if (Block.State == CVX::EBlockState::Full || Block.State == CVX::EBlockState::Procedural)
        {
            return true;
        }
        if (Block.State != CVX::EBlockState::Mixed)
        {
            return false;
        }
        return IsMixedVoxelSolid(Block.Mask, VC[0], VC[1], VC[2]);
    };

    auto EmitUnitFace = [&OutMesh](const FIntVector& LocalVoxel, int32 Axis, int32 Sign)
    {
        const int32 UAxis = (Axis + 1) % 3;
        const int32 VAxis = (Axis + 2) % 3;
        EmitQuad(
            OutMesh,
            Axis,
            Sign,
            LocalVoxel[Axis] + (Sign > 0 ? 1 : 0),
            LocalVoxel[UAxis],
            LocalVoxel[VAxis],
            1,
            1);
    };

    static const FIntVector NeighbourDelta[6] =
    {
        FIntVector(-1, 0, 0), FIntVector(1, 0, 0),
        FIntVector(0, -1, 0), FIntVector(0, 1, 0),
        FIntVector(0, 0, -1), FIntVector(0, 0, 1)
    };

    for (int32 BZ = 0; BZ < BlocksPerPageAxis; ++BZ)
    {
        for (int32 BY = 0; BY < BlocksPerPageAxis; ++BY)
        {
            for (int32 BX = 0; BX < BlocksPerPageAxis; ++BX)
            {
                const FCVXSurfaceBlockSnapshot& Current = Snapshot.GetBlock(BX, BY, BZ);
                if (Current.State == CVX::EBlockState::Empty)
                {
                    continue;
                }

                const FIntVector LocalBlockMinMM(
                    BX * CVX::BlockEdgeVoxels,
                    BY * CVX::BlockEdgeVoxels,
                    BZ * CVX::BlockEdgeVoxels);

                if (Current.State == CVX::EBlockState::Full || Current.State == CVX::EBlockState::Procedural)
                {
                    for (int32 Face = 0; Face < 6; ++Face)
                    {
                        const int32 Axis = Face / 2;
                        const int32 Sign = (Face & 1) ? 1 : -1;
                        const FIntVector DB = NeighbourDelta[Face];
                        const FCVXSurfaceBlockSnapshot& Neighbour = Snapshot.GetBlock(BX + DB.X, BY + DB.Y, BZ + DB.Z);
                        if (Neighbour.State == CVX::EBlockState::Full || Neighbour.State == CVX::EBlockState::Procedural)
                        {
                            continue;
                        }

                        const int32 UAxis = (Axis + 1) % 3;
                        const int32 VAxis = (Axis + 2) % 3;
                        if (Neighbour.State == CVX::EBlockState::Empty)
                        {
                            EmitQuad(
                                OutMesh,
                                Axis,
                                Sign,
                                LocalBlockMinMM[Axis] + (Sign > 0 ? CVX::BlockEdgeVoxels : 0),
                                LocalBlockMinMM[UAxis],
                                LocalBlockMinMM[VAxis],
                                CVX::BlockEdgeVoxels,
                                CVX::BlockEdgeVoxels);
                            continue;
                        }

                        for (int32 V = 0; V < CVX::BlockEdgeVoxels; ++V)
                        {
                            for (int32 U = 0; U < CVX::BlockEdgeVoxels; ++U)
                            {
                                int32 NV[3] = {0,0,0};
                                NV[Axis] = Sign > 0 ? 0 : CVX::BlockEdgeVoxels - 1;
                                NV[UAxis] = U;
                                NV[VAxis] = V;
                                if (!IsMixedVoxelSolid(Neighbour.Mask, NV[0], NV[1], NV[2]))
                                {
                                    FIntVector LocalVoxel = LocalBlockMinMM;
                                    LocalVoxel[Axis] += Sign > 0 ? CVX::BlockEdgeVoxels - 1 : 0;
                                    LocalVoxel[UAxis] += U;
                                    LocalVoxel[VAxis] += V;
                                    EmitUnitFace(LocalVoxel, Axis, Sign);
                                }
                            }
                        }
                    }
                    continue;
                }

                for (int32 VZ = 0; VZ < CVX::BlockEdgeVoxels; ++VZ)
                {
                    for (int32 VY = 0; VY < CVX::BlockEdgeVoxels; ++VY)
                    {
                        for (int32 VX = 0; VX < CVX::BlockEdgeVoxels; ++VX)
                        {
                            if (!IsMixedVoxelSolid(Current.Mask, VX, VY, VZ))
                            {
                                continue;
                            }
                            const FIntVector LocalVoxel = LocalBlockMinMM + FIntVector(VX, VY, VZ);
                            for (int32 Face = 0; Face < 6; ++Face)
                            {
                                ++OutMesh.SampledCells;
                                if (!IsVoxelSolidLocal(LocalVoxel + NeighbourDelta[Face]))
                                {
                                    EmitUnitFace(LocalVoxel, Face / 2, (Face & 1) ? 1 : -1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    (void)bGreedyMerge;
    (void)PageOriginMM;
}

void FCVXSurfaceMesher::BuildPageExteriorSurfaceFromSnapshot(
    const FCVXSurfacePageSnapshot& Snapshot,
    FCVXSurfaceMeshData& OutMesh)
{
    OutMesh.Reset();
    const FIntVector PageOrigin = Snapshot.PageCoord * PageEdgeMM;
    const FIntVector LogicalSize = Snapshot.LogicalSizeMM;

    auto IsMixedVoxelSolid = [](uint64 Mask, int32 X, int32 Y, int32 Z) -> bool
    {
        return (Mask & (uint64(1) << CVX::LocalVoxelBit(X, Y, Z))) != 0ull;
    };

    auto IsVoxelSolidLocal = [&Snapshot, &IsMixedVoxelSolid](const FIntVector& LocalVoxel) -> bool
    {
        int32 BC[3];
        int32 VC[3];
        for (int32 Axis = 0; Axis < 3; ++Axis)
        {
            const int32 V = LocalVoxel[Axis];
            if (V < 0) { BC[Axis] = -1; VC[Axis] = CVX::BlockEdgeVoxels - 1; }
            else { BC[Axis] = V / CVX::BlockEdgeVoxels; VC[Axis] = V % CVX::BlockEdgeVoxels; }
        }
        const FCVXSurfaceBlockSnapshot& Block = Snapshot.GetBlock(BC[0], BC[1], BC[2]);
        if (Block.State == CVX::EBlockState::Full || Block.State == CVX::EBlockState::Procedural) return true;
        if (Block.State != CVX::EBlockState::Mixed) return false;
        return IsMixedVoxelSolid(Block.Mask, VC[0], VC[1], VC[2]);
    };

    for (int32 Axis = 0; Axis < 3; ++Axis)
    {
        const int32 UAxis = (Axis + 1) % 3;
        const int32 VAxis = (Axis + 2) % 3;
        for (int32 SignIndex = 0; SignIndex < 2; ++SignIndex)
        {
            const int32 Sign = SignIndex == 0 ? -1 : 1;
            const int32 WorldPlane = Sign < 0 ? 0 : LogicalSize[Axis];
            const bool bTouches = Sign < 0
                ? PageOrigin[Axis] == 0
                : (PageOrigin[Axis] < LogicalSize[Axis] && PageOrigin[Axis] + PageEdgeMM >= LogicalSize[Axis]);
            if (!bTouches) continue;

            const int32 PlaneLocal = WorldPlane - PageOrigin[Axis];
            const int32 LocalVoxelAxis = Sign < 0 ? 0 : PlaneLocal - 1;
            const int32 UCount = FMath::Clamp(LogicalSize[UAxis] - PageOrigin[UAxis], 0, PageEdgeMM);
            const int32 VCount = FMath::Clamp(LogicalSize[VAxis] - PageOrigin[VAxis], 0, PageEdgeMM);
            if (UCount <= 0 || VCount <= 0) continue;

            TArray<uint8> Mask;
            Mask.SetNumZeroed(UCount * VCount);
            for (int32 V = 0; V < VCount; ++V)
            {
                for (int32 U = 0; U < UCount; ++U)
                {
                    int32 C[3] = {0,0,0};
                    C[Axis] = LocalVoxelAxis;
                    C[UAxis] = U;
                    C[VAxis] = V;
                    if (IsVoxelSolidLocal(FIntVector(C[0], C[1], C[2])))
                    {
                        Mask[U + UCount * V] = 1u;
                    }
                }
            }

            for (int32 V = 0; V < VCount; ++V)
            {
                for (int32 U = 0; U < UCount; )
                {
                    if (!Mask[U + UCount * V]) { ++U; continue; }
                    int32 Width = 1;
                    while (U + Width < UCount && Mask[(U + Width) + UCount * V]) ++Width;
                    int32 Height = 1;
                    bool bGrow = true;
                    while (V + Height < VCount && bGrow)
                    {
                        for (int32 DU = 0; DU < Width; ++DU)
                        {
                            if (!Mask[(U + DU) + UCount * (V + Height)]) { bGrow = false; break; }
                        }
                        if (bGrow) ++Height;
                    }
                    for (int32 DV = 0; DV < Height; ++DV)
                        for (int32 DU = 0; DU < Width; ++DU)
                            Mask[(U + DU) + UCount * (V + DV)] = 0u;
                    EmitQuad(OutMesh, Axis, Sign, PlaneLocal, U, V, Width, Height);
                    U += Width;
                }
            }
        }
    }
}


