# CVX-004A R4 R2 FORCE DIRECT

## Fix 1 — current Forge compile failure
Removed the unnecessary `TArray<FShaderCompilerError>` / `GetShaderSourceFilePath`
validation from the PostConfigInit bootstrap. UE 5.8 saw FShaderCompilerError as an
incomplete type in that translation unit.

The bootstrap still:
1. finds the CicadaVoxel plugin,
2. verifies the physical Shaders folder,
3. verifies `Shaders/Private/CVXRoboticsField.usf`,
4. maps `/Plugin/CicadaVoxel`,
5. explicitly loads `CicadaVoxelRender`.

## Fix 2 — latent startup ensure from previous logs
Do not create a SceneViewExtension at PostConfigInit. GEngine is not ready then.

Startup is now:

PostConfigInit:
    shader directory mapping
    load CicadaVoxelRender
    global shader registration
    register PostEngineInit callback

PostEngineInit:
    create FCVXRoboticsFieldViewExtension

## Runtime invariant
R4 forced-direct behavior is unchanged:
- PIE/game world uses direct sparse field
- legacy HISM/ISM/DynamicMesh runtime skin is hard-disabled

Expected green message:
    CVX-004A R4 DIRECT FIELD ACTIVE | LEGACY SKIN HARD OFF

Expected log:
    CVX-004A R4 R2 PostEngineInit: DIRECT FIELD VIEW EXTENSION ACTIVE
