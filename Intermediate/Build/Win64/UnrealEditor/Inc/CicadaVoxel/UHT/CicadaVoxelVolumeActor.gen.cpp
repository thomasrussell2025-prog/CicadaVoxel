// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CicadaVoxelVolumeActor.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_UOBJECT");
void EmptyLinkFunctionForGeneratedCodeCicadaVoxelVolumeActor() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FIntVector(ETypeConstructPhase);
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector(ETypeConstructPhase);
ENGINE_API UClass* Z_Construct_UClass_AActor(ETypeConstructPhase);
ENGINE_API UClass* Z_Construct_UClass_UHierarchicalInstancedStaticMeshComponent(ETypeConstructPhase);
ENGINE_API UClass* Z_Construct_UClass_UInstancedStaticMeshComponent(ETypeConstructPhase);
ENGINE_API UClass* Z_Construct_UClass_USceneComponent(ETypeConstructPhase);
GEOMETRYFRAMEWORK_API UClass* Z_Construct_UClass_UDynamicMeshComponent(ETypeConstructPhase);
// ********** End Cross Module References **********************************************************

// ********** Begin Same Module References *********************************************************
UPackage* Z_Construct_UPackage__Script_CicadaVoxel(ETypeConstructPhase);
CICADAVOXEL_API UClass* Z_Construct_UClass_ACicadaVoxelVolumeActor(ETypeConstructPhase);
CICADAVOXEL_API UEnum* Z_Construct_UEnum_CicadaVoxel_ECVXEditorPreviewMode(ETypeConstructPhase);
CICADAVOXEL_API UClass* Z_Construct_UClass_ACicadaVoxelVolumeActor(ETypeConstructPhase);
// ********** End Same Module References ***********************************************************
#define UHT_STRUCT_BASE(INIT) UE::CodeGen::ConstInit::TCompiledInObjectPtr<const FStructBaseChain>(UE::Private::AsStructBaseChain(INIT))

// ********** Begin Enum ECVXEditorPreviewMode *****************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UEnum_CicadaVoxel_ECVXEditorPreviewMode_Statics
template<> CICADAVOXEL_NON_ATTRIBUTED_API UEnum* StaticEnum<ECVXEditorPreviewMode>()
{
	return Z_Construct_UEnum_CicadaVoxel_ECVXEditorPreviewMode(ETypeConstructPhase::Outer);
}
struct UHT_STATICS
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "BlueprintType", "true" },
		{ "DefaultProof.DisplayName", "Default Crater + 1 mm Witness" },
		{ "DefaultProof.Name", "ECVXEditorPreviewMode::DefaultProof" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
		{ "None.DisplayName", "None" },
		{ "None.Name", "ECVXEditorPreviewMode::None" },
		{ "Witness1mm.DisplayName", "1 mm Witness" },
		{ "Witness1mm.Name", "ECVXEditorPreviewMode::Witness1mm" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "ECVXEditorPreviewMode::None", (int64)ECVXEditorPreviewMode::None },
		{ "ECVXEditorPreviewMode::Witness1mm", (int64)ECVXEditorPreviewMode::Witness1mm },
		{ "ECVXEditorPreviewMode::DefaultProof", (int64)ECVXEditorPreviewMode::DefaultProof },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct UHT_STATICS 
const UECodeGen_Private::FEnumParams UHT_STATICS::EnumParams = {
	(FTypeConstructFunc*)Z_Construct_UPackage__Script_CicadaVoxel,
	nullptr,
	"ECVXEditorPreviewMode",
	"ECVXEditorPreviewMode",
	UHT_STATICS::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(UHT_STATICS::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	(uint8)UEnum::EUnderlyingType::uint8,
	METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)
};
static FEnumRegistrationInfo ZRIE_ECVXEditorPreviewMode;
UEnum* Z_Construct_UEnum_CicadaVoxel_ECVXEditorPreviewMode(ETypeConstructPhase Phase)
{
	if (Phase == ETypeConstructPhase::Outer)
	{
		if (!ZRIE_ECVXEditorPreviewMode.OuterSingleton)
		{
			ZRIE_ECVXEditorPreviewMode.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_CicadaVoxel_ECVXEditorPreviewMode, (UObject*)Z_Construct_UPackage__Script_CicadaVoxel(ETypeConstructPhase::Outer), TEXT("ECVXEditorPreviewMode"));
		}
		return ZRIE_ECVXEditorPreviewMode.OuterSingleton;
	}
	if (!ZRIE_ECVXEditorPreviewMode.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(ZRIE_ECVXEditorPreviewMode.InnerSingleton, UHT_STATICS::EnumParams);
	}
	return ZRIE_ECVXEditorPreviewMode.InnerSingleton;
}
#undef UHT_STATICS
// ********** End Enum ECVXEditorPreviewMode *******************************************************

// ********** Begin Class ACicadaVoxelVolumeActor Function CarveSphereLocalMM **********************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_ACicadaVoxelVolumeActor_CarveSphereLocalMM_Statics
struct UHT_STATICS
{
	struct CicadaVoxelVolumeActor_eventCarveSphereLocalMM_Parms
	{
		FVector CenterMM;
		float RadiusMM;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "Category", "CVX|Edit" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function CarveSphereLocalMM constinit property declarations ********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_CenterMM;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RadiusMM;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function CarveSphereLocalMM constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function CarveSphereLocalMM Property Definitions *******************************
const UECodeGen_Private::FStructPropertyParams UHT_STATICS::NewProp_CenterMM = { "CenterMM", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, nullptr, nullptr, 1, STRUCT_OFFSET(CicadaVoxelVolumeActor_eventCarveSphereLocalMM_Parms, CenterMM), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_RadiusMM = { "RadiusMM", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(CicadaVoxelVolumeActor_eventCarveSphereLocalMM_Parms, RadiusMM), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_CenterMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_RadiusMM,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Function CarveSphereLocalMM Property Definitions *********************************
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_ACicadaVoxelVolumeActor, nullptr, "CarveSphereLocalMM", UHT_STATICS::PropPointers, UE_ARRAY_COUNT(UHT_STATICS::PropPointers), DataSizeOf<UHT_STATICS::CicadaVoxelVolumeActor_eventCarveSphereLocalMM_Parms>(), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
static_assert(sizeof(UHT_STATICS::CicadaVoxelVolumeActor_eventCarveSphereLocalMM_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ACicadaVoxelVolumeActor_CarveSphereLocalMM(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
DEFINE_FUNCTION(ACicadaVoxelVolumeActor::execCarveSphereLocalMM)
{
	P_GET_STRUCT(FVector,Z_Param_CenterMM);
	P_GET_PROPERTY(FFloatProperty,Z_Param_RadiusMM);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->CarveSphereLocalMM(Z_Param_CenterMM,Z_Param_RadiusMM);
	P_NATIVE_END;
}
// ********** End Class ACicadaVoxelVolumeActor Function CarveSphereLocalMM ************************

// ********** Begin Class ACicadaVoxelVolumeActor Function CarveVoxelLocalMM ***********************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_ACicadaVoxelVolumeActor_CarveVoxelLocalMM_Statics
struct UHT_STATICS
{
	struct CicadaVoxelVolumeActor_eventCarveVoxelLocalMM_Parms
	{
		FIntVector VoxelMM;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "Category", "CVX|Edit" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function CarveVoxelLocalMM constinit property declarations *********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_VoxelMM;
	static void NewProp_ReturnValue_SetBit(void* Obj)
	{
		((CicadaVoxelVolumeActor_eventCarveVoxelLocalMM_Parms*)Obj)->ReturnValue = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function CarveVoxelLocalMM constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function CarveVoxelLocalMM Property Definitions ********************************
const UECodeGen_Private::FStructPropertyParams UHT_STATICS::NewProp_VoxelMM = { "VoxelMM", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, nullptr, nullptr, 1, STRUCT_OFFSET(CicadaVoxelVolumeActor_eventCarveVoxelLocalMM_Parms, VoxelMM), Z_Construct_UScriptStruct_FIntVector, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(CicadaVoxelVolumeActor_eventCarveVoxelLocalMM_Parms), &UHT_STATICS::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_VoxelMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Function CarveVoxelLocalMM Property Definitions **********************************
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_ACicadaVoxelVolumeActor, nullptr, "CarveVoxelLocalMM", UHT_STATICS::PropPointers, UE_ARRAY_COUNT(UHT_STATICS::PropPointers), DataSizeOf<UHT_STATICS::CicadaVoxelVolumeActor_eventCarveVoxelLocalMM_Parms>(), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
static_assert(sizeof(UHT_STATICS::CicadaVoxelVolumeActor_eventCarveVoxelLocalMM_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ACicadaVoxelVolumeActor_CarveVoxelLocalMM(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
DEFINE_FUNCTION(ACicadaVoxelVolumeActor::execCarveVoxelLocalMM)
{
	P_GET_STRUCT(FIntVector,Z_Param_VoxelMM);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->CarveVoxelLocalMM(Z_Param_VoxelMM);
	P_NATIVE_END;
}
// ********** End Class ACicadaVoxelVolumeActor Function CarveVoxelLocalMM *************************

// ********** Begin Class ACicadaVoxelVolumeActor Function GetStatsString **************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_ACicadaVoxelVolumeActor_GetStatsString_Statics
struct UHT_STATICS
{
	struct CicadaVoxelVolumeActor_eventGetStatsString_Parms
	{
		FString ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "Category", "CVX|Stats" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetStatsString constinit property declarations ************************
	static const UECodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetStatsString constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetStatsString Property Definitions ***********************************
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(CicadaVoxelVolumeActor_eventGetStatsString_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Function GetStatsString Property Definitions *************************************
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_ACicadaVoxelVolumeActor, nullptr, "GetStatsString", UHT_STATICS::PropPointers, UE_ARRAY_COUNT(UHT_STATICS::PropPointers), DataSizeOf<UHT_STATICS::CicadaVoxelVolumeActor_eventGetStatsString_Parms>(), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
static_assert(sizeof(UHT_STATICS::CicadaVoxelVolumeActor_eventGetStatsString_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ACicadaVoxelVolumeActor_GetStatsString(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
DEFINE_FUNCTION(ACicadaVoxelVolumeActor::execGetStatsString)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FString*)Z_Param__Result=P_THIS->GetStatsString();
	P_NATIVE_END;
}
// ********** End Class ACicadaVoxelVolumeActor Function GetStatsString ****************************

// ********** Begin Class ACicadaVoxelVolumeActor Function IsSolidLocalMM **************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_ACicadaVoxelVolumeActor_IsSolidLocalMM_Statics
struct UHT_STATICS
{
	struct CicadaVoxelVolumeActor_eventIsSolidLocalMM_Parms
	{
		FIntVector VoxelMM;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "Category", "CVX|Query" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function IsSolidLocalMM constinit property declarations ************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_VoxelMM;
	static void NewProp_ReturnValue_SetBit(void* Obj)
	{
		((CicadaVoxelVolumeActor_eventIsSolidLocalMM_Parms*)Obj)->ReturnValue = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function IsSolidLocalMM constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function IsSolidLocalMM Property Definitions ***********************************
const UECodeGen_Private::FStructPropertyParams UHT_STATICS::NewProp_VoxelMM = { "VoxelMM", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, nullptr, nullptr, 1, STRUCT_OFFSET(CicadaVoxelVolumeActor_eventIsSolidLocalMM_Parms, VoxelMM), Z_Construct_UScriptStruct_FIntVector, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(CicadaVoxelVolumeActor_eventIsSolidLocalMM_Parms), &UHT_STATICS::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_VoxelMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Function IsSolidLocalMM Property Definitions *************************************
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_ACicadaVoxelVolumeActor, nullptr, "IsSolidLocalMM", UHT_STATICS::PropPointers, UE_ARRAY_COUNT(UHT_STATICS::PropPointers), DataSizeOf<UHT_STATICS::CicadaVoxelVolumeActor_eventIsSolidLocalMM_Parms>(), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
static_assert(sizeof(UHT_STATICS::CicadaVoxelVolumeActor_eventIsSolidLocalMM_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ACicadaVoxelVolumeActor_IsSolidLocalMM(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
DEFINE_FUNCTION(ACicadaVoxelVolumeActor::execIsSolidLocalMM)
{
	P_GET_STRUCT(FIntVector,Z_Param_VoxelMM);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsSolidLocalMM(Z_Param_VoxelMM);
	P_NATIVE_END;
}
// ********** End Class ACicadaVoxelVolumeActor Function IsSolidLocalMM ****************************

// ********** Begin Class ACicadaVoxelVolumeActor Function RebuildVisualization ********************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_ACicadaVoxelVolumeActor_RebuildVisualization_Statics
struct UHT_STATICS
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "CallInEditor", "true" },
		{ "Category", "CVX|Visual" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RebuildVisualization constinit property declarations ******************
// ********** End Function RebuildVisualization constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_ACicadaVoxelVolumeActor, nullptr, "RebuildVisualization", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
UFunction* Z_Construct_UFunction_ACicadaVoxelVolumeActor_RebuildVisualization(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
DEFINE_FUNCTION(ACicadaVoxelVolumeActor::execRebuildVisualization)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RebuildVisualization();
	P_NATIVE_END;
}
// ********** End Class ACicadaVoxelVolumeActor Function RebuildVisualization **********************

// ********** Begin Class ACicadaVoxelVolumeActor Function ResetProof ******************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_ACicadaVoxelVolumeActor_ResetProof_Statics
struct UHT_STATICS
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "CallInEditor", "true" },
		{ "Category", "CVX|Proof" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function ResetProof constinit property declarations ****************************
// ********** End Function ResetProof constinit property declarations ******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_ACicadaVoxelVolumeActor, nullptr, "ResetProof", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
UFunction* Z_Construct_UFunction_ACicadaVoxelVolumeActor_ResetProof(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
DEFINE_FUNCTION(ACicadaVoxelVolumeActor::execResetProof)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ResetProof();
	P_NATIVE_END;
}
// ********** End Class ACicadaVoxelVolumeActor Function ResetProof ********************************

// ********** Begin Class ACicadaVoxelVolumeActor Function Run1mmWitnessProof **********************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_ACicadaVoxelVolumeActor_Run1mmWitnessProof_Statics
struct UHT_STATICS
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "CallInEditor", "true" },
		{ "Category", "CVX|1mm Witness" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function Run1mmWitnessProof constinit property declarations ********************
// ********** End Function Run1mmWitnessProof constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_ACicadaVoxelVolumeActor, nullptr, "Run1mmWitnessProof", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
UFunction* Z_Construct_UFunction_ACicadaVoxelVolumeActor_Run1mmWitnessProof(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
DEFINE_FUNCTION(ACicadaVoxelVolumeActor::execRun1mmWitnessProof)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Run1mmWitnessProof();
	P_NATIVE_END;
}
// ********** End Class ACicadaVoxelVolumeActor Function Run1mmWitnessProof ************************

// ********** Begin Class ACicadaVoxelVolumeActor Function RunDefaultProof *************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_ACicadaVoxelVolumeActor_RunDefaultProof_Statics
struct UHT_STATICS
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "CallInEditor", "true" },
		{ "Category", "CVX|Proof" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RunDefaultProof constinit property declarations ***********************
// ********** End Function RunDefaultProof constinit property declarations *************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_ACicadaVoxelVolumeActor, nullptr, "RunDefaultProof", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
UFunction* Z_Construct_UFunction_ACicadaVoxelVolumeActor_RunDefaultProof(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
DEFINE_FUNCTION(ACicadaVoxelVolumeActor::execRunDefaultProof)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RunDefaultProof();
	P_NATIVE_END;
}
// ********** End Class ACicadaVoxelVolumeActor Function RunDefaultProof ***************************

// ********** Begin Class ACicadaVoxelVolumeActor **************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UClass_ACicadaVoxelVolumeActor_Statics
struct UHT_STATICS
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "CicadaVoxelVolumeActor.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableEditorPreview_MetaData[] = {
		{ "Category", "CVX|Editor Preview" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EditorPreviewMode_MetaData[] = {
		{ "Category", "CVX|Editor Preview" },
		{ "EditCondition", "bEnableEditorPreview" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastEditorPreview_MetaData[] = {
		{ "Category", "CVX|Editor Preview" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LogicalSizeMM_MetaData[] = {
		{ "Category", "CVX|Volume" },
		{ "ClampMin", "16" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ProofCenterMM_MetaData[] = {
		{ "Category", "CVX|Proof" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ProofSphereRadiusMM_MetaData[] = {
		{ "Category", "CVX|Proof" },
		{ "ClampMin", "0.5" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAutoRunProofOnBeginPlay_MetaData[] = {
		{ "Category", "CVX|Proof" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bInclude1mmWitnessInDefaultProof_MetaData[] = {
		{ "Category", "CVX|1mm Witness" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WitnessPatchOriginMM_MetaData[] = {
		{ "Category", "CVX|1mm Witness" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WitnessPatchSizeMM_MetaData[] = {
		{ "Category", "CVX|1mm Witness" },
		{ "ClampMax", "32" },
		{ "ClampMin", "4" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FineVoxelVisualScale_MetaData[] = {
		{ "Category", "CVX|Visual" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.55" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bForceSeamlessRuntimeFineVoxels_MetaData[] = {
		{ "Category", "CVX|Visual" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Editor witness can keep tiny diagnostic gaps, but gameplay surfaces should tile exactly.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Editor witness can keep tiny diagnostic gaps, but gameplay surfaces should tile exactly." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRefineExposedHotSurfacesTo1mm_MetaData[] = {
		{ "Category", "CVX|Visual" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// A Full 4 mm block adjacent to empty/mixed space is promoted to 64 exact 1 mm cells.\n// This prevents visible destruction from degrading to 4 mm cubes as a cavity grows.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "A Full 4 mm block adjacent to empty/mixed space is promoted to 64 exact 1 mm cells.\nThis prevents visible destruction from degrading to 4 mm cubes as a cavity grows." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCullOccludedFineVoxels_MetaData[] = {
		{ "Category", "CVX|Visual" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Fine ISM slots remain fixed for the temporary renderer, but fully occluded 1 mm cells\n// are hidden so only the actual exposed material skin contributes visible geometry.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Fine ISM slots remain fixed for the temporary renderer, but fully occluded 1 mm cells\nare hidden so only the actual exposed material skin contributes visible geometry." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SolidFrontierDepthChunks_MetaData[] = {
		{ "Category", "CVX|Visual" },
		{ "ClampMax", "2" },
		{ "ClampMin", "1" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Keep a visible implicit-solid frontier around dirty chunks so the procedural interior\n// never looks hollow while the temporary instance renderer is still in use.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Keep a visible implicit-solid frontier around dirty chunks so the procedural interior\nnever looks hollow while the temporary instance renderer is still in use." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxVisualInstances_MetaData[] = {
		{ "Category", "CVX|Visual" },
		{ "ClampMax", "1000000" },
		{ "ClampMin", "1000" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseIncrementalVisualUpdates_MetaData[] = {
		{ "Category", "CVX|Performance" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// CVX-002A hot edit path. Runtime edits mutate stable HISM slots instead of clearing and\n// recreating every instance. Work is spread across frames using a dirty 16 mm chunk queue.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "CVX-002A hot edit path. Runtime edits mutate stable HISM slots instead of clearing and\nrecreating every instance. Work is spread across frames using a dirty 16 mm chunk queue." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_VisualChunkBudgetPerFrame_MetaData[] = {
		{ "AdvancedDisplay", "" },
		{ "Category", "CVX|Performance" },
		{ "ClampMax", "64" },
		{ "ClampMin", "1" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Legacy chunk-count cap retained as a safety ceiling. CVX-002B additionally uses a\n// millisecond budget and 4 mm dirty-block granularity.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Legacy chunk-count cap retained as a safety ceiling. CVX-002B additionally uses a\nmillisecond budget and 4 mm dirty-block granularity." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_VisualUpdateBudgetMS_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ClampMax", "8.0" },
		{ "ClampMin", "0.10" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxVisualBlocksPerFrame_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ClampMax", "256" },
		{ "ClampMin", "1" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HotBlockReserveInstances_MetaData[] = {
		{ "AdvancedDisplay", "" },
		{ "Category", "CVX|Performance" },
		{ "ClampMax", "131072" },
		{ "ClampMin", "0" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HotFineReserveInstances_MetaData[] = {
		{ "AdvancedDisplay", "" },
		{ "Category", "CVX|Performance" },
		{ "ClampMax", "262144" },
		{ "ClampMin", "0" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAsyncHISMTreeBuilds_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bPublishLegacyFullGPUSnapshot_MetaData[] = {
		{ "AdvancedDisplay", "" },
		{ "Category", "CVX|Performance" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// The old validation bridge serializes the entire dirty sparse set. It is not used by the\n// current visualizer, so keep it off during hot runtime edits. The future GPU page renderer\n// will publish deltas instead of rebuilding a full snapshot.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The old validation bridge serializes the entire dirty sparse set. It is not used by the\ncurrent visualizer, so keep it off during hot runtime edits. The future GPU page renderer\nwill publish deltas instead of rebuilding a full snapshot." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RuntimeStatsPublishInterval_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.02" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseExactSurfaceMeshRuntime_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// CVX-002F: runtime hot geometry is no longer represented by one Unreal instance per\n// 1 mm voxel. Each affected 16 mm chunk gets an exact surface mesh extracted from the\n// authoritative 1 mm field. This removes the coarse/hollow visual catch-up failure.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "CVX-002F: runtime hot geometry is no longer represented by one Unreal instance per\n1 mm voxel. Each affected 16 mm chunk gets an exact surface mesh extracted from the\nauthoritative 1 mm field. This removes the coarse/hollow visual catch-up failure." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bGreedySurfaceMeshing_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Merge adjacent coplanar 1 mm faces into rectangles. Geometry still changes only on\n// integer millimetre boundaries; this reduces triangle count without reducing truth resolution.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Merge adjacent coplanar 1 mm faces into rectangles. Geometry still changes only on\ninteger millimetre boundaries; this reduces triangle count without reducing truth resolution." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bTwoSidedExactSurface_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// CVX-002G integrity guard: render exact hot surfaces from either side. This is\n// deliberately limited to the small dynamic hot meshlets while we validate final\n// triangle orientation before the custom GPU renderer replaces DynamicMesh.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "CVX-002G integrity guard: render exact hot surfaces from either side. This is\ndeliberately limited to the small dynamic hot meshlets while we validate final\ntriangle orientation before the custom GPU renderer replaces DynamicMesh." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bExactSurfaceShadows_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Tiny rapidly changing 1 mm cavity faces produce noisy shadow-map aliasing and add\n// no useful structural information. Cold/coarse geometry continues to cast shadows.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Tiny rapidly changing 1 mm cavity faces produce noisy shadow-map aliasing and add\nno useful structural information. Cold/coarse geometry continues to cast shadows." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bSafeSurfaceHandoff_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Keep the cold 16 mm proxy until a valid exact mesh has been installed. This prevents\n// the temporary 16 mm square holes seen when a queued neighbour was hidden before its\n// replacement render proxy was ready.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Keep the cold 16 mm proxy until a valid exact mesh has been installed. This prevents\nthe temporary 16 mm square holes seen when a queued neighbour was hidden before its\nreplacement render proxy was ready." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bSingleOwnerRuntimeSurface_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// CVX-002H: exact runtime uses a plain ISM cold shell, never HISM, so a chunk has\n// exactly one render owner during destruction. This removes HISM tree lag / ghost\n// rectangles where an old coarse proxy and a new exact mesh compete for the same area.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "CVX-002H: exact runtime uses a plain ISM cold shell, never HISM, so a chunk has\nexactly one render owner during destruction. This removes HISM tree lag / ghost\nrectangles where an old coarse proxy and a new exact mesh compete for the same area." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SurfaceProxyRetirements_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bImmediateImpactNeighbourMeshes_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Rebuild the impact chunk plus six face neighbours immediately. The edited surface\n// therefore crosses 16 mm chunk boundaries as one coherent local patch instead of\n// visibly waiting for neighbour chunks over several frames.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Rebuild the impact chunk plus six face neighbours immediately. The edited surface\ntherefore crosses 16 mm chunk boundaries as one coherent local patch instead of\nvisibly waiting for neighbour chunks over several frames." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SurfaceMeshBudgetMS_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ClampMax", "8.0" },
		{ "ClampMin", "0.10" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxSurfaceMeshChunksPerFrame_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ClampMax", "32" },
		{ "ClampMin", "1" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bImmediateImpactSurfaceMesh_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Rebuild the chunk under the beam immediately; neighbours remain budgeted.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Rebuild the chunk under the beam immediately; neighbours remain budgeted." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BeamSurfaceMeshBackpressureChunks_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ClampMax", "256" },
		{ "ClampMin", "0" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastSurfaceMeshUpdateMS_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PendingSurfaceMeshChunkCount_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastSurfaceMeshChunksProcessed_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastSurfaceMeshQuads_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastSurfaceMeshTriangles_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActiveSurfaceMeshChunks_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SurfaceHandoffGuards_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SurfaceImmediateNeighbourRebuilds_MetaData[] = {
		{ "Category", "CVX|Exact Surface Mesh" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastSparseEditMS_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastVisualUpdateMS_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastFullVisualRebuildMS_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PendingVisualChunkCount_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PendingVisualBlockCount_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastVisualBlocksProcessed_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastRefinedSurfaceBlocksProcessed_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FineSlotBatchesReused_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastLegacyGPUSnapshotMS_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastPerformanceStats_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableVirtualPageSpine_MetaData[] = {
		{ "Category", "CVX|Virtual Pages" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// CVX-002D: virtual 64 mm hot-page spine. This tracks only edited sparse pages and\n// estimates the exact compact upload payload that the custom GPU renderer will consume.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "CVX-002D: virtual 64 mm hot-page spine. This tracks only edited sparse pages and\nestimates the exact compact upload payload that the custom GPU renderer will consume." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_VirtualPageEdgeMM_MetaData[] = {
		{ "Category", "CVX|Virtual Pages" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResidentEditedPageCount_MetaData[] = {
		{ "Category", "CVX|Virtual Pages" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ExplicitPageChunkCount_MetaData[] = {
		{ "Category", "CVX|Virtual Pages" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastPageDeltaBytes_MetaData[] = {
		{ "Category", "CVX|Virtual Pages" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FullWorldHotUploadCount_MetaData[] = {
		{ "Category", "CVX|Virtual Pages" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastVirtualPageStats_MetaData[] = {
		{ "Category", "CVX|Virtual Pages" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUsePageNativeRuntime_MetaData[] = {
		{ "Category", "CVX|003A Page Runtime" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// CVX-003A active runtime bridge. Hot rendering is owned by 64 mm pages rather than\n// hundreds of 16 mm DynamicMesh components. The old chunk renderer remains only as an\n// opt-in reference path for comparison while the custom GPU ray renderer is brought online.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "CVX-003A active runtime bridge. Hot rendering is owned by 64 mm pages rather than\nhundreds of 16 mm DynamicMesh components. The old chunk renderer remains only as an\nopt-in reference path for comparison while the custom GPU ray renderer is brought online." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxSurfacePagesPerFrame_MetaData[] = {
		{ "Category", "CVX|003A Page Runtime" },
		{ "ClampMax", "16" },
		{ "ClampMin", "1" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SurfacePageBudgetMS_MetaData[] = {
		{ "Category", "CVX|003A Page Runtime" },
		{ "ClampMax", "16.0" },
		{ "ClampMin", "0.25" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAsyncSurfacePageMeshing_MetaData[] = {
		{ "Category", "CVX|003C Realtime Surface" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxAsyncSurfacePageJobs_MetaData[] = {
		{ "Category", "CVX|003C Realtime Surface" },
		{ "ClampMax", "8" },
		{ "ClampMin", "1" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRealtimeExteriorSurface_MetaData[] = {
		{ "Category", "CVX|003C Realtime Surface" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Outer world boundary is a tiny 2D problem compared with remeshing an entire\n// 64 mm volume page. Update touched exterior pages synchronously so the player\n// sees the hole in the same frame; complete cavity geometry is built asynchronously.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Outer world boundary is a tiny 2D problem compared with remeshing an entire\n64 mm volume page. Update touched exterior pages synchronously so the player\nsees the hole in the same frame; complete cavity geometry is built asynchronously." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxRealtimeExteriorPagesPerFrame_MetaData[] = {
		{ "Category", "CVX|003C Realtime Surface" },
		{ "ClampMax", "16" },
		{ "ClampMin", "1" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RealtimeExteriorBudgetMS_MetaData[] = {
		{ "Category", "CVX|003C Realtime Surface" },
		{ "ClampMax", "8.0" },
		{ "ClampMin", "0.1" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AsyncSurfacePageJobsInFlight_MetaData[] = {
		{ "Category", "CVX|003C Realtime Surface" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AsyncSurfacePagesCompleted_MetaData[] = {
		{ "Category", "CVX|003C Realtime Surface" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AsyncSurfaceStaleResults_MetaData[] = {
		{ "Category", "CVX|003C Realtime Surface" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastAsyncSurfaceWorkerMS_MetaData[] = {
		{ "Category", "CVX|003C Realtime Surface" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastExteriorSurfaceMS_MetaData[] = {
		{ "Category", "CVX|003C Realtime Surface" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastExteriorPagesUpdated_MetaData[] = {
		{ "Category", "CVX|003C Realtime Surface" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableReferenceSurfaceMemoryGovernor_MetaData[] = {
		{ "Category", "CVX|003D Reference Cache" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// CVX-003D: the DynamicMesh page renderer is now explicitly a bounded reference cache.\n// It is not allowed to consume VRAM in proportion to the total amount of destruction.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "CVX-003D: the DynamicMesh page renderer is now explicitly a bounded reference cache.\nIt is not allowed to consume VRAM in proportion to the total amount of destruction." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxResidentReferenceSurfacePages_MetaData[] = {
		{ "Category", "CVX|003D Reference Cache" },
		{ "ClampMax", "256" },
		{ "ClampMin", "4" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReferenceSurfaceBudgetMB_MetaData[] = {
		{ "Category", "CVX|003D Reference Cache" },
		{ "ClampMax", "1024.0" },
		{ "ClampMin", "16.0" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxReferencePageQuads_MetaData[] = {
		{ "Category", "CVX|003D Reference Cache" },
		{ "ClampMax", "250000" },
		{ "ClampMin", "1000" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RealtimeExteriorMaxHzPerPage_MetaData[] = {
		{ "Category", "CVX|003D Reference Cache" },
		{ "ClampMax", "60.0" },
		{ "ClampMin", "2.0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Replacing DynamicMesh topology every frame can temporarily retain old render buffers\n// until the render thread/RHI retires them. Cap how often one exterior page is rebuilt.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Replacing DynamicMesh topology every frame can temporarily retain old render buffers\nuntil the render thread/RHI retires them. Cap how often one exterior page is rebuilt." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReferenceSurfaceEvictions_MetaData[] = {
		{ "Category", "CVX|003D Reference Cache" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReferenceSurfaceComplexityRejects_MetaData[] = {
		{ "Category", "CVX|003D Reference Cache" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EstimatedReferenceSurfaceMB_MetaData[] = {
		{ "Category", "CVX|003D Reference Cache" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResidentReferenceSurfacePages_MetaData[] = {
		{ "Category", "CVX|003D Reference Cache" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bKeepLegacyChunkSurfaceReference_MetaData[] = {
		{ "AdvancedDisplay", "" },
		{ "Category", "CVX|003A Page Runtime" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PendingSurfacePageCount_MetaData[] = {
		{ "Category", "CVX|003A Page Runtime" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastSurfacePagesProcessed_MetaData[] = {
		{ "Category", "CVX|003A Page Runtime" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActiveSurfacePageCount_MetaData[] = {
		{ "Category", "CVX|003A Page Runtime" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastSurfacePageMS_MetaData[] = {
		{ "Category", "CVX|003A Page Runtime" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastSurfacePageQuads_MetaData[] = {
		{ "Category", "CVX|003A Page Runtime" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUploadChangedPagesToGPU_MetaData[] = {
		{ "Category", "CVX|003A GPU Delta" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastActualGPUDeltaBytes_MetaData[] = {
		{ "Category", "CVX|003A GPU Delta" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TotalActualGPUDeltaBytes_MetaData[] = {
		{ "Category", "CVX|003A GPU Delta" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GPUDeltaPacketCount_MetaData[] = {
		{ "Category", "CVX|003A GPU Delta" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseHierarchicalBulkEdits_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// CVX-003B: destruction throughput is now treated as a data problem first. Large\n// subtract stamps classify 64 mm pages -> 16 mm chunks -> 4 mm blocks and only\n// touch individual 1 mm bits on the geometric boundary.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "CVX-003B: destruction throughput is now treated as a data problem first. Large\nsubtract stamps classify 64 mm pages -> 16 mm chunks -> 4 mm blocks and only\ntouch individual 1 mm bits on the geometric boundary." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bThroughputFirstEditing_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Let authoritative edits run at full speed even when the temporary CPU reference\n// surface cannot visually catch up. The GPU renderer is the real consumer next.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Let authoritative edits run at full speed even when the temporary CPU reference\nsurface cannot visually catch up. The GPU renderer is the real consumer next." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bDeferReferenceSurfaceWhileBeam_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// While the beam is held, do not spend frame time rebuilding the old DynamicMesh\n// reference pages. Queue invalidations and catch the reference view up after release.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "While the beam is held, do not spend frame time rebuilding the old DynamicMesh\nreference pages. Queue invalidations and catch the reference view up after release." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxBeamCarvesPerFrame_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
		{ "ClampMax", "16" },
		{ "ClampMin", "1" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ThroughputBeamMinEditsPerSecond_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
		{ "ClampMax", "1000.0" },
		{ "ClampMin", "30.0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Throughput mode never runs the beam slower than this even if an old placed actor\n// serialized a lower BeamEditsPerSecond value from an earlier plugin build.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Throughput mode never runs the beam slower than this even if an old placed actor\nserialized a lower BeamEditsPerSecond value from an earlier plugin build." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MacroBlastRadiusMM_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
		{ "ClampMax", "2000.0" },
		{ "ClampMin", "10.0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Press B while aiming at the volume for a large one-shot edit benchmark.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Press B while aiming at the volume for a large one-shot edit benchmark." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MacroBlastCount_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastBulkPagesCleared_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastBulkChunksCleared_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastBulkBlocksCleared_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastBulkBoundaryBlocks_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastBulkSubBlocks2mmCleared_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastBulkFineVoxelTests_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastBeamCarvesThisFrame_MetaData[] = {
		{ "Category", "CVX|003B Bulk Edit" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseRoboticsDirectFieldRenderer_MetaData[] = {
		{ "Category", "CVX|004A DIRECT FIELD - FORCED" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// CVX-004A ROBOTICS/TRAINING MODE\n//\n// The active game-world visual now ray-intersects the same sparse material field\n// used by edits. HISM/ISM/DynamicMesh surfaces are disabled, so there is no\n// separate \"outer skin\" that can flicker or disagree with sensor truth.\n// R4 FORCE DIRECT: these are diagnostics only. Game/PIE worlds always use the\n// direct material-field renderer and always disable the retired runtime skin.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "CVX-004A ROBOTICS/TRAINING MODE\n\nThe active game-world visual now ray-intersects the same sparse material field\nused by edits. HISM/ISM/DynamicMesh surfaces are disabled, so there is no\nseparate \"outer skin\" that can flicker or disagree with sensor truth.\nR4 FORCE DIRECT: these are diagnostics only. Game/PIE worlds always use the\ndirect material-field renderer and always disable the retired runtime skin." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bDisableLegacyRuntimeVisualsInRoboticsMode_MetaData[] = {
		{ "Category", "CVX|004A DIRECT FIELD - FORCED" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DirectFieldRuntimeMode_MetaData[] = {
		{ "Category", "CVX|004A DIRECT FIELD - FORCED" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RoboticsFieldSnapshotHz_MetaData[] = {
		{ "Category", "CVX|004A DIRECT FIELD - FORCED" },
		{ "ClampMax", "240.0" },
		{ "ClampMin", "10.0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Snapshot coalescing is deliberately frame-like: hundreds of authoritative beam\n// edits can occur per second while the renderer consumes the newest stable field\n// generation. CVX-004B replaces this bootstrap snapshot with persistent GPU deltas.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Snapshot coalescing is deliberately frame-like: hundreds of authoritative beam\nedits can occur per second while the renderer consumes the newest stable field\ngeneration. CVX-004B replaces this bootstrap snapshot with persistent GPU deltas." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastRoboticsFieldSnapshotMS_MetaData[] = {
		{ "Category", "CVX|004A DIRECT FIELD - FORCED" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastRoboticsFieldSnapshotBytes_MetaData[] = {
		{ "Category", "CVX|004A DIRECT FIELD - FORCED" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RoboticsFieldPartialPages_MetaData[] = {
		{ "Category", "CVX|004A DIRECT FIELD - FORCED" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RoboticsFieldExplicitChunks_MetaData[] = {
		{ "Category", "CVX|004A DIRECT FIELD - FORCED" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RoboticsFieldMixedBlocks_MetaData[] = {
		{ "Category", "CVX|004A DIRECT FIELD - FORCED" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastRoboticsFieldStats_MetaData[] = {
		{ "Category", "CVX|004A DIRECT FIELD - FORCED" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableFirstPersonCarving_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bContinuousBeamDigging_MetaData[] = {
		{ "Category", "CVX|Interaction" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Hold LMB for a continuous digging beam. The beam is deliberately rate-limited so\n// voxel truth cannot outrun the temporary visualizer by hundreds of dirty blocks.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Hold LMB for a continuous digging beam. The beam is deliberately rate-limited so\nvoxel truth cannot outrun the temporary visualizer by hundreds of dirty blocks." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BeamEditsPerSecond_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ClampMax", "1000.0" },
		{ "ClampMin", "1.0" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BeamCarveRadiusMM_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ClampMax", "500.0" },
		{ "ClampMin", "0.5" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BeamVisualBackpressureBlocks_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ClampMax", "4096" },
		{ "ClampMin", "0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Temporary visualizer guard. The authoritative edit is cheap, but allowing it to run\n// hundreds of blocks ahead is what produced invisible interiors / coarse catch-up.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Temporary visualizer guard. The authoritative edit is cheap, but allowing it to run\nhundreds of blocks ahead is what produced invisible interiors / coarse catch-up." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseHierarchicalRayTraversal_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bImmediateImpactSurfaceRefresh_MetaData[] = {
		{ "Category", "CVX|Interaction" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Refresh the impact block plus six face neighbours immediately. Bulk visual work remains\n// time-budgeted, but the point the player is drilling should never sit behind a long queue.\n" },
#endif
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Refresh the impact block plus six face neighbours immediately. Bulk visual work remains\ntime-budgeted, but the point the player is drilling should never sit behind a long queue." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FirstPersonCarveRadiusMM_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ClampMax", "100.0" },
		{ "ClampMin", "0.5" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FirstPersonMaxDistanceCM_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ClampMin", "100.0" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastRayQueryMS_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastRayChunkSkips16mm_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastRayBlockSkips4mm_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastRayFineSteps1mm_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BeamThrottleFrames_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BeamCarvesThisHold_MetaData[] = {
		{ "Category", "CVX|Interaction" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastImmediateSurfaceMS_MetaData[] = {
		{ "Category", "CVX|Performance" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastStats_MetaData[] = {
		{ "Category", "CVX|Stats" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastVisualStats_MetaData[] = {
		{ "Category", "CVX|Stats" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Last1mmWitness_MetaData[] = {
		{ "Category", "CVX|1mm Witness" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SceneRoot_MetaData[] = {
		{ "Category", "CVX|Visual" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CoarseChunkInstances_MetaData[] = {
		{ "Category", "CVX|Visual" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ExactColdShellInstances_MetaData[] = {
		{ "Category", "CVX|Visual" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Runtime exact-surface cold shell. Plain ISM is intentional: edited chunks are\n// retired once and never need a HISM cluster-tree rebuild or stale cluster visibility.\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Runtime exact-surface cold shell. Plain ISM is intentional: edited chunks are\nretired once and never need a HISM cluster-tree rebuild or stale cluster visibility." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FrontierChunkInstances_MetaData[] = {
		{ "Category", "CVX|Visual" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Untouched implicit-solid chunks immediately around the edited region. Plain ISM is\n// intentional: these chunks change membership during destruction and must not wait for\n// a HISM cluster-tree rebuild before becoming visible.\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Untouched implicit-solid chunks immediately around the edited region. Plain ISM is\nintentional: these chunks change membership during destruction and must not wait for\na HISM cluster-tree rebuild before becoming visible." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FullBlockInstances_MetaData[] = {
		{ "Category", "CVX|Visual" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Hot geometry intentionally uses plain ISM rather than HISM. UE's HISM hierarchy is\n// excellent for thousands of static instances; repeatedly changing its tree is exactly the\n// wrong workload for destructive 4 mm / 1 mm cells.\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Hot geometry intentionally uses plain ISM rather than HISM. UE's HISM hierarchy is\nexcellent for thousands of static instances; repeatedly changing its tree is exactly the\nwrong workload for destructive 4 mm / 1 mm cells." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FineVoxelInstances_MetaData[] = {
		{ "Category", "CVX|Visual" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SurfaceMeshComponents_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Runtime-only exact-surface meshlets. One component per active 16 mm hot chunk keeps\n// topology rebuilds bounded to tiny local regions instead of invalidating one giant mesh.\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Runtime-only exact-surface meshlets. One component per active 16 mm hot chunk keeps\ntopology rebuilds bounded to tiny local regions instead of invalidating one giant mesh." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SurfacePageComponents_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// CVX-003A: one exact reference mesh per 64 mm hot page. This is the final CPU\n// reference renderer before the page data is consumed directly by the GPU ray path.\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/CicadaVoxelVolumeActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "CVX-003A: one exact reference mesh per 64 mm hot page. This is the final CPU\nreference renderer before the page data is consumed directly by the GPU ray path." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class ACicadaVoxelVolumeActor constinit property declarations ******************
	static void NewProp_bEnableEditorPreview_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bEnableEditorPreview = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableEditorPreview;
	static const UECodeGen_Private::FBytePropertyParams NewProp_EditorPreviewMode_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_EditorPreviewMode;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LastEditorPreview;
	static const UECodeGen_Private::FStructPropertyParams NewProp_LogicalSizeMM;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ProofCenterMM;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ProofSphereRadiusMM;
	static void NewProp_bAutoRunProofOnBeginPlay_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bAutoRunProofOnBeginPlay = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAutoRunProofOnBeginPlay;
	static void NewProp_bInclude1mmWitnessInDefaultProof_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bInclude1mmWitnessInDefaultProof = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bInclude1mmWitnessInDefaultProof;
	static const UECodeGen_Private::FStructPropertyParams NewProp_WitnessPatchOriginMM;
	static const UECodeGen_Private::FIntPropertyParams NewProp_WitnessPatchSizeMM;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FineVoxelVisualScale;
	static void NewProp_bForceSeamlessRuntimeFineVoxels_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bForceSeamlessRuntimeFineVoxels = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bForceSeamlessRuntimeFineVoxels;
	static void NewProp_bRefineExposedHotSurfacesTo1mm_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bRefineExposedHotSurfacesTo1mm = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRefineExposedHotSurfacesTo1mm;
	static void NewProp_bCullOccludedFineVoxels_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bCullOccludedFineVoxels = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCullOccludedFineVoxels;
	static const UECodeGen_Private::FIntPropertyParams NewProp_SolidFrontierDepthChunks;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxVisualInstances;
	static void NewProp_bUseIncrementalVisualUpdates_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bUseIncrementalVisualUpdates = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseIncrementalVisualUpdates;
	static const UECodeGen_Private::FIntPropertyParams NewProp_VisualChunkBudgetPerFrame;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_VisualUpdateBudgetMS;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxVisualBlocksPerFrame;
	static const UECodeGen_Private::FIntPropertyParams NewProp_HotBlockReserveInstances;
	static const UECodeGen_Private::FIntPropertyParams NewProp_HotFineReserveInstances;
	static void NewProp_bAsyncHISMTreeBuilds_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bAsyncHISMTreeBuilds = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAsyncHISMTreeBuilds;
	static void NewProp_bPublishLegacyFullGPUSnapshot_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bPublishLegacyFullGPUSnapshot = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bPublishLegacyFullGPUSnapshot;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RuntimeStatsPublishInterval;
	static void NewProp_bUseExactSurfaceMeshRuntime_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bUseExactSurfaceMeshRuntime = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseExactSurfaceMeshRuntime;
	static void NewProp_bGreedySurfaceMeshing_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bGreedySurfaceMeshing = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bGreedySurfaceMeshing;
	static void NewProp_bTwoSidedExactSurface_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bTwoSidedExactSurface = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bTwoSidedExactSurface;
	static void NewProp_bExactSurfaceShadows_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bExactSurfaceShadows = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bExactSurfaceShadows;
	static void NewProp_bSafeSurfaceHandoff_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bSafeSurfaceHandoff = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bSafeSurfaceHandoff;
	static void NewProp_bSingleOwnerRuntimeSurface_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bSingleOwnerRuntimeSurface = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bSingleOwnerRuntimeSurface;
	static const UECodeGen_Private::FIntPropertyParams NewProp_SurfaceProxyRetirements;
	static void NewProp_bImmediateImpactNeighbourMeshes_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bImmediateImpactNeighbourMeshes = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bImmediateImpactNeighbourMeshes;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_SurfaceMeshBudgetMS;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxSurfaceMeshChunksPerFrame;
	static void NewProp_bImmediateImpactSurfaceMesh_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bImmediateImpactSurfaceMesh = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bImmediateImpactSurfaceMesh;
	static const UECodeGen_Private::FIntPropertyParams NewProp_BeamSurfaceMeshBackpressureChunks;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_LastSurfaceMeshUpdateMS;
	static const UECodeGen_Private::FIntPropertyParams NewProp_PendingSurfaceMeshChunkCount;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastSurfaceMeshChunksProcessed;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastSurfaceMeshQuads;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastSurfaceMeshTriangles;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ActiveSurfaceMeshChunks;
	static const UECodeGen_Private::FIntPropertyParams NewProp_SurfaceHandoffGuards;
	static const UECodeGen_Private::FIntPropertyParams NewProp_SurfaceImmediateNeighbourRebuilds;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_LastSparseEditMS;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_LastVisualUpdateMS;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_LastFullVisualRebuildMS;
	static const UECodeGen_Private::FIntPropertyParams NewProp_PendingVisualChunkCount;
	static const UECodeGen_Private::FIntPropertyParams NewProp_PendingVisualBlockCount;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastVisualBlocksProcessed;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastRefinedSurfaceBlocksProcessed;
	static const UECodeGen_Private::FIntPropertyParams NewProp_FineSlotBatchesReused;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_LastLegacyGPUSnapshotMS;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LastPerformanceStats;
	static void NewProp_bEnableVirtualPageSpine_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bEnableVirtualPageSpine = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableVirtualPageSpine;
	static const UECodeGen_Private::FIntPropertyParams NewProp_VirtualPageEdgeMM;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ResidentEditedPageCount;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ExplicitPageChunkCount;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_LastPageDeltaBytes;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_FullWorldHotUploadCount;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LastVirtualPageStats;
	static void NewProp_bUsePageNativeRuntime_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bUsePageNativeRuntime = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUsePageNativeRuntime;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxSurfacePagesPerFrame;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_SurfacePageBudgetMS;
	static void NewProp_bAsyncSurfacePageMeshing_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bAsyncSurfacePageMeshing = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAsyncSurfacePageMeshing;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxAsyncSurfacePageJobs;
	static void NewProp_bRealtimeExteriorSurface_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bRealtimeExteriorSurface = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRealtimeExteriorSurface;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxRealtimeExteriorPagesPerFrame;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RealtimeExteriorBudgetMS;
	static const UECodeGen_Private::FIntPropertyParams NewProp_AsyncSurfacePageJobsInFlight;
	static const UECodeGen_Private::FIntPropertyParams NewProp_AsyncSurfacePagesCompleted;
	static const UECodeGen_Private::FIntPropertyParams NewProp_AsyncSurfaceStaleResults;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_LastAsyncSurfaceWorkerMS;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_LastExteriorSurfaceMS;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastExteriorPagesUpdated;
	static void NewProp_bEnableReferenceSurfaceMemoryGovernor_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bEnableReferenceSurfaceMemoryGovernor = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableReferenceSurfaceMemoryGovernor;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxResidentReferenceSurfacePages;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReferenceSurfaceBudgetMB;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxReferencePageQuads;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RealtimeExteriorMaxHzPerPage;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReferenceSurfaceEvictions;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReferenceSurfaceComplexityRejects;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_EstimatedReferenceSurfaceMB;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ResidentReferenceSurfacePages;
	static void NewProp_bKeepLegacyChunkSurfaceReference_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bKeepLegacyChunkSurfaceReference = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bKeepLegacyChunkSurfaceReference;
	static const UECodeGen_Private::FIntPropertyParams NewProp_PendingSurfacePageCount;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastSurfacePagesProcessed;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ActiveSurfacePageCount;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_LastSurfacePageMS;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastSurfacePageQuads;
	static void NewProp_bUploadChangedPagesToGPU_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bUploadChangedPagesToGPU = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUploadChangedPagesToGPU;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_LastActualGPUDeltaBytes;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_TotalActualGPUDeltaBytes;
	static const UECodeGen_Private::FIntPropertyParams NewProp_GPUDeltaPacketCount;
	static void NewProp_bUseHierarchicalBulkEdits_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bUseHierarchicalBulkEdits = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseHierarchicalBulkEdits;
	static void NewProp_bThroughputFirstEditing_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bThroughputFirstEditing = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bThroughputFirstEditing;
	static void NewProp_bDeferReferenceSurfaceWhileBeam_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bDeferReferenceSurfaceWhileBeam = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bDeferReferenceSurfaceWhileBeam;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxBeamCarvesPerFrame;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ThroughputBeamMinEditsPerSecond;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MacroBlastRadiusMM;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MacroBlastCount;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastBulkPagesCleared;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastBulkChunksCleared;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastBulkBlocksCleared;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastBulkBoundaryBlocks;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastBulkSubBlocks2mmCleared;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_LastBulkFineVoxelTests;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastBeamCarvesThisFrame;
	static void NewProp_bUseRoboticsDirectFieldRenderer_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bUseRoboticsDirectFieldRenderer = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseRoboticsDirectFieldRenderer;
	static void NewProp_bDisableLegacyRuntimeVisualsInRoboticsMode_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bDisableLegacyRuntimeVisualsInRoboticsMode = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bDisableLegacyRuntimeVisualsInRoboticsMode;
	static const UECodeGen_Private::FStrPropertyParams NewProp_DirectFieldRuntimeMode;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RoboticsFieldSnapshotHz;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_LastRoboticsFieldSnapshotMS;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_LastRoboticsFieldSnapshotBytes;
	static const UECodeGen_Private::FIntPropertyParams NewProp_RoboticsFieldPartialPages;
	static const UECodeGen_Private::FIntPropertyParams NewProp_RoboticsFieldExplicitChunks;
	static const UECodeGen_Private::FIntPropertyParams NewProp_RoboticsFieldMixedBlocks;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LastRoboticsFieldStats;
	static void NewProp_bEnableFirstPersonCarving_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bEnableFirstPersonCarving = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableFirstPersonCarving;
	static void NewProp_bContinuousBeamDigging_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bContinuousBeamDigging = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bContinuousBeamDigging;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_BeamEditsPerSecond;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_BeamCarveRadiusMM;
	static const UECodeGen_Private::FIntPropertyParams NewProp_BeamVisualBackpressureBlocks;
	static void NewProp_bUseHierarchicalRayTraversal_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bUseHierarchicalRayTraversal = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseHierarchicalRayTraversal;
	static void NewProp_bImmediateImpactSurfaceRefresh_SetBit(void* Obj)
	{
		((ACicadaVoxelVolumeActor*)Obj)->bImmediateImpactSurfaceRefresh = 1;
	}
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bImmediateImpactSurfaceRefresh;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FirstPersonCarveRadiusMM;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FirstPersonMaxDistanceCM;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_LastRayQueryMS;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastRayChunkSkips16mm;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastRayBlockSkips4mm;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LastRayFineSteps1mm;
	static const UECodeGen_Private::FIntPropertyParams NewProp_BeamThrottleFrames;
	static const UECodeGen_Private::FIntPropertyParams NewProp_BeamCarvesThisHold;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_LastImmediateSurfaceMS;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LastStats;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LastVisualStats;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Last1mmWitness;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SceneRoot;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CoarseChunkInstances;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ExactColdShellInstances;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FrontierChunkInstances;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FullBlockInstances;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FineVoxelInstances;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SurfaceMeshComponents_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_SurfaceMeshComponents;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SurfacePageComponents_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_SurfacePageComponents;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class ACicadaVoxelVolumeActor constinit property declarations ********************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("CarveSphereLocalMM"), .Pointer = &ACicadaVoxelVolumeActor::execCarveSphereLocalMM },
		{ .NameUTF8 = UTF8TEXT("CarveVoxelLocalMM"), .Pointer = &ACicadaVoxelVolumeActor::execCarveVoxelLocalMM },
		{ .NameUTF8 = UTF8TEXT("GetStatsString"), .Pointer = &ACicadaVoxelVolumeActor::execGetStatsString },
		{ .NameUTF8 = UTF8TEXT("IsSolidLocalMM"), .Pointer = &ACicadaVoxelVolumeActor::execIsSolidLocalMM },
		{ .NameUTF8 = UTF8TEXT("RebuildVisualization"), .Pointer = &ACicadaVoxelVolumeActor::execRebuildVisualization },
		{ .NameUTF8 = UTF8TEXT("ResetProof"), .Pointer = &ACicadaVoxelVolumeActor::execResetProof },
		{ .NameUTF8 = UTF8TEXT("Run1mmWitnessProof"), .Pointer = &ACicadaVoxelVolumeActor::execRun1mmWitnessProof },
		{ .NameUTF8 = UTF8TEXT("RunDefaultProof"), .Pointer = &ACicadaVoxelVolumeActor::execRunDefaultProof },
	};
	static FTypeConstructFunc* DependentSingletons[];
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ACicadaVoxelVolumeActor_CarveSphereLocalMM, "CarveSphereLocalMM" }, // 79e04bcd6f5bf6a9e5c26ea186983d7e7217adfe
		{ &Z_Construct_UFunction_ACicadaVoxelVolumeActor_CarveVoxelLocalMM, "CarveVoxelLocalMM" }, // 284d51870c70bcdea89ba505911f58175ed573cb
		{ &Z_Construct_UFunction_ACicadaVoxelVolumeActor_GetStatsString, "GetStatsString" }, // 84a1edb0ca95bf079626d702d1545a6cf7d39d94
		{ &Z_Construct_UFunction_ACicadaVoxelVolumeActor_IsSolidLocalMM, "IsSolidLocalMM" }, // 1c7d3443a523355a02ef1e1f0dca3e07588f9bc7
		{ &Z_Construct_UFunction_ACicadaVoxelVolumeActor_RebuildVisualization, "RebuildVisualization" }, // edba0acc588ef7f6e4a264cdf30f2f569a24139c
		{ &Z_Construct_UFunction_ACicadaVoxelVolumeActor_ResetProof, "ResetProof" }, // 25a6a10d78788aae2349c8a7e920642fcc467a87
		{ &Z_Construct_UFunction_ACicadaVoxelVolumeActor_Run1mmWitnessProof, "Run1mmWitnessProof" }, // caf00d7921f53420e36cd0fba455e12bcaeb531b
		{ &Z_Construct_UFunction_ACicadaVoxelVolumeActor_RunDefaultProof, "RunDefaultProof" }, // 83c01a785af22becf48539b7aff04cb834e7a45d
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACicadaVoxelVolumeActor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct UHT_STATICS

// ********** Begin Class ACicadaVoxelVolumeActor Property Definitions *****************************
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bEnableEditorPreview = { "bEnableEditorPreview", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bEnableEditorPreview_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableEditorPreview_MetaData), NewProp_bEnableEditorPreview_MetaData) };
const UECodeGen_Private::FBytePropertyParams UHT_STATICS::NewProp_EditorPreviewMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams UHT_STATICS::NewProp_EditorPreviewMode = { "EditorPreviewMode", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, EditorPreviewMode), Z_Construct_UEnum_CicadaVoxel_ECVXEditorPreviewMode, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EditorPreviewMode_MetaData), NewProp_EditorPreviewMode_MetaData) }; // 7d28477de6a0f879d84596516dd79918fd5814d4
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_LastEditorPreview = { "LastEditorPreview", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastEditorPreview), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastEditorPreview_MetaData), NewProp_LastEditorPreview_MetaData) };
const UECodeGen_Private::FStructPropertyParams UHT_STATICS::NewProp_LogicalSizeMM = { "LogicalSizeMM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LogicalSizeMM), Z_Construct_UScriptStruct_FIntVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LogicalSizeMM_MetaData), NewProp_LogicalSizeMM_MetaData) };
const UECodeGen_Private::FStructPropertyParams UHT_STATICS::NewProp_ProofCenterMM = { "ProofCenterMM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ProofCenterMM), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ProofCenterMM_MetaData), NewProp_ProofCenterMM_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_ProofSphereRadiusMM = { "ProofSphereRadiusMM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ProofSphereRadiusMM), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ProofSphereRadiusMM_MetaData), NewProp_ProofSphereRadiusMM_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bAutoRunProofOnBeginPlay = { "bAutoRunProofOnBeginPlay", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bAutoRunProofOnBeginPlay_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAutoRunProofOnBeginPlay_MetaData), NewProp_bAutoRunProofOnBeginPlay_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bInclude1mmWitnessInDefaultProof = { "bInclude1mmWitnessInDefaultProof", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bInclude1mmWitnessInDefaultProof_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bInclude1mmWitnessInDefaultProof_MetaData), NewProp_bInclude1mmWitnessInDefaultProof_MetaData) };
const UECodeGen_Private::FStructPropertyParams UHT_STATICS::NewProp_WitnessPatchOriginMM = { "WitnessPatchOriginMM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, WitnessPatchOriginMM), Z_Construct_UScriptStruct_FIntVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WitnessPatchOriginMM_MetaData), NewProp_WitnessPatchOriginMM_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_WitnessPatchSizeMM = { "WitnessPatchSizeMM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, WitnessPatchSizeMM), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WitnessPatchSizeMM_MetaData), NewProp_WitnessPatchSizeMM_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_FineVoxelVisualScale = { "FineVoxelVisualScale", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, FineVoxelVisualScale), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FineVoxelVisualScale_MetaData), NewProp_FineVoxelVisualScale_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bForceSeamlessRuntimeFineVoxels = { "bForceSeamlessRuntimeFineVoxels", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bForceSeamlessRuntimeFineVoxels_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bForceSeamlessRuntimeFineVoxels_MetaData), NewProp_bForceSeamlessRuntimeFineVoxels_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bRefineExposedHotSurfacesTo1mm = { "bRefineExposedHotSurfacesTo1mm", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bRefineExposedHotSurfacesTo1mm_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRefineExposedHotSurfacesTo1mm_MetaData), NewProp_bRefineExposedHotSurfacesTo1mm_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bCullOccludedFineVoxels = { "bCullOccludedFineVoxels", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bCullOccludedFineVoxels_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCullOccludedFineVoxels_MetaData), NewProp_bCullOccludedFineVoxels_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_SolidFrontierDepthChunks = { "SolidFrontierDepthChunks", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, SolidFrontierDepthChunks), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SolidFrontierDepthChunks_MetaData), NewProp_SolidFrontierDepthChunks_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_MaxVisualInstances = { "MaxVisualInstances", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, MaxVisualInstances), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxVisualInstances_MetaData), NewProp_MaxVisualInstances_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bUseIncrementalVisualUpdates = { "bUseIncrementalVisualUpdates", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bUseIncrementalVisualUpdates_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseIncrementalVisualUpdates_MetaData), NewProp_bUseIncrementalVisualUpdates_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_VisualChunkBudgetPerFrame = { "VisualChunkBudgetPerFrame", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, VisualChunkBudgetPerFrame), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_VisualChunkBudgetPerFrame_MetaData), NewProp_VisualChunkBudgetPerFrame_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_VisualUpdateBudgetMS = { "VisualUpdateBudgetMS", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, VisualUpdateBudgetMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_VisualUpdateBudgetMS_MetaData), NewProp_VisualUpdateBudgetMS_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_MaxVisualBlocksPerFrame = { "MaxVisualBlocksPerFrame", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, MaxVisualBlocksPerFrame), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxVisualBlocksPerFrame_MetaData), NewProp_MaxVisualBlocksPerFrame_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_HotBlockReserveInstances = { "HotBlockReserveInstances", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, HotBlockReserveInstances), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HotBlockReserveInstances_MetaData), NewProp_HotBlockReserveInstances_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_HotFineReserveInstances = { "HotFineReserveInstances", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, HotFineReserveInstances), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HotFineReserveInstances_MetaData), NewProp_HotFineReserveInstances_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bAsyncHISMTreeBuilds = { "bAsyncHISMTreeBuilds", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bAsyncHISMTreeBuilds_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAsyncHISMTreeBuilds_MetaData), NewProp_bAsyncHISMTreeBuilds_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bPublishLegacyFullGPUSnapshot = { "bPublishLegacyFullGPUSnapshot", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bPublishLegacyFullGPUSnapshot_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bPublishLegacyFullGPUSnapshot_MetaData), NewProp_bPublishLegacyFullGPUSnapshot_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_RuntimeStatsPublishInterval = { "RuntimeStatsPublishInterval", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, RuntimeStatsPublishInterval), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RuntimeStatsPublishInterval_MetaData), NewProp_RuntimeStatsPublishInterval_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bUseExactSurfaceMeshRuntime = { "bUseExactSurfaceMeshRuntime", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bUseExactSurfaceMeshRuntime_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseExactSurfaceMeshRuntime_MetaData), NewProp_bUseExactSurfaceMeshRuntime_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bGreedySurfaceMeshing = { "bGreedySurfaceMeshing", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bGreedySurfaceMeshing_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bGreedySurfaceMeshing_MetaData), NewProp_bGreedySurfaceMeshing_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bTwoSidedExactSurface = { "bTwoSidedExactSurface", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bTwoSidedExactSurface_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bTwoSidedExactSurface_MetaData), NewProp_bTwoSidedExactSurface_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bExactSurfaceShadows = { "bExactSurfaceShadows", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bExactSurfaceShadows_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bExactSurfaceShadows_MetaData), NewProp_bExactSurfaceShadows_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bSafeSurfaceHandoff = { "bSafeSurfaceHandoff", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bSafeSurfaceHandoff_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bSafeSurfaceHandoff_MetaData), NewProp_bSafeSurfaceHandoff_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bSingleOwnerRuntimeSurface = { "bSingleOwnerRuntimeSurface", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bSingleOwnerRuntimeSurface_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bSingleOwnerRuntimeSurface_MetaData), NewProp_bSingleOwnerRuntimeSurface_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_SurfaceProxyRetirements = { "SurfaceProxyRetirements", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, SurfaceProxyRetirements), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SurfaceProxyRetirements_MetaData), NewProp_SurfaceProxyRetirements_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bImmediateImpactNeighbourMeshes = { "bImmediateImpactNeighbourMeshes", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bImmediateImpactNeighbourMeshes_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bImmediateImpactNeighbourMeshes_MetaData), NewProp_bImmediateImpactNeighbourMeshes_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_SurfaceMeshBudgetMS = { "SurfaceMeshBudgetMS", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, SurfaceMeshBudgetMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SurfaceMeshBudgetMS_MetaData), NewProp_SurfaceMeshBudgetMS_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_MaxSurfaceMeshChunksPerFrame = { "MaxSurfaceMeshChunksPerFrame", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, MaxSurfaceMeshChunksPerFrame), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxSurfaceMeshChunksPerFrame_MetaData), NewProp_MaxSurfaceMeshChunksPerFrame_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bImmediateImpactSurfaceMesh = { "bImmediateImpactSurfaceMesh", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bImmediateImpactSurfaceMesh_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bImmediateImpactSurfaceMesh_MetaData), NewProp_bImmediateImpactSurfaceMesh_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_BeamSurfaceMeshBackpressureChunks = { "BeamSurfaceMeshBackpressureChunks", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, BeamSurfaceMeshBackpressureChunks), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BeamSurfaceMeshBackpressureChunks_MetaData), NewProp_BeamSurfaceMeshBackpressureChunks_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_LastSurfaceMeshUpdateMS = { "LastSurfaceMeshUpdateMS", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastSurfaceMeshUpdateMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastSurfaceMeshUpdateMS_MetaData), NewProp_LastSurfaceMeshUpdateMS_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_PendingSurfaceMeshChunkCount = { "PendingSurfaceMeshChunkCount", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, PendingSurfaceMeshChunkCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PendingSurfaceMeshChunkCount_MetaData), NewProp_PendingSurfaceMeshChunkCount_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastSurfaceMeshChunksProcessed = { "LastSurfaceMeshChunksProcessed", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastSurfaceMeshChunksProcessed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastSurfaceMeshChunksProcessed_MetaData), NewProp_LastSurfaceMeshChunksProcessed_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastSurfaceMeshQuads = { "LastSurfaceMeshQuads", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastSurfaceMeshQuads), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastSurfaceMeshQuads_MetaData), NewProp_LastSurfaceMeshQuads_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastSurfaceMeshTriangles = { "LastSurfaceMeshTriangles", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastSurfaceMeshTriangles), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastSurfaceMeshTriangles_MetaData), NewProp_LastSurfaceMeshTriangles_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_ActiveSurfaceMeshChunks = { "ActiveSurfaceMeshChunks", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ActiveSurfaceMeshChunks), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActiveSurfaceMeshChunks_MetaData), NewProp_ActiveSurfaceMeshChunks_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_SurfaceHandoffGuards = { "SurfaceHandoffGuards", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, SurfaceHandoffGuards), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SurfaceHandoffGuards_MetaData), NewProp_SurfaceHandoffGuards_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_SurfaceImmediateNeighbourRebuilds = { "SurfaceImmediateNeighbourRebuilds", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, SurfaceImmediateNeighbourRebuilds), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SurfaceImmediateNeighbourRebuilds_MetaData), NewProp_SurfaceImmediateNeighbourRebuilds_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_LastSparseEditMS = { "LastSparseEditMS", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastSparseEditMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastSparseEditMS_MetaData), NewProp_LastSparseEditMS_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_LastVisualUpdateMS = { "LastVisualUpdateMS", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastVisualUpdateMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastVisualUpdateMS_MetaData), NewProp_LastVisualUpdateMS_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_LastFullVisualRebuildMS = { "LastFullVisualRebuildMS", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastFullVisualRebuildMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastFullVisualRebuildMS_MetaData), NewProp_LastFullVisualRebuildMS_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_PendingVisualChunkCount = { "PendingVisualChunkCount", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, PendingVisualChunkCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PendingVisualChunkCount_MetaData), NewProp_PendingVisualChunkCount_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_PendingVisualBlockCount = { "PendingVisualBlockCount", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, PendingVisualBlockCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PendingVisualBlockCount_MetaData), NewProp_PendingVisualBlockCount_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastVisualBlocksProcessed = { "LastVisualBlocksProcessed", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastVisualBlocksProcessed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastVisualBlocksProcessed_MetaData), NewProp_LastVisualBlocksProcessed_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastRefinedSurfaceBlocksProcessed = { "LastRefinedSurfaceBlocksProcessed", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastRefinedSurfaceBlocksProcessed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastRefinedSurfaceBlocksProcessed_MetaData), NewProp_LastRefinedSurfaceBlocksProcessed_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_FineSlotBatchesReused = { "FineSlotBatchesReused", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, FineSlotBatchesReused), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FineSlotBatchesReused_MetaData), NewProp_FineSlotBatchesReused_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_LastLegacyGPUSnapshotMS = { "LastLegacyGPUSnapshotMS", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastLegacyGPUSnapshotMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastLegacyGPUSnapshotMS_MetaData), NewProp_LastLegacyGPUSnapshotMS_MetaData) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_LastPerformanceStats = { "LastPerformanceStats", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastPerformanceStats), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastPerformanceStats_MetaData), NewProp_LastPerformanceStats_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bEnableVirtualPageSpine = { "bEnableVirtualPageSpine", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bEnableVirtualPageSpine_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableVirtualPageSpine_MetaData), NewProp_bEnableVirtualPageSpine_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_VirtualPageEdgeMM = { "VirtualPageEdgeMM", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, VirtualPageEdgeMM), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_VirtualPageEdgeMM_MetaData), NewProp_VirtualPageEdgeMM_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_ResidentEditedPageCount = { "ResidentEditedPageCount", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ResidentEditedPageCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResidentEditedPageCount_MetaData), NewProp_ResidentEditedPageCount_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_ExplicitPageChunkCount = { "ExplicitPageChunkCount", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ExplicitPageChunkCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ExplicitPageChunkCount_MetaData), NewProp_ExplicitPageChunkCount_MetaData) };
const UECodeGen_Private::FInt64PropertyParams UHT_STATICS::NewProp_LastPageDeltaBytes = { "LastPageDeltaBytes", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int64, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastPageDeltaBytes), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastPageDeltaBytes_MetaData), NewProp_LastPageDeltaBytes_MetaData) };
const UECodeGen_Private::FInt64PropertyParams UHT_STATICS::NewProp_FullWorldHotUploadCount = { "FullWorldHotUploadCount", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int64, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, FullWorldHotUploadCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FullWorldHotUploadCount_MetaData), NewProp_FullWorldHotUploadCount_MetaData) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_LastVirtualPageStats = { "LastVirtualPageStats", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastVirtualPageStats), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastVirtualPageStats_MetaData), NewProp_LastVirtualPageStats_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bUsePageNativeRuntime = { "bUsePageNativeRuntime", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bUsePageNativeRuntime_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUsePageNativeRuntime_MetaData), NewProp_bUsePageNativeRuntime_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_MaxSurfacePagesPerFrame = { "MaxSurfacePagesPerFrame", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, MaxSurfacePagesPerFrame), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxSurfacePagesPerFrame_MetaData), NewProp_MaxSurfacePagesPerFrame_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_SurfacePageBudgetMS = { "SurfacePageBudgetMS", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, SurfacePageBudgetMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SurfacePageBudgetMS_MetaData), NewProp_SurfacePageBudgetMS_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bAsyncSurfacePageMeshing = { "bAsyncSurfacePageMeshing", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bAsyncSurfacePageMeshing_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAsyncSurfacePageMeshing_MetaData), NewProp_bAsyncSurfacePageMeshing_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_MaxAsyncSurfacePageJobs = { "MaxAsyncSurfacePageJobs", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, MaxAsyncSurfacePageJobs), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxAsyncSurfacePageJobs_MetaData), NewProp_MaxAsyncSurfacePageJobs_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bRealtimeExteriorSurface = { "bRealtimeExteriorSurface", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bRealtimeExteriorSurface_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRealtimeExteriorSurface_MetaData), NewProp_bRealtimeExteriorSurface_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_MaxRealtimeExteriorPagesPerFrame = { "MaxRealtimeExteriorPagesPerFrame", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, MaxRealtimeExteriorPagesPerFrame), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxRealtimeExteriorPagesPerFrame_MetaData), NewProp_MaxRealtimeExteriorPagesPerFrame_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_RealtimeExteriorBudgetMS = { "RealtimeExteriorBudgetMS", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, RealtimeExteriorBudgetMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RealtimeExteriorBudgetMS_MetaData), NewProp_RealtimeExteriorBudgetMS_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_AsyncSurfacePageJobsInFlight = { "AsyncSurfacePageJobsInFlight", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, AsyncSurfacePageJobsInFlight), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AsyncSurfacePageJobsInFlight_MetaData), NewProp_AsyncSurfacePageJobsInFlight_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_AsyncSurfacePagesCompleted = { "AsyncSurfacePagesCompleted", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, AsyncSurfacePagesCompleted), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AsyncSurfacePagesCompleted_MetaData), NewProp_AsyncSurfacePagesCompleted_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_AsyncSurfaceStaleResults = { "AsyncSurfaceStaleResults", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, AsyncSurfaceStaleResults), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AsyncSurfaceStaleResults_MetaData), NewProp_AsyncSurfaceStaleResults_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_LastAsyncSurfaceWorkerMS = { "LastAsyncSurfaceWorkerMS", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastAsyncSurfaceWorkerMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastAsyncSurfaceWorkerMS_MetaData), NewProp_LastAsyncSurfaceWorkerMS_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_LastExteriorSurfaceMS = { "LastExteriorSurfaceMS", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastExteriorSurfaceMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastExteriorSurfaceMS_MetaData), NewProp_LastExteriorSurfaceMS_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastExteriorPagesUpdated = { "LastExteriorPagesUpdated", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastExteriorPagesUpdated), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastExteriorPagesUpdated_MetaData), NewProp_LastExteriorPagesUpdated_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bEnableReferenceSurfaceMemoryGovernor = { "bEnableReferenceSurfaceMemoryGovernor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bEnableReferenceSurfaceMemoryGovernor_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableReferenceSurfaceMemoryGovernor_MetaData), NewProp_bEnableReferenceSurfaceMemoryGovernor_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_MaxResidentReferenceSurfacePages = { "MaxResidentReferenceSurfacePages", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, MaxResidentReferenceSurfacePages), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxResidentReferenceSurfacePages_MetaData), NewProp_MaxResidentReferenceSurfacePages_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_ReferenceSurfaceBudgetMB = { "ReferenceSurfaceBudgetMB", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ReferenceSurfaceBudgetMB), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReferenceSurfaceBudgetMB_MetaData), NewProp_ReferenceSurfaceBudgetMB_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_MaxReferencePageQuads = { "MaxReferencePageQuads", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, MaxReferencePageQuads), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxReferencePageQuads_MetaData), NewProp_MaxReferencePageQuads_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_RealtimeExteriorMaxHzPerPage = { "RealtimeExteriorMaxHzPerPage", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, RealtimeExteriorMaxHzPerPage), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RealtimeExteriorMaxHzPerPage_MetaData), NewProp_RealtimeExteriorMaxHzPerPage_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_ReferenceSurfaceEvictions = { "ReferenceSurfaceEvictions", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ReferenceSurfaceEvictions), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReferenceSurfaceEvictions_MetaData), NewProp_ReferenceSurfaceEvictions_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_ReferenceSurfaceComplexityRejects = { "ReferenceSurfaceComplexityRejects", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ReferenceSurfaceComplexityRejects), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReferenceSurfaceComplexityRejects_MetaData), NewProp_ReferenceSurfaceComplexityRejects_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_EstimatedReferenceSurfaceMB = { "EstimatedReferenceSurfaceMB", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, EstimatedReferenceSurfaceMB), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EstimatedReferenceSurfaceMB_MetaData), NewProp_EstimatedReferenceSurfaceMB_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_ResidentReferenceSurfacePages = { "ResidentReferenceSurfacePages", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ResidentReferenceSurfacePages), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResidentReferenceSurfacePages_MetaData), NewProp_ResidentReferenceSurfacePages_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bKeepLegacyChunkSurfaceReference = { "bKeepLegacyChunkSurfaceReference", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bKeepLegacyChunkSurfaceReference_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bKeepLegacyChunkSurfaceReference_MetaData), NewProp_bKeepLegacyChunkSurfaceReference_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_PendingSurfacePageCount = { "PendingSurfacePageCount", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, PendingSurfacePageCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PendingSurfacePageCount_MetaData), NewProp_PendingSurfacePageCount_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastSurfacePagesProcessed = { "LastSurfacePagesProcessed", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastSurfacePagesProcessed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastSurfacePagesProcessed_MetaData), NewProp_LastSurfacePagesProcessed_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_ActiveSurfacePageCount = { "ActiveSurfacePageCount", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ActiveSurfacePageCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActiveSurfacePageCount_MetaData), NewProp_ActiveSurfacePageCount_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_LastSurfacePageMS = { "LastSurfacePageMS", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastSurfacePageMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastSurfacePageMS_MetaData), NewProp_LastSurfacePageMS_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastSurfacePageQuads = { "LastSurfacePageQuads", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastSurfacePageQuads), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastSurfacePageQuads_MetaData), NewProp_LastSurfacePageQuads_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bUploadChangedPagesToGPU = { "bUploadChangedPagesToGPU", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bUploadChangedPagesToGPU_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUploadChangedPagesToGPU_MetaData), NewProp_bUploadChangedPagesToGPU_MetaData) };
const UECodeGen_Private::FInt64PropertyParams UHT_STATICS::NewProp_LastActualGPUDeltaBytes = { "LastActualGPUDeltaBytes", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int64, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastActualGPUDeltaBytes), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastActualGPUDeltaBytes_MetaData), NewProp_LastActualGPUDeltaBytes_MetaData) };
const UECodeGen_Private::FInt64PropertyParams UHT_STATICS::NewProp_TotalActualGPUDeltaBytes = { "TotalActualGPUDeltaBytes", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int64, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, TotalActualGPUDeltaBytes), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TotalActualGPUDeltaBytes_MetaData), NewProp_TotalActualGPUDeltaBytes_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_GPUDeltaPacketCount = { "GPUDeltaPacketCount", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, GPUDeltaPacketCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GPUDeltaPacketCount_MetaData), NewProp_GPUDeltaPacketCount_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bUseHierarchicalBulkEdits = { "bUseHierarchicalBulkEdits", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bUseHierarchicalBulkEdits_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseHierarchicalBulkEdits_MetaData), NewProp_bUseHierarchicalBulkEdits_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bThroughputFirstEditing = { "bThroughputFirstEditing", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bThroughputFirstEditing_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bThroughputFirstEditing_MetaData), NewProp_bThroughputFirstEditing_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bDeferReferenceSurfaceWhileBeam = { "bDeferReferenceSurfaceWhileBeam", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bDeferReferenceSurfaceWhileBeam_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bDeferReferenceSurfaceWhileBeam_MetaData), NewProp_bDeferReferenceSurfaceWhileBeam_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_MaxBeamCarvesPerFrame = { "MaxBeamCarvesPerFrame", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, MaxBeamCarvesPerFrame), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxBeamCarvesPerFrame_MetaData), NewProp_MaxBeamCarvesPerFrame_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_ThroughputBeamMinEditsPerSecond = { "ThroughputBeamMinEditsPerSecond", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ThroughputBeamMinEditsPerSecond), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ThroughputBeamMinEditsPerSecond_MetaData), NewProp_ThroughputBeamMinEditsPerSecond_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_MacroBlastRadiusMM = { "MacroBlastRadiusMM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, MacroBlastRadiusMM), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MacroBlastRadiusMM_MetaData), NewProp_MacroBlastRadiusMM_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_MacroBlastCount = { "MacroBlastCount", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, MacroBlastCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MacroBlastCount_MetaData), NewProp_MacroBlastCount_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastBulkPagesCleared = { "LastBulkPagesCleared", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastBulkPagesCleared), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastBulkPagesCleared_MetaData), NewProp_LastBulkPagesCleared_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastBulkChunksCleared = { "LastBulkChunksCleared", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastBulkChunksCleared), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastBulkChunksCleared_MetaData), NewProp_LastBulkChunksCleared_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastBulkBlocksCleared = { "LastBulkBlocksCleared", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastBulkBlocksCleared), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastBulkBlocksCleared_MetaData), NewProp_LastBulkBlocksCleared_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastBulkBoundaryBlocks = { "LastBulkBoundaryBlocks", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastBulkBoundaryBlocks), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastBulkBoundaryBlocks_MetaData), NewProp_LastBulkBoundaryBlocks_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastBulkSubBlocks2mmCleared = { "LastBulkSubBlocks2mmCleared", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastBulkSubBlocks2mmCleared), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastBulkSubBlocks2mmCleared_MetaData), NewProp_LastBulkSubBlocks2mmCleared_MetaData) };
const UECodeGen_Private::FInt64PropertyParams UHT_STATICS::NewProp_LastBulkFineVoxelTests = { "LastBulkFineVoxelTests", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int64, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastBulkFineVoxelTests), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastBulkFineVoxelTests_MetaData), NewProp_LastBulkFineVoxelTests_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastBeamCarvesThisFrame = { "LastBeamCarvesThisFrame", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastBeamCarvesThisFrame), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastBeamCarvesThisFrame_MetaData), NewProp_LastBeamCarvesThisFrame_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bUseRoboticsDirectFieldRenderer = { "bUseRoboticsDirectFieldRenderer", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bUseRoboticsDirectFieldRenderer_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseRoboticsDirectFieldRenderer_MetaData), NewProp_bUseRoboticsDirectFieldRenderer_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bDisableLegacyRuntimeVisualsInRoboticsMode = { "bDisableLegacyRuntimeVisualsInRoboticsMode", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bDisableLegacyRuntimeVisualsInRoboticsMode_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bDisableLegacyRuntimeVisualsInRoboticsMode_MetaData), NewProp_bDisableLegacyRuntimeVisualsInRoboticsMode_MetaData) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_DirectFieldRuntimeMode = { "DirectFieldRuntimeMode", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, DirectFieldRuntimeMode), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DirectFieldRuntimeMode_MetaData), NewProp_DirectFieldRuntimeMode_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_RoboticsFieldSnapshotHz = { "RoboticsFieldSnapshotHz", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, RoboticsFieldSnapshotHz), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RoboticsFieldSnapshotHz_MetaData), NewProp_RoboticsFieldSnapshotHz_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_LastRoboticsFieldSnapshotMS = { "LastRoboticsFieldSnapshotMS", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastRoboticsFieldSnapshotMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastRoboticsFieldSnapshotMS_MetaData), NewProp_LastRoboticsFieldSnapshotMS_MetaData) };
const UECodeGen_Private::FInt64PropertyParams UHT_STATICS::NewProp_LastRoboticsFieldSnapshotBytes = { "LastRoboticsFieldSnapshotBytes", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int64, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastRoboticsFieldSnapshotBytes), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastRoboticsFieldSnapshotBytes_MetaData), NewProp_LastRoboticsFieldSnapshotBytes_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_RoboticsFieldPartialPages = { "RoboticsFieldPartialPages", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, RoboticsFieldPartialPages), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RoboticsFieldPartialPages_MetaData), NewProp_RoboticsFieldPartialPages_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_RoboticsFieldExplicitChunks = { "RoboticsFieldExplicitChunks", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, RoboticsFieldExplicitChunks), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RoboticsFieldExplicitChunks_MetaData), NewProp_RoboticsFieldExplicitChunks_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_RoboticsFieldMixedBlocks = { "RoboticsFieldMixedBlocks", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, RoboticsFieldMixedBlocks), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RoboticsFieldMixedBlocks_MetaData), NewProp_RoboticsFieldMixedBlocks_MetaData) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_LastRoboticsFieldStats = { "LastRoboticsFieldStats", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastRoboticsFieldStats), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastRoboticsFieldStats_MetaData), NewProp_LastRoboticsFieldStats_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bEnableFirstPersonCarving = { "bEnableFirstPersonCarving", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bEnableFirstPersonCarving_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableFirstPersonCarving_MetaData), NewProp_bEnableFirstPersonCarving_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bContinuousBeamDigging = { "bContinuousBeamDigging", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bContinuousBeamDigging_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bContinuousBeamDigging_MetaData), NewProp_bContinuousBeamDigging_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_BeamEditsPerSecond = { "BeamEditsPerSecond", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, BeamEditsPerSecond), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BeamEditsPerSecond_MetaData), NewProp_BeamEditsPerSecond_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_BeamCarveRadiusMM = { "BeamCarveRadiusMM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, BeamCarveRadiusMM), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BeamCarveRadiusMM_MetaData), NewProp_BeamCarveRadiusMM_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_BeamVisualBackpressureBlocks = { "BeamVisualBackpressureBlocks", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, BeamVisualBackpressureBlocks), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BeamVisualBackpressureBlocks_MetaData), NewProp_BeamVisualBackpressureBlocks_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bUseHierarchicalRayTraversal = { "bUseHierarchicalRayTraversal", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bUseHierarchicalRayTraversal_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseHierarchicalRayTraversal_MetaData), NewProp_bUseHierarchicalRayTraversal_MetaData) };
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bImmediateImpactSurfaceRefresh = { "bImmediateImpactSurfaceRefresh", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(ACicadaVoxelVolumeActor), &UHT_STATICS::NewProp_bImmediateImpactSurfaceRefresh_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bImmediateImpactSurfaceRefresh_MetaData), NewProp_bImmediateImpactSurfaceRefresh_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_FirstPersonCarveRadiusMM = { "FirstPersonCarveRadiusMM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, FirstPersonCarveRadiusMM), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FirstPersonCarveRadiusMM_MetaData), NewProp_FirstPersonCarveRadiusMM_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_FirstPersonMaxDistanceCM = { "FirstPersonMaxDistanceCM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, FirstPersonMaxDistanceCM), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FirstPersonMaxDistanceCM_MetaData), NewProp_FirstPersonMaxDistanceCM_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_LastRayQueryMS = { "LastRayQueryMS", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastRayQueryMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastRayQueryMS_MetaData), NewProp_LastRayQueryMS_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastRayChunkSkips16mm = { "LastRayChunkSkips16mm", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastRayChunkSkips16mm), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastRayChunkSkips16mm_MetaData), NewProp_LastRayChunkSkips16mm_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastRayBlockSkips4mm = { "LastRayBlockSkips4mm", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastRayBlockSkips4mm), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastRayBlockSkips4mm_MetaData), NewProp_LastRayBlockSkips4mm_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_LastRayFineSteps1mm = { "LastRayFineSteps1mm", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastRayFineSteps1mm), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastRayFineSteps1mm_MetaData), NewProp_LastRayFineSteps1mm_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_BeamThrottleFrames = { "BeamThrottleFrames", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, BeamThrottleFrames), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BeamThrottleFrames_MetaData), NewProp_BeamThrottleFrames_MetaData) };
const UECodeGen_Private::FIntPropertyParams UHT_STATICS::NewProp_BeamCarvesThisHold = { "BeamCarvesThisHold", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, BeamCarvesThisHold), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BeamCarvesThisHold_MetaData), NewProp_BeamCarvesThisHold_MetaData) };
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_LastImmediateSurfaceMS = { "LastImmediateSurfaceMS", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastImmediateSurfaceMS), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastImmediateSurfaceMS_MetaData), NewProp_LastImmediateSurfaceMS_MetaData) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_LastStats = { "LastStats", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastStats), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastStats_MetaData), NewProp_LastStats_MetaData) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_LastVisualStats = { "LastVisualStats", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, LastVisualStats), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastVisualStats_MetaData), NewProp_LastVisualStats_MetaData) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_Last1mmWitness = { "Last1mmWitness", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, Last1mmWitness), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Last1mmWitness_MetaData), NewProp_Last1mmWitness_MetaData) };
const UECodeGen_Private::FObjectPropertyParams UHT_STATICS::NewProp_SceneRoot = { "SceneRoot", nullptr, (EPropertyFlags)0x01140000000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, SceneRoot), Z_Construct_UClass_USceneComponent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SceneRoot_MetaData), NewProp_SceneRoot_MetaData) };
const UECodeGen_Private::FObjectPropertyParams UHT_STATICS::NewProp_CoarseChunkInstances = { "CoarseChunkInstances", nullptr, (EPropertyFlags)0x01140000000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, CoarseChunkInstances), Z_Construct_UClass_UHierarchicalInstancedStaticMeshComponent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CoarseChunkInstances_MetaData), NewProp_CoarseChunkInstances_MetaData) };
const UECodeGen_Private::FObjectPropertyParams UHT_STATICS::NewProp_ExactColdShellInstances = { "ExactColdShellInstances", nullptr, (EPropertyFlags)0x01140000000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, ExactColdShellInstances), Z_Construct_UClass_UInstancedStaticMeshComponent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ExactColdShellInstances_MetaData), NewProp_ExactColdShellInstances_MetaData) };
const UECodeGen_Private::FObjectPropertyParams UHT_STATICS::NewProp_FrontierChunkInstances = { "FrontierChunkInstances", nullptr, (EPropertyFlags)0x01140000000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, FrontierChunkInstances), Z_Construct_UClass_UInstancedStaticMeshComponent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FrontierChunkInstances_MetaData), NewProp_FrontierChunkInstances_MetaData) };
const UECodeGen_Private::FObjectPropertyParams UHT_STATICS::NewProp_FullBlockInstances = { "FullBlockInstances", nullptr, (EPropertyFlags)0x01140000000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, FullBlockInstances), Z_Construct_UClass_UInstancedStaticMeshComponent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FullBlockInstances_MetaData), NewProp_FullBlockInstances_MetaData) };
const UECodeGen_Private::FObjectPropertyParams UHT_STATICS::NewProp_FineVoxelInstances = { "FineVoxelInstances", nullptr, (EPropertyFlags)0x01140000000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, FineVoxelInstances), Z_Construct_UClass_UInstancedStaticMeshComponent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FineVoxelInstances_MetaData), NewProp_FineVoxelInstances_MetaData) };
const UECodeGen_Private::FObjectPropertyParams UHT_STATICS::NewProp_SurfaceMeshComponents_Inner = { "SurfaceMeshComponents", nullptr, (EPropertyFlags)0x0104000000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, nullptr, nullptr, 1, 0, Z_Construct_UClass_UDynamicMeshComponent, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams UHT_STATICS::NewProp_SurfaceMeshComponents = { "SurfaceMeshComponents", nullptr, (EPropertyFlags)0x0114008000002008, UECodeGen_Private::EPropertyGenFlags::Array, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, SurfaceMeshComponents), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SurfaceMeshComponents_MetaData), NewProp_SurfaceMeshComponents_MetaData) };
const UECodeGen_Private::FObjectPropertyParams UHT_STATICS::NewProp_SurfacePageComponents_Inner = { "SurfacePageComponents", nullptr, (EPropertyFlags)0x0104000000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, nullptr, nullptr, 1, 0, Z_Construct_UClass_UDynamicMeshComponent, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams UHT_STATICS::NewProp_SurfacePageComponents = { "SurfacePageComponents", nullptr, (EPropertyFlags)0x0114008000002008, UECodeGen_Private::EPropertyGenFlags::Array, nullptr, nullptr, 1, STRUCT_OFFSET(ACicadaVoxelVolumeActor, SurfacePageComponents), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SurfacePageComponents_MetaData), NewProp_SurfacePageComponents_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bEnableEditorPreview,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_EditorPreviewMode_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_EditorPreviewMode,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastEditorPreview,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LogicalSizeMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ProofCenterMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ProofSphereRadiusMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bAutoRunProofOnBeginPlay,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bInclude1mmWitnessInDefaultProof,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_WitnessPatchOriginMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_WitnessPatchSizeMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_FineVoxelVisualScale,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bForceSeamlessRuntimeFineVoxels,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bRefineExposedHotSurfacesTo1mm,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bCullOccludedFineVoxels,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SolidFrontierDepthChunks,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaxVisualInstances,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bUseIncrementalVisualUpdates,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_VisualChunkBudgetPerFrame,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_VisualUpdateBudgetMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaxVisualBlocksPerFrame,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_HotBlockReserveInstances,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_HotFineReserveInstances,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bAsyncHISMTreeBuilds,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bPublishLegacyFullGPUSnapshot,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_RuntimeStatsPublishInterval,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bUseExactSurfaceMeshRuntime,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bGreedySurfaceMeshing,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bTwoSidedExactSurface,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bExactSurfaceShadows,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bSafeSurfaceHandoff,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bSingleOwnerRuntimeSurface,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SurfaceProxyRetirements,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bImmediateImpactNeighbourMeshes,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SurfaceMeshBudgetMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaxSurfaceMeshChunksPerFrame,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bImmediateImpactSurfaceMesh,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_BeamSurfaceMeshBackpressureChunks,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastSurfaceMeshUpdateMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_PendingSurfaceMeshChunkCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastSurfaceMeshChunksProcessed,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastSurfaceMeshQuads,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastSurfaceMeshTriangles,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ActiveSurfaceMeshChunks,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SurfaceHandoffGuards,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SurfaceImmediateNeighbourRebuilds,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastSparseEditMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastVisualUpdateMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastFullVisualRebuildMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_PendingVisualChunkCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_PendingVisualBlockCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastVisualBlocksProcessed,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastRefinedSurfaceBlocksProcessed,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_FineSlotBatchesReused,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastLegacyGPUSnapshotMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastPerformanceStats,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bEnableVirtualPageSpine,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_VirtualPageEdgeMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ResidentEditedPageCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ExplicitPageChunkCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastPageDeltaBytes,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_FullWorldHotUploadCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastVirtualPageStats,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bUsePageNativeRuntime,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaxSurfacePagesPerFrame,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SurfacePageBudgetMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bAsyncSurfacePageMeshing,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaxAsyncSurfacePageJobs,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bRealtimeExteriorSurface,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaxRealtimeExteriorPagesPerFrame,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_RealtimeExteriorBudgetMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_AsyncSurfacePageJobsInFlight,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_AsyncSurfacePagesCompleted,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_AsyncSurfaceStaleResults,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastAsyncSurfaceWorkerMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastExteriorSurfaceMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastExteriorPagesUpdated,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bEnableReferenceSurfaceMemoryGovernor,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaxResidentReferenceSurfacePages,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ReferenceSurfaceBudgetMB,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaxReferencePageQuads,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_RealtimeExteriorMaxHzPerPage,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ReferenceSurfaceEvictions,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ReferenceSurfaceComplexityRejects,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_EstimatedReferenceSurfaceMB,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ResidentReferenceSurfacePages,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bKeepLegacyChunkSurfaceReference,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_PendingSurfacePageCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastSurfacePagesProcessed,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ActiveSurfacePageCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastSurfacePageMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastSurfacePageQuads,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bUploadChangedPagesToGPU,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastActualGPUDeltaBytes,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_TotalActualGPUDeltaBytes,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_GPUDeltaPacketCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bUseHierarchicalBulkEdits,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bThroughputFirstEditing,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bDeferReferenceSurfaceWhileBeam,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaxBeamCarvesPerFrame,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ThroughputBeamMinEditsPerSecond,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MacroBlastRadiusMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MacroBlastCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastBulkPagesCleared,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastBulkChunksCleared,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastBulkBlocksCleared,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastBulkBoundaryBlocks,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastBulkSubBlocks2mmCleared,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastBulkFineVoxelTests,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastBeamCarvesThisFrame,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bUseRoboticsDirectFieldRenderer,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bDisableLegacyRuntimeVisualsInRoboticsMode,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_DirectFieldRuntimeMode,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_RoboticsFieldSnapshotHz,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastRoboticsFieldSnapshotMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastRoboticsFieldSnapshotBytes,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_RoboticsFieldPartialPages,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_RoboticsFieldExplicitChunks,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_RoboticsFieldMixedBlocks,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastRoboticsFieldStats,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bEnableFirstPersonCarving,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bContinuousBeamDigging,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_BeamEditsPerSecond,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_BeamCarveRadiusMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_BeamVisualBackpressureBlocks,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bUseHierarchicalRayTraversal,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bImmediateImpactSurfaceRefresh,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_FirstPersonCarveRadiusMM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_FirstPersonMaxDistanceCM,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastRayQueryMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastRayChunkSkips16mm,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastRayBlockSkips4mm,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastRayFineSteps1mm,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_BeamThrottleFrames,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_BeamCarvesThisHold,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastImmediateSurfaceMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastStats,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_LastVisualStats,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_Last1mmWitness,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SceneRoot,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_CoarseChunkInstances,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ExactColdShellInstances,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_FrontierChunkInstances,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_FullBlockInstances,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_FineVoxelInstances,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SurfaceMeshComponents_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SurfaceMeshComponents,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SurfacePageComponents_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SurfacePageComponents,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Class ACicadaVoxelVolumeActor Property Definitions *******************************
FTypeConstructFunc* UHT_STATICS::DependentSingletons[] = {
	(FTypeConstructFunc*)Z_Construct_UClass_AActor,
	(FTypeConstructFunc*)Z_Construct_UPackage__Script_CicadaVoxel,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams UHT_STATICS::ClassParams = {
	&Z_Construct_UClass_ACicadaVoxelVolumeActor,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	UHT_STATICS::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(UHT_STATICS::PropPointers),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)
};
static void ACicadaVoxelVolumeActor_StaticRegisterNativesACicadaVoxelVolumeActor()
{
	UClass* Class = ACicadaVoxelVolumeActor::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, 		MakeConstArrayView(UHT_STATICS::Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_ACicadaVoxelVolumeActor;
UClass* Z_Construct_UClass_ACicadaVoxelVolumeActor(ETypeConstructPhase Phase)
{
	if (Phase == ETypeConstructPhase::Inner)
	{
		using TClass = ACicadaVoxelVolumeActor;
		if (!Z_Registration_Info_UClass_ACicadaVoxelVolumeActor.InnerSingleton)
		{
			GetPrivateStaticClassBody(
				TClass::StaticPackage(),
				TEXT("CicadaVoxelVolumeActor"),
				Z_Registration_Info_UClass_ACicadaVoxelVolumeActor.InnerSingleton,
				ACicadaVoxelVolumeActor_StaticRegisterNativesACicadaVoxelVolumeActor,
				DataSizeOf<TClass>(),
				alignof(TClass),
				TClass::StaticClassFlags,
				TClass::StaticClassCastFlags(),
				TClass::StaticConfigName(),
				(UClass::ClassConstructorType)InternalConstructor<TClass>,
				(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
				UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
				&TClass::Super::StaticClass,
				&TClass::WithinClass::StaticClass
			);
		}
		return Z_Registration_Info_UClass_ACicadaVoxelVolumeActor.InnerSingleton;
	}
	if (!Z_Registration_Info_UClass_ACicadaVoxelVolumeActor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ACicadaVoxelVolumeActor.OuterSingleton, UHT_STATICS::ClassParams);
	}
	return Z_Registration_Info_UClass_ACicadaVoxelVolumeActor.OuterSingleton;
}
#undef UHT_STATICS
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, ACicadaVoxelVolumeActor);
ACicadaVoxelVolumeActor::~ACicadaVoxelVolumeActor() {}
// ********** End Class ACicadaVoxelVolumeActor ****************************************************

// ********** Begin Registration *******************************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_CompiledInDeferFile_FID_Users_thoma_Documents_Unreal_Projects_MmmVoxels_Plugins_CicadaVoxel_Source_CicadaVoxel_Public_CicadaVoxelVolumeActor_h__Script_CicadaVoxel_Statics
struct UHT_STATICS
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ Z_Construct_UEnum_CicadaVoxel_ECVXEditorPreviewMode, TEXT("ECVXEditorPreviewMode"), &ZRIE_ECVXEditorPreviewMode, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2099791741U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ACicadaVoxelVolumeActor, TEXT("ACicadaVoxelVolumeActor"), &Z_Registration_Info_UClass_ACicadaVoxelVolumeActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ACicadaVoxelVolumeActor), 510206702U) },
	};
}; // UHT_STATICS 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_thoma_Documents_Unreal_Projects_MmmVoxels_Plugins_CicadaVoxel_Source_CicadaVoxel_Public_CicadaVoxelVolumeActor_h__Script_CicadaVoxel_2481650ced618204efda83e0e90fee01c9511159{
	TEXT("/Script/CicadaVoxel"),
	UHT_STATICS::ClassInfo, UE_ARRAY_COUNT(UHT_STATICS::ClassInfo),
	nullptr, 0,
	UHT_STATICS::EnumInfo, UE_ARRAY_COUNT(UHT_STATICS::EnumInfo),
	nullptr, 0,
};
#undef UHT_STATICS
// ********** End Registration *********************************************************************
#undef UHT_STRUCT_BASE

PRAGMA_ENABLE_DEPRECATION_WARNINGS
