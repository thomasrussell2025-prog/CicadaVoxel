#include "CVXRoboticsFieldBridge.h"

#include "Misc/ScopeLock.h"

namespace
{
    FCriticalSection GCVXRoboticsFieldMutex;
    TSharedPtr<const FCVXRoboticsFieldSnapshot, ESPMode::ThreadSafe> GCVXRoboticsFieldSnapshot;
}

void FCVXRoboticsFieldBridge::SubmitSnapshot(
    TSharedPtr<const FCVXRoboticsFieldSnapshot, ESPMode::ThreadSafe> Snapshot)
{
    FScopeLock Lock(&GCVXRoboticsFieldMutex);
    GCVXRoboticsFieldSnapshot = MoveTemp(Snapshot);
}

TSharedPtr<const FCVXRoboticsFieldSnapshot, ESPMode::ThreadSafe>
FCVXRoboticsFieldBridge::GetLatestSnapshot()
{
    FScopeLock Lock(&GCVXRoboticsFieldMutex);
    return GCVXRoboticsFieldSnapshot;
}

void FCVXRoboticsFieldBridge::ClearSnapshot(uint32 OwnerId)
{
    FScopeLock Lock(&GCVXRoboticsFieldMutex);
    if (GCVXRoboticsFieldSnapshot.IsValid()
        && GCVXRoboticsFieldSnapshot->OwnerId == OwnerId)
    {
        GCVXRoboticsFieldSnapshot.Reset();
    }
}
