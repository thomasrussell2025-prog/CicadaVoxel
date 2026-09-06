#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CVXSparseVolume.h"
#include "CVXVirtualPageSpine.h"
#include "CicadaVoxelVolumeActor.generated.h"

class UHierarchicalInstancedStaticMeshComponent;
class UInstancedStaticMeshComponent;
class UDynamicMeshComponent;
class USceneComponent;
class APlayerController;
struct FCVXSurfaceMeshData;

UENUM(BlueprintType)
enum class ECVXEditorPreviewMode : uint8
{
    None UMETA(DisplayName="None"),
    Witness1mm UMETA(DisplayName="1 mm Witness"),
    DefaultProof UMETA(DisplayName="Default Crater + 1 mm Witness")
};

UCLASS(BlueprintType, Blueprintable)
class CICADAVOXEL_API ACicadaVoxelVolumeActor : public AActor
{
    GENERATED_BODY()

public:
    ACicadaVoxelVolumeActor();

    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Editor Preview")
    bool bEnableEditorPreview = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Editor Preview", meta=(EditCondition="bEnableEditorPreview"))
    ECVXEditorPreviewMode EditorPreviewMode = ECVXEditorPreviewMode::Witness1mm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Editor Preview")
    FString LastEditorPreview;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Volume", meta=(ClampMin="16"))
    FIntVector LogicalSizeMM = FIntVector(1000, 1000, 1000);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Proof")
    FVector ProofCenterMM = FVector(18.0, 500.0, 500.0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Proof", meta=(ClampMin="0.5"))
    float ProofSphereRadiusMM = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Proof")
    bool bAutoRunProofOnBeginPlay = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|1mm Witness")
    bool bInclude1mmWitnessInDefaultProof = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|1mm Witness")
    FIntVector WitnessPatchOriginMM = FIntVector(0, 450, 450);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|1mm Witness", meta=(ClampMin="4", ClampMax="32"))
    int32 WitnessPatchSizeMM = 16;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Visual", meta=(ClampMin="0.55", ClampMax="1.0"))
    float FineVoxelVisualScale = 0.88f;

    // Editor witness can keep tiny diagnostic gaps, but gameplay surfaces should tile exactly.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Visual")
    bool bForceSeamlessRuntimeFineVoxels = true;

    // A Full 4 mm block adjacent to empty/mixed space is promoted to 64 exact 1 mm cells.
    // This prevents visible destruction from degrading to 4 mm cubes as a cavity grows.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Visual")
    bool bRefineExposedHotSurfacesTo1mm = true;

    // Fine ISM slots remain fixed for the temporary renderer, but fully occluded 1 mm cells
    // are hidden so only the actual exposed material skin contributes visible geometry.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Visual")
    bool bCullOccludedFineVoxels = true;

    // Keep a visible implicit-solid frontier around dirty chunks so the procedural interior
    // never looks hollow while the temporary instance renderer is still in use.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Visual", meta=(ClampMin="1", ClampMax="2"))
    int32 SolidFrontierDepthChunks = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Visual", meta=(ClampMin="1000", ClampMax="1000000"))
    int32 MaxVisualInstances = 180000;

    // CVX-002A hot edit path. Runtime edits mutate stable HISM slots instead of clearing and
    // recreating every instance. Work is spread across frames using a dirty 16 mm chunk queue.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Performance")
    bool bUseIncrementalVisualUpdates = true;

    // Legacy chunk-count cap retained as a safety ceiling. CVX-002B additionally uses a
    // millisecond budget and 4 mm dirty-block granularity.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Performance", meta=(ClampMin="1", ClampMax="64", AdvancedDisplay))
    int32 VisualChunkBudgetPerFrame = 8;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Performance", meta=(ClampMin="0.10", ClampMax="8.0"))
    float VisualUpdateBudgetMS = 1.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Performance", meta=(ClampMin="1", ClampMax="256"))
    int32 MaxVisualBlocksPerFrame = 48;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Performance", meta=(ClampMin="0", ClampMax="131072", AdvancedDisplay))
    int32 HotBlockReserveInstances = 8192;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Performance", meta=(ClampMin="0", ClampMax="262144", AdvancedDisplay))
    int32 HotFineReserveInstances = 65536;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Performance")
    bool bAsyncHISMTreeBuilds = true;

    // The old validation bridge serializes the entire dirty sparse set. It is not used by the
    // current visualizer, so keep it off during hot runtime edits. The future GPU page renderer
    // will publish deltas instead of rebuilding a full snapshot.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Performance", meta=(AdvancedDisplay))
    bool bPublishLegacyFullGPUSnapshot = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Performance", meta=(ClampMin="0.02", ClampMax="1.0"))
    float RuntimeStatsPublishInterval = 0.15f;

    // CVX-002F: runtime hot geometry is no longer represented by one Unreal instance per
    // 1 mm voxel. Each affected 16 mm chunk gets an exact surface mesh extracted from the
    // authoritative 1 mm field. This removes the coarse/hollow visual catch-up failure.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Exact Surface Mesh")
    bool bUseExactSurfaceMeshRuntime = true;

    // Merge adjacent coplanar 1 mm faces into rectangles. Geometry still changes only on
    // integer millimetre boundaries; this reduces triangle count without reducing truth resolution.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Exact Surface Mesh")
    bool bGreedySurfaceMeshing = true;

    // CVX-002G integrity guard: render exact hot surfaces from either side. This is
    // deliberately limited to the small dynamic hot meshlets while we validate final
    // triangle orientation before the custom GPU renderer replaces DynamicMesh.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Exact Surface Mesh")
    bool bTwoSidedExactSurface = true;

    // Tiny rapidly changing 1 mm cavity faces produce noisy shadow-map aliasing and add
    // no useful structural information. Cold/coarse geometry continues to cast shadows.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Exact Surface Mesh")
    bool bExactSurfaceShadows = false;

    // Keep the cold 16 mm proxy until a valid exact mesh has been installed. This prevents
    // the temporary 16 mm square holes seen when a queued neighbour was hidden before its
    // replacement render proxy was ready.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Exact Surface Mesh")
    bool bSafeSurfaceHandoff = true;

    // CVX-002H: exact runtime uses a plain ISM cold shell, never HISM, so a chunk has
    // exactly one render owner during destruction. This removes HISM tree lag / ghost
    // rectangles where an old coarse proxy and a new exact mesh compete for the same area.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Exact Surface Mesh")
    bool bSingleOwnerRuntimeSurface = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Exact Surface Mesh")
    int32 SurfaceProxyRetirements = 0;

    // Rebuild the impact chunk plus six face neighbours immediately. The edited surface
    // therefore crosses 16 mm chunk boundaries as one coherent local patch instead of
    // visibly waiting for neighbour chunks over several frames.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Exact Surface Mesh")
    bool bImmediateImpactNeighbourMeshes = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Exact Surface Mesh", meta=(ClampMin="0.10", ClampMax="8.0"))
    float SurfaceMeshBudgetMS = 1.50f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Exact Surface Mesh", meta=(ClampMin="1", ClampMax="32"))
    int32 MaxSurfaceMeshChunksPerFrame = 3;

    // Rebuild the chunk under the beam immediately; neighbours remain budgeted.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Exact Surface Mesh")
    bool bImmediateImpactSurfaceMesh = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Exact Surface Mesh", meta=(ClampMin="0", ClampMax="256"))
    int32 BeamSurfaceMeshBackpressureChunks = 32;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Exact Surface Mesh")
    float LastSurfaceMeshUpdateMS = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Exact Surface Mesh")
    int32 PendingSurfaceMeshChunkCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Exact Surface Mesh")
    int32 LastSurfaceMeshChunksProcessed = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Exact Surface Mesh")
    int32 LastSurfaceMeshQuads = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Exact Surface Mesh")
    int32 LastSurfaceMeshTriangles = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Exact Surface Mesh")
    int32 ActiveSurfaceMeshChunks = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Exact Surface Mesh")
    int32 SurfaceHandoffGuards = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Exact Surface Mesh")
    int32 SurfaceImmediateNeighbourRebuilds = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Performance")
    float LastSparseEditMS = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Performance")
    float LastVisualUpdateMS = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Performance")
    float LastFullVisualRebuildMS = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Performance")
    int32 PendingVisualChunkCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Performance")
    int32 PendingVisualBlockCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Performance")
    int32 LastVisualBlocksProcessed = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Performance")
    int32 LastRefinedSurfaceBlocksProcessed = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Performance")
    int32 FineSlotBatchesReused = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Performance")
    float LastLegacyGPUSnapshotMS = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Performance")
    FString LastPerformanceStats;

    // CVX-002D: virtual 64 mm hot-page spine. This tracks only edited sparse pages and
    // estimates the exact compact upload payload that the custom GPU renderer will consume.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Virtual Pages")
    bool bEnableVirtualPageSpine = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Virtual Pages")
    int32 VirtualPageEdgeMM = FCVXVirtualPageSpine::PageEdgeMM;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Virtual Pages")
    int32 ResidentEditedPageCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Virtual Pages")
    int32 ExplicitPageChunkCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Virtual Pages")
    int64 LastPageDeltaBytes = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Virtual Pages")
    int64 FullWorldHotUploadCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Virtual Pages")
    FString LastVirtualPageStats;

    // CVX-003A active runtime bridge. Hot rendering is owned by 64 mm pages rather than
    // hundreds of 16 mm DynamicMesh components. The old chunk renderer remains only as an
    // opt-in reference path for comparison while the custom GPU ray renderer is brought online.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003A Page Runtime")
    bool bUsePageNativeRuntime = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003A Page Runtime", meta=(ClampMin="1", ClampMax="16"))
    int32 MaxSurfacePagesPerFrame = 8;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003A Page Runtime", meta=(ClampMin="0.25", ClampMax="16.0"))
    float SurfacePageBudgetMS = 4.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003C Realtime Surface")
    bool bAsyncSurfacePageMeshing = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003C Realtime Surface", meta=(ClampMin="1", ClampMax="8"))
    int32 MaxAsyncSurfacePageJobs = 3;

    // Outer world boundary is a tiny 2D problem compared with remeshing an entire
    // 64 mm volume page. Update touched exterior pages synchronously so the player
    // sees the hole in the same frame; complete cavity geometry is built asynchronously.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003C Realtime Surface")
    bool bRealtimeExteriorSurface = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003C Realtime Surface", meta=(ClampMin="1", ClampMax="16"))
    int32 MaxRealtimeExteriorPagesPerFrame = 6;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003C Realtime Surface", meta=(ClampMin="0.1", ClampMax="8.0"))
    float RealtimeExteriorBudgetMS = 2.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003C Realtime Surface")
    int32 AsyncSurfacePageJobsInFlight = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003C Realtime Surface")
    int32 AsyncSurfacePagesCompleted = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003C Realtime Surface")
    int32 AsyncSurfaceStaleResults = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003C Realtime Surface")
    float LastAsyncSurfaceWorkerMS = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003C Realtime Surface")
    float LastExteriorSurfaceMS = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003C Realtime Surface")
    int32 LastExteriorPagesUpdated = 0;

    // CVX-003D: the DynamicMesh page renderer is now explicitly a bounded reference cache.
    // It is not allowed to consume VRAM in proportion to the total amount of destruction.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003D Reference Cache")
    bool bEnableReferenceSurfaceMemoryGovernor = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003D Reference Cache", meta=(ClampMin="4", ClampMax="256"))
    int32 MaxResidentReferenceSurfacePages = 32;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003D Reference Cache", meta=(ClampMin="16.0", ClampMax="1024.0"))
    float ReferenceSurfaceBudgetMB = 192.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003D Reference Cache", meta=(ClampMin="1000", ClampMax="250000"))
    int32 MaxReferencePageQuads = 60000;

    // Replacing DynamicMesh topology every frame can temporarily retain old render buffers
    // until the render thread/RHI retires them. Cap how often one exterior page is rebuilt.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003D Reference Cache", meta=(ClampMin="2.0", ClampMax="60.0"))
    float RealtimeExteriorMaxHzPerPage = 12.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003D Reference Cache")
    int32 ReferenceSurfaceEvictions = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003D Reference Cache")
    int32 ReferenceSurfaceComplexityRejects = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003D Reference Cache")
    float EstimatedReferenceSurfaceMB = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003D Reference Cache")
    int32 ResidentReferenceSurfacePages = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003A Page Runtime", meta=(AdvancedDisplay))
    bool bKeepLegacyChunkSurfaceReference = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003A Page Runtime")
    int32 PendingSurfacePageCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003A Page Runtime")
    int32 LastSurfacePagesProcessed = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003A Page Runtime")
    int32 ActiveSurfacePageCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003A Page Runtime")
    float LastSurfacePageMS = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003A Page Runtime")
    int32 LastSurfacePageQuads = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003A GPU Delta")
    bool bUploadChangedPagesToGPU = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003A GPU Delta")
    int64 LastActualGPUDeltaBytes = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003A GPU Delta")
    int64 TotalActualGPUDeltaBytes = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003A GPU Delta")
    int32 GPUDeltaPacketCount = 0;

    // CVX-003B: destruction throughput is now treated as a data problem first. Large
    // subtract stamps classify 64 mm pages -> 16 mm chunks -> 4 mm blocks and only
    // touch individual 1 mm bits on the geometric boundary.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003B Bulk Edit")
    bool bUseHierarchicalBulkEdits = true;

    // Let authoritative edits run at full speed even when the temporary CPU reference
    // surface cannot visually catch up. The GPU renderer is the real consumer next.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003B Bulk Edit")
    bool bThroughputFirstEditing = true;

    // While the beam is held, do not spend frame time rebuilding the old DynamicMesh
    // reference pages. Queue invalidations and catch the reference view up after release.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003B Bulk Edit")
    bool bDeferReferenceSurfaceWhileBeam = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003B Bulk Edit", meta=(ClampMin="1", ClampMax="16"))
    int32 MaxBeamCarvesPerFrame = 8;

    // Throughput mode never runs the beam slower than this even if an old placed actor
    // serialized a lower BeamEditsPerSecond value from an earlier plugin build.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003B Bulk Edit", meta=(ClampMin="30.0", ClampMax="1000.0"))
    float ThroughputBeamMinEditsPerSecond = 180.0f;

    // Press B while aiming at the volume for a large one-shot edit benchmark.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|003B Bulk Edit", meta=(ClampMin="10.0", ClampMax="2000.0"))
    float MacroBlastRadiusMM = 250.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003B Bulk Edit")
    int32 MacroBlastCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003B Bulk Edit")
    int32 LastBulkPagesCleared = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003B Bulk Edit")
    int32 LastBulkChunksCleared = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003B Bulk Edit")
    int32 LastBulkBlocksCleared = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003B Bulk Edit")
    int32 LastBulkBoundaryBlocks = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003B Bulk Edit")
    int32 LastBulkSubBlocks2mmCleared = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003B Bulk Edit")
    int64 LastBulkFineVoxelTests = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|003B Bulk Edit")
    int32 LastBeamCarvesThisFrame = 0;

    // CVX-004A ROBOTICS/TRAINING MODE
    //
    // The active game-world visual now ray-intersects the same sparse material field
    // used by edits. HISM/ISM/DynamicMesh surfaces are disabled, so there is no
    // separate "outer skin" that can flicker or disagree with sensor truth.
    // R4 FORCE DIRECT: these are diagnostics only. Game/PIE worlds always use the
    // direct material-field renderer and always disable the retired runtime skin.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|004A DIRECT FIELD - FORCED")
    bool bUseRoboticsDirectFieldRenderer = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|004A DIRECT FIELD - FORCED")
    bool bDisableLegacyRuntimeVisualsInRoboticsMode = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|004A DIRECT FIELD - FORCED")
    FString DirectFieldRuntimeMode = TEXT("FORCED IN PLAY: 64mm -> 16mm -> 4mm -> exact 1mm; legacy skin disabled");

    // Snapshot coalescing is deliberately frame-like: hundreds of authoritative beam
    // edits can occur per second while the renderer consumes the newest stable field
    // generation. CVX-004B replaces this bootstrap snapshot with persistent GPU deltas.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|004A DIRECT FIELD - FORCED", meta=(ClampMin="10.0", ClampMax="240.0"))
    float RoboticsFieldSnapshotHz = 60.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|004A DIRECT FIELD - FORCED")
    float LastRoboticsFieldSnapshotMS = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|004A DIRECT FIELD - FORCED")
    int64 LastRoboticsFieldSnapshotBytes = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|004A DIRECT FIELD - FORCED")
    int32 RoboticsFieldPartialPages = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|004A DIRECT FIELD - FORCED")
    int32 RoboticsFieldExplicitChunks = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|004A DIRECT FIELD - FORCED")
    int32 RoboticsFieldMixedBlocks = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|004A DIRECT FIELD - FORCED")
    FString LastRoboticsFieldStats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Interaction")
    bool bEnableFirstPersonCarving = true;

    // Hold LMB for a continuous digging beam. The beam is deliberately rate-limited so
    // voxel truth cannot outrun the temporary visualizer by hundreds of dirty blocks.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Interaction")
    bool bContinuousBeamDigging = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Interaction", meta=(ClampMin="1.0", ClampMax="1000.0"))
    float BeamEditsPerSecond = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Interaction", meta=(ClampMin="0.5", ClampMax="500.0"))
    float BeamCarveRadiusMM = 60.0f;

    // Temporary visualizer guard. The authoritative edit is cheap, but allowing it to run
    // hundreds of blocks ahead is what produced invisible interiors / coarse catch-up.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Interaction", meta=(ClampMin="0", ClampMax="4096"))
    int32 BeamVisualBackpressureBlocks = 192;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Interaction")
    bool bUseHierarchicalRayTraversal = true;

    // Refresh the impact block plus six face neighbours immediately. Bulk visual work remains
    // time-budgeted, but the point the player is drilling should never sit behind a long queue.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Interaction")
    bool bImmediateImpactSurfaceRefresh = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Interaction", meta=(ClampMin="0.5", ClampMax="100.0"))
    float FirstPersonCarveRadiusMM = 12.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CVX|Interaction", meta=(ClampMin="100.0"))
    float FirstPersonMaxDistanceCM = 5000.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Interaction")
    float LastRayQueryMS = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Interaction")
    int32 LastRayChunkSkips16mm = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Interaction")
    int32 LastRayBlockSkips4mm = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Interaction")
    int32 LastRayFineSteps1mm = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Interaction")
    int32 BeamThrottleFrames = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Interaction")
    int32 BeamCarvesThisHold = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Performance")
    float LastImmediateSurfaceMS = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Stats")
    FString LastStats;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Stats")
    FString LastVisualStats;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|1mm Witness")
    FString Last1mmWitness;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Visual")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Visual")
    TObjectPtr<UHierarchicalInstancedStaticMeshComponent> CoarseChunkInstances;

    // Runtime exact-surface cold shell. Plain ISM is intentional: edited chunks are
    // retired once and never need a HISM cluster-tree rebuild or stale cluster visibility.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Visual")
    TObjectPtr<UInstancedStaticMeshComponent> ExactColdShellInstances;

    // Untouched implicit-solid chunks immediately around the edited region. Plain ISM is
    // intentional: these chunks change membership during destruction and must not wait for
    // a HISM cluster-tree rebuild before becoming visible.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Visual")
    TObjectPtr<UInstancedStaticMeshComponent> FrontierChunkInstances;

    // Hot geometry intentionally uses plain ISM rather than HISM. UE's HISM hierarchy is
    // excellent for thousands of static instances; repeatedly changing its tree is exactly the
    // wrong workload for destructive 4 mm / 1 mm cells.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Visual")
    TObjectPtr<UInstancedStaticMeshComponent> FullBlockInstances;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CVX|Visual")
    TObjectPtr<UInstancedStaticMeshComponent> FineVoxelInstances;

    // Runtime-only exact-surface meshlets. One component per active 16 mm hot chunk keeps
    // topology rebuilds bounded to tiny local regions instead of invalidating one giant mesh.
    UPROPERTY(Transient)
    TArray<TObjectPtr<UDynamicMeshComponent>> SurfaceMeshComponents;


    // CVX-003A: one exact reference mesh per 64 mm hot page. This is the final CPU
    // reference renderer before the page data is consumed directly by the GPU ray path.
    UPROPERTY(Transient)
    TArray<TObjectPtr<UDynamicMeshComponent>> SurfacePageComponents;

    UFUNCTION(CallInEditor, BlueprintCallable, Category="CVX|Proof")
    void RunDefaultProof();

    UFUNCTION(CallInEditor, BlueprintCallable, Category="CVX|1mm Witness")
    void Run1mmWitnessProof();

    UFUNCTION(CallInEditor, BlueprintCallable, Category="CVX|Proof")
    void ResetProof();

    UFUNCTION(CallInEditor, BlueprintCallable, Category="CVX|Visual")
    void RebuildVisualization();

    UFUNCTION(BlueprintCallable, Category="CVX|Edit")
    bool CarveVoxelLocalMM(FIntVector VoxelMM);

    UFUNCTION(BlueprintCallable, Category="CVX|Edit")
    void CarveSphereLocalMM(FVector CenterMM, float RadiusMM);

    UFUNCTION(BlueprintPure, Category="CVX|Query")
    bool IsSolidLocalMM(FIntVector VoxelMM) const;

    UFUNCTION(BlueprintPure, Category="CVX|Stats")
    FString GetStatsString() const;

private:
    struct FCVXChunkVisualSlots
    {
        TArray<int32> BlockIndices;
        TArray<TArray<int32>> FineIndicesByBlock;
    };

    void EnsureInitialized();
    void RefreshStatsAndPublish();
    void RebuildVirtualPageSpine();
    void ApplyVirtualPageChanges(const TArray<FCVXChangedBlock>& ChangedBlocks);
    void ApplyVirtualPageChanges(const FCVXEditDelta& Delta);
    void ScheduleRuntimeStatsPublish();
    void FlushRuntimeStatsPublish(float DeltaSeconds);
    int32 Apply1mmWitnessPattern();
    bool TryCarveFromPlayerView(float DeltaSeconds);
    bool CarveAtCurrentViewRay(APlayerController* PlayerController, float RadiusMM);
    void RefreshImpactSurfaceNow(const FVector& CenterMM);
    bool SegmentBoxEntry(const FVector& SegmentStart, const FVector& SegmentEnd, const FBox& Box, FVector& OutEntry) const;

    FTransform MakeVisualBoxTransform(const FVector& MinMM, const FVector& MaxMM) const;
    FTransform MakeHiddenTransform(const FVector& CenterMM) const;
    void AddVisualBox(TArray<FTransform>& OutTransforms, const FVector& MinMM, const FVector& MaxMM, int32& InOutCount);
    bool IsBoundaryChunk(const FIntVector& ChunkCoord, const FIntVector& ChunkCounts) const;
    bool IsChunkCoordInBounds(const FIntVector& ChunkCoord) const;
    FTransform MakeCoarseChunkTransform(const FIntVector& ChunkCoord, bool bVisible) const;
    FTransform BuildBlockSlotTransform(const FIntVector& ChunkCoord, int32 BlockIndex) const;
    void BuildBlockSlotTransforms(const FIntVector& ChunkCoord, TArray<FTransform>& OutTransforms) const;
    void BuildFineSlotTransforms(const FIntVector& ChunkCoord, int32 BlockIndex, uint64 SolidMask, TArray<FTransform>& OutTransforms) const;
    bool ShouldRefineFullBlockTo1mm(const FIntVector& ChunkCoord, int32 BlockIndex) const;
    bool GetNeighbourBlockAddress(const FIntVector& ChunkCoord, int32 BlockIndex, const FIntVector& BlockDelta, FIntVector& OutChunkCoord, int32& OutBlockIndex) const;
    bool AreIndicesContiguous(const TArray<int32>& Indices) const;
    bool UpdateSlotTransforms(UInstancedStaticMeshComponent* Component, const TArray<int32>& Indices, const TArray<FTransform>& Transforms, bool bMarkRenderStateDirty);
    bool AllocateSlotBatch(UInstancedStaticMeshComponent* Component, const TArray<FTransform>& Transforms, TArray<int32>& OutIndices);
    bool AcquireFineSlotBatch(const TArray<FTransform>& Transforms, TArray<int32>& OutIndices, bool& bTouchedFine);
    void ReleaseFineSlotBatch(const FIntVector& ChunkCoord, int32 BlockIndex, TArray<int32>& FineIndices, bool& bTouchedFine);

    void ResetVisualSlotCaches();
    UInstancedStaticMeshComponent* GetActiveColdShellComponent() const;
    void FlushActiveColdShellChanges(bool bTouchedColdShell);
    void EnsureCoarseChunkVisual(const FIntVector& ChunkCoord, bool bVisible, bool& bTouchedCoarse);
    void EnsureFrontierChunkVisual(const FIntVector& ChunkCoord, bool bVisible, bool& bTouchedFrontier);
    void EnsureSolidFrontierAroundChunk(const FIntVector& ChunkCoord, bool& bTouchedFrontier);
    bool EnsureHotChunkSlots(const FIntVector& ChunkCoord, bool& bTouchedBlocks);
    void UpdateHotBlockVisual(const FIntVector& ChunkCoord, int32 BlockIndex, bool& bTouchedBlocks, bool& bTouchedFine);
    void UpdateHotChunkVisual(const FIntVector& ChunkCoord, bool& bTouchedBlocks, bool& bTouchedFine);
    void QueueVisualBlock(const FIntVector& ChunkCoord, int32 BlockIndex);
    void QueueVisualBlockAndSurfaceNeighbours(const FIntVector& ChunkCoord, int32 BlockIndex);
    void QueueVisualBlocks(const TArray<FCVXChangedBlock>& ChangedBlocks);
    void QueueVisualChunk(const FIntVector& ChunkCoord);
    void QueueVisualChunks(const TArray<FIntVector>& ChunkCoords);
    void ProcessPendingVisualUpdates();
    void KickTreeBuild(UHierarchicalInstancedStaticMeshComponent* Component, bool bForceUpdate);
    void MaintainAsyncVisualTrees();
    void UpdatePerformanceString(int32 ProcessedChunks);
    void ShowRuntimeDebugMessage();

    bool IsRoboticsDirectFieldRuntimeActive() const;
    void DisableLegacyRuntimeVisualsForRobotics();
    void MarkRoboticsFieldDirty();
    void FlushRoboticsFieldSnapshot(float DeltaSeconds, bool bForce = false);
    void SubmitRoboticsFieldSnapshot();
    void SubmitPersistentGPUStateBootstrap();
    TSharedPtr<const struct FCVXRoboticsFieldSnapshot, ESPMode::ThreadSafe> BuildRoboticsFieldSnapshot();

    bool IsExactSurfaceRuntimeActive() const;
    void ClearSurfaceMeshRuntime();
    void QueueSurfaceMeshChunk(const FIntVector& ChunkCoord);
    void QueueSurfaceMeshChunkAndNeighbours(const FIntVector& ChunkCoord);
    void QueueSurfaceMeshFromChangedBlocks(const TArray<FCVXChangedBlock>& ChangedBlocks);
    void QueueSurfaceMeshFromDirtyChunks();
    void ProcessPendingSurfaceMeshUpdates();
    void RebuildSurfaceMeshChunk(const FIntVector& ChunkCoord);
    void RebuildImpactSurfaceMeshNow(const FVector& CenterMM);
    void ApplyExactSurfaceComponentSettings(UDynamicMeshComponent* Component);
    UDynamicMeshComponent* GetOrCreateSurfaceMeshComponent(const FIntVector& ChunkCoord);
    void UpdateSurfaceMeshStats();

    bool IsPageNativeRuntimeActive() const;
    void ClearPageSurfaceRuntime();
    FIntVector ChunkToSurfacePage(const FIntVector& ChunkCoord) const;
    bool IsSurfacePageInBounds(const FIntVector& PageCoord) const;
    void QueueSurfacePage(const FIntVector& PageCoord);
    void QueueSurfacePagesFromChangedBlocks(const TArray<FCVXChangedBlock>& ChangedBlocks);
    void QueueSurfacePagesFromEditDelta(const FCVXEditDelta& Delta);
    void QueueSurfacePagesFromDirtyChunks();
    UDynamicMeshComponent* GetOrCreateSurfacePageComponent(const FIntVector& PageCoord);
    void RebuildSurfacePage(const FIntVector& PageCoord);
    void ApplySurfacePageMesh(const FIntVector& PageCoord, FCVXSurfaceMeshData&& MeshData);
    void RebuildExteriorSurfacePageNow(const FIntVector& PageCoord);
    void QueueExteriorSurfacePage(const FIntVector& PageCoord);
    void QueueExteriorSurfacePagesFromChangedBlocks(const TArray<FCVXChangedBlock>& ChangedBlocks);
    void QueueExteriorSurfacePagesFromEditDelta(const FCVXEditDelta& Delta);
    void ProcessPendingExteriorSurfaceUpdates();
    bool IsSurfacePageOnWorldBoundary(const FIntVector& PageCoord) const;
    void LaunchAsyncSurfacePageJob(const FIntVector& PageCoord, uint32 Generation);
    void ProcessPendingSurfacePageUpdates();
    void UpdateSurfacePageStats();
    void RetireColdChunksForPage(const FIntVector& PageCoord);
    void RestoreColdChunksForPage(const FIntVector& PageCoord);
    void TouchReferenceSurfacePage(const FIntVector& PageCoord);
    void EnforceReferenceSurfaceBudget();
    bool EvictReferenceSurfacePage(const FIntVector& PageCoord);
    int64 EstimateReferenceSurfaceBytes(const FCVXSurfaceMeshData& MeshData) const;
    void SubmitGPUPageDelta(const TArray<FCVXChangedBlock>& ChangedBlocks);
    void SubmitGPUPageDelta(const FCVXEditDelta& Delta);

#if WITH_EDITOR
    uint32 BuildEditorPreviewSignature() const;
    void ApplyEditorPreview();
    uint32 LastEditorPreviewSignature = 0;
    bool bHasEditorPreviewSignature = false;
#endif

    float BeamEditAccumulator = 0.0f;
    bool bBeamWasDownLastFrame = false;

    bool bInitialized = false;
    bool bRuntimeStatsPublishPending = false;
    float RuntimeStatsPublishAccumulator = 0.0f;
    int32 VisualAllocatedSlots = 0;
    bool bVisualBudgetHit = false;

    TMap<FIntVector, int32> CoarseInstanceByChunk;
    TMap<FIntVector, int32> FrontierInstanceByChunk;
    TMap<FIntVector, FCVXChunkVisualSlots> HotChunkVisualSlots;
    TArray<TArray<int32>> FreeFineSlotBatches;
    TArray<FIntVector> PendingVisualChunkQueue;
    int32 PendingVisualChunkReadIndex = 0;
    TSet<FIntVector> PendingVisualChunkSet;
    TMap<FIntVector, uint64> PendingVisualBlockMasks;
    bool bCoarseTreeBuildPending = false;

    TMap<FIntVector, int32> SurfaceMeshComponentByChunk;
    TMap<FIntVector, int32> SurfaceMeshQuadsByChunk;
    TMap<FIntVector, int32> SurfaceMeshTrianglesByChunk;
    TArray<FIntVector> PendingSurfaceMeshQueue;
    int32 PendingSurfaceMeshReadIndex = 0;
    TSet<FIntVector> PendingSurfaceMeshSet;
    int64 TotalSurfaceMeshQuads = 0;
    int64 TotalSurfaceMeshTriangles = 0;

    TMap<FIntVector, int32> SurfacePageComponentByCoord;
    TMap<FIntVector, int32> SurfacePageQuadsByCoord;
    TArray<FIntVector> PendingSurfacePageQueue;
    int32 PendingSurfacePageReadIndex = 0;
    TSet<FIntVector> PendingSurfacePageSet;
    TSet<FIntVector> SurfacePageJobsInFlightSet;
    TSet<FIntVector> SurfacePageRerunSet;
    TMap<FIntVector, uint32> SurfacePageGeneration;
    TSet<FIntVector> PendingExteriorSurfacePageSet;

    // Bounded reference-renderer residency. World truth is not evicted; only temporary
    // CPU/DynamicMesh visualization pages are.
    TMap<FIntVector, uint64> SurfacePageLastTouchSerial;
    TMap<FIntVector, int64> SurfacePageEstimatedBytesByCoord;
    TMap<FIntVector, double> SurfacePageLastExteriorBuildSeconds;
    uint64 SurfacePageTouchSerial = 0;
    int64 EstimatedReferenceSurfaceBytes = 0;

    bool bRoboticsFieldDirty = true;
    float RoboticsFieldSnapshotAccumulator = 0.0f;

    FCVXSparseVolume Volume;
    FCVXVirtualPageSpine VirtualPageSpine;
};
