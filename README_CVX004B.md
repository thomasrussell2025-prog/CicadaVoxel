# CVX-004B — Persistent GPU Field

## Why
CVX-004A proved the direct material-field renderer and eliminated the outer-skin bug,
but it still did two expensive things:

1. While editing, the game thread repeatedly scanned all dirty chunks to rebuild a
   complete compact snapshot.
2. Every rendered frame copied those arrays and created/uploaded four RDG buffers again.

That is the opposite of a scalable robotics world.

## CVX-004B
The GPU field is now persistent and bounded.

Fixed hot cache:
- 4096 x 64 mm pages
- 8192-entry open-address page hash
- 64 explicit 16 mm chunks per page
- 4 packed 2-bit state words per chunk
- fixed exact uint64 occupancy mask for each 4 mm block
- ~132 MB maximum GPU field allocation

Untouched pages are not allocated. A missing page in the hash means the current proof
world's implicit-solid base.

## Edit path
CPU sparse edit
 -> CVX4 changed-page/chunk packet
 -> page-slot allocation on CPU
 -> repeated same-chunk updates coalesced
 -> one small RDG upload per frame
 -> compute shader patches persistent GPU buffers
 -> direct pixel traversal sees new truth

No full-world upload and no whole-field snapshot rebuild.

## Large world
Do NOT create the 128 m generator in this build yet. The new fixed hash/residency cache
is the prerequisite. CVX-004C can put a deterministic procedural base world behind this
cache without allocating a page table for the whole world.

## Next optimization after this gate
CVX-004C:
- deterministic 128 x 128 x 32 m training world
- int64 mm world coordinates
- procedural base classification
- camera/robot-centred hot residency
- page frustum culling
- 8x8 screen tiles
- later Hi-Z and distance skipping
