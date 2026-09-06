# CICADA Voxel CVX-004A — Robotics Direct Field

This is an architectural break from CVX-002/003 reference rendering.

## Main bug removed by construction

Robotics game-world mode does not use:
- HISM outer shell
- ISM hot blocks
- ISM 1 mm cubes
- DynamicMesh 16 mm chunks
- DynamicMesh 64 mm pages

Those representations cannot flicker or disagree with edits because they are not present.

The visible diagnostic pass ray-intersects the sparse authoritative field directly:

    64 mm page
      -> 16 mm chunk
        -> 4 mm block
          -> exact 64-bit 1 mm occupancy only when MIXED

Untouched solid space stays implicit.

## Robotics principle

The material field is the ground truth. Human preview and future simulated sensors consume
the same structure. Cold Nanite geometry is a cache/visualization representation, never the
authority for active edits or training labels.

## Current bootstrap limitation

CVX-004A rebuilds a compact CPU snapshot and uploads RDG structured buffers for the direct
view. It coalesces edits to 60 Hz by default. The next step is persistent GPU page residency
and compact in-place delta application so unchanged pages are not re-uploaded.

This is intentionally a correctness-first direct field renderer, replacing the broken mesh
handoff before large procedural world streaming is layered on top.

## Test

Use:
- Use Robotics Direct Field Renderer = ON
- Disable Legacy Runtime Visuals = ON
- Snapshot Hz = 60
- Throughput First Editing = ON
- Beam = ON

The runtime overlay should contain:

    CVX-004A ROBOTICS FIELD
    legacySkin=OFF
    fullWorldUploads=0

The normal Unreal scene is copied first; the exact sparse field is alpha-overlaid directly by
the global pixel shader. There is no runtime outer-skin mesh to flicker.


## CVX-004A R2 compile repair

Forge diagnostics confirmed that the new CicadaVoxelRender module, global shader,
and SceneViewExtension all compiled and linked successfully under UE 5.8.2.

The remaining game-module compile failure was:

    TArray<uint32>::AddInit

UE 5.8.2 TArray does not expose AddInit. R2 appends the 64 page-map entries using
AddUninitialized() and explicitly fills them with the implicit-full sentinel.

No direct-field renderer architecture was rolled back.


## CVX-004A R3 — deterministic shader bootstrap

Forge diagnostics from R2 confirmed the C++ build completed, including the
render module, shader registration translation unit, and SceneViewExtension.
UE then crashed at startup because ShaderCore could not resolve:

    /Plugin/CicadaVoxel/Private/CVXRoboticsField.usf

R3 removes module-order timing from the equation.

Startup order is now:

    CicadaVoxelShaderBootstrap
    LoadingPhase=PostConfigInit
        |
        +-- verify Plugin/Shaders exists
        +-- verify CVXRoboticsField.usf physically exists
        +-- AddShaderSourceDirectoryMapping("/Plugin/CicadaVoxel", ...)
        +-- GetShaderSourceFilePath() validation
        |
        +-- explicitly LoadModuleChecked("CicadaVoxelRender")
                |
                +-- global shader registration
                +-- SceneViewExtension startup

CicadaVoxelRender has LoadingPhase=None and therefore cannot auto-load before
the bootstrap mapping. The gameplay/data module remains LoadingPhase=Default.

The direct 64/16/4/1 mm robotics field architecture is otherwise unchanged.
