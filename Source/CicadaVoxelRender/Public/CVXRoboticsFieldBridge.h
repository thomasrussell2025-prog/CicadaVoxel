#pragma once

#include "CoreMinimal.h"

// CVX-004A robotics/training field snapshot.
//
// This is not render geometry. It is a compact GPU-readable view of the same
// authoritative sparse material field used by edits and queries:
//
//   1 mm truth -> 4 mm block -> 16 mm chunk -> 64 mm page.
//
// Sentinel values deliberately make untouched solid world implicit.
struct CICADAVOXELRENDER_API FCVXRoboticsFieldSnapshot
{
    static constexpr uint32 ImplicitFull = 0xFFFFFFFFu;
    static constexpr uint32 FullyEmpty = 0xFFFFFFFEu;
    static constexpr int32 PageEdgeMM = 64;
    static constexpr int32 ChunksPerPage = 64;
    static constexpr int32 ChunkWords = 8;

    uint64 Generation = 0;
    uint32 OwnerId = 0;
    bool bActive = false;

    // Actor transform is kept separate from field topology so world-space robot
    // sensors and the human diagnostic view query exactly the same local field.
    FTransform WorldToLocal;

    FIntVector LogicalSizeMM = FIntVector::ZeroValue;
    FIntVector PageCounts = FIntVector::ZeroValue;

    // CVX-004B: field topology no longer lives in per-frame snapshots.
    // Persistent GPU page residency is updated only through compact CVX4 deltas.
    uint64 GetPayloadBytes() const
    {
        return 0u;
    }
};

class CICADAVOXELRENDER_API FCVXRoboticsFieldBridge
{
public:
    static void SubmitSnapshot(TSharedPtr<const FCVXRoboticsFieldSnapshot, ESPMode::ThreadSafe> Snapshot);
    static TSharedPtr<const FCVXRoboticsFieldSnapshot, ESPMode::ThreadSafe> GetLatestSnapshot();
    static void ClearSnapshot(uint32 OwnerId);
};
