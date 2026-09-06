# CVX-004A R4 FORCE DIRECT

## Why this exists
The R3 test video itself displayed:
- `CVX-003A page-native reference`
- `CVX-003C REALTIME SURFACE`

That proves the old mesh/page renderer was still active. The direct field was not.

R4 removes the choice in game/PIE worlds.

## Runtime invariant
`IsRoboticsDirectFieldRuntimeActive()` is true for every game/PIE world.
It no longer depends on a serialized actor property.

At BeginPlay R4 destroys/clears:
- coarse HISM
- cold-shell ISM
- frontier ISM
- 4 mm ISM
- 1 mm ISM
- 16 mm DynamicMesh chunks
- 64 mm DynamicMesh pages
- all legacy visual queues

## Required on-screen proof
When Play starts you must see:

    CVX-004A R4 DIRECT FIELD ACTIVE | LEGACY SKIN HARD OFF

The cyan runtime telemetry must include:

    CVX-004A R4 FORCE DIRECT
    legacySkin=HARD_OFF

If the overlay still says `CVX-003C REALTIME SURFACE`, Unreal is loading an old
CicadaVoxel game module and the deployment/build path is the bug, not rendering.

## Next
Once this invariant is confirmed, CVX-004B will replace the current per-frame compact
snapshot upload with persistent GPU page residency + delta patches, then add screen/tile
culling for robotics-scale worlds.
