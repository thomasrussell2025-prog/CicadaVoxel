#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"

class FCVXApplyFieldPatchesCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FCVXApplyFieldPatchesCS);
    SHADER_USE_PARAMETER_STRUCT(FCVXApplyFieldPatchesCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, PatchCount)
        SHADER_PARAMETER(uint32, PatchStrideWords)

        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<uint>, PatchWords)

        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint>, PageHashRW)
        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint>, PageMetaRW)
        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint>, ChunkStateWordsRW)
        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint>, BlockMasksRW)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};

class FCVXRoboticsFieldPS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FCVXRoboticsFieldPS);
    SHADER_USE_PARAMETER_STRUCT(FCVXRoboticsFieldPS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(FVector4f, ViewRectMinSize)
        SHADER_PARAMETER(FVector4f, LogicalSizeMM_PageEdge)
        SHADER_PARAMETER(FVector4f, PageCounts)
        SHADER_PARAMETER(FVector4f, RayOriginMM)
        SHADER_PARAMETER(FVector4f, RayDirTL)
        SHADER_PARAMETER(FVector4f, RayDirTR)
        SHADER_PARAMETER(FVector4f, RayDirBL)
        SHADER_PARAMETER(FVector4f, RayDirBR)

        SHADER_PARAMETER(uint32, PageHashCapacity)
        SHADER_PARAMETER(uint32, PageCapacity)

        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<uint>, PageHash)
        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<uint>, PageMeta)
        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<uint>, ChunkStateWords)
        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<uint>, BlockMasks)

        RENDER_TARGET_BINDING_SLOTS()
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};
