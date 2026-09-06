// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "CicadaVoxelVolumeActor.h"

#ifdef CICADAVOXEL_CicadaVoxelVolumeActor_generated_h
#error "CicadaVoxelVolumeActor.generated.h already included, missing '#pragma once' in CicadaVoxelVolumeActor.h"
#endif
#define CICADAVOXEL_CicadaVoxelVolumeActor_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class ACicadaVoxelVolumeActor **************************************************
#define FID_Users_thoma_Documents_Unreal_Projects_MmmVoxels_Plugins_CicadaVoxel_Source_CicadaVoxel_Public_CicadaVoxelVolumeActor_h_27_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetStatsString); \
	DECLARE_FUNCTION(execIsSolidLocalMM); \
	DECLARE_FUNCTION(execCarveSphereLocalMM); \
	DECLARE_FUNCTION(execCarveVoxelLocalMM); \
	DECLARE_FUNCTION(execRebuildVisualization); \
	DECLARE_FUNCTION(execResetProof); \
	DECLARE_FUNCTION(execRun1mmWitnessProof); \
	DECLARE_FUNCTION(execRunDefaultProof);


struct Z_Construct_UClass_ACicadaVoxelVolumeActor_Statics;
CICADAVOXEL_API UClass* Z_Construct_UClass_ACicadaVoxelVolumeActor(ETypeConstructPhase);

#define FID_Users_thoma_Documents_Unreal_Projects_MmmVoxels_Plugins_CicadaVoxel_Source_CicadaVoxel_Public_CicadaVoxelVolumeActor_h_27_INCLASS_NO_PURE_DECLS \
private: \
	friend struct ::Z_Construct_UClass_ACicadaVoxelVolumeActor_Statics; \
	friend CICADAVOXEL_API UClass* ::Z_Construct_UClass_ACicadaVoxelVolumeActor(ETypeConstructPhase); \
public: \
	DECLARE_CLASS2(ACicadaVoxelVolumeActor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/CicadaVoxel"), Z_Construct_UClass_ACicadaVoxelVolumeActor) \
	DECLARE_SERIALIZER(ACicadaVoxelVolumeActor)


#define FID_Users_thoma_Documents_Unreal_Projects_MmmVoxels_Plugins_CicadaVoxel_Source_CicadaVoxel_Public_CicadaVoxelVolumeActor_h_27_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ACicadaVoxelVolumeActor(ACicadaVoxelVolumeActor&&) = delete; \
	ACicadaVoxelVolumeActor(const ACicadaVoxelVolumeActor&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ACicadaVoxelVolumeActor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ACicadaVoxelVolumeActor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ACicadaVoxelVolumeActor) \
	NO_API virtual ~ACicadaVoxelVolumeActor();


#define FID_Users_thoma_Documents_Unreal_Projects_MmmVoxels_Plugins_CicadaVoxel_Source_CicadaVoxel_Public_CicadaVoxelVolumeActor_h_24_PROLOG
#define FID_Users_thoma_Documents_Unreal_Projects_MmmVoxels_Plugins_CicadaVoxel_Source_CicadaVoxel_Public_CicadaVoxelVolumeActor_h_27_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_thoma_Documents_Unreal_Projects_MmmVoxels_Plugins_CicadaVoxel_Source_CicadaVoxel_Public_CicadaVoxelVolumeActor_h_27_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_thoma_Documents_Unreal_Projects_MmmVoxels_Plugins_CicadaVoxel_Source_CicadaVoxel_Public_CicadaVoxelVolumeActor_h_27_INCLASS_NO_PURE_DECLS \
	FID_Users_thoma_Documents_Unreal_Projects_MmmVoxels_Plugins_CicadaVoxel_Source_CicadaVoxel_Public_CicadaVoxelVolumeActor_h_27_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ACicadaVoxelVolumeActor;

// ********** End Class ACicadaVoxelVolumeActor ****************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_thoma_Documents_Unreal_Projects_MmmVoxels_Plugins_CicadaVoxel_Source_CicadaVoxel_Public_CicadaVoxelVolumeActor_h

// ********** Begin Enum ECVXEditorPreviewMode *****************************************************
#define FOREACH_ENUM_ECVXEDITORPREVIEWMODE(op) \
	op(ECVXEditorPreviewMode::None) \
	op(ECVXEditorPreviewMode::Witness1mm) \
	op(ECVXEditorPreviewMode::DefaultProof) 

enum class ECVXEditorPreviewMode : uint8;
template<> struct TIsUEnumClass<ECVXEditorPreviewMode> { enum { Value = true }; };
template<> UE_NODEBUG CICADAVOXEL_NON_ATTRIBUTED_API UEnum* StaticEnum<ECVXEditorPreviewMode>();
// ********** End Enum ECVXEditorPreviewMode *******************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
