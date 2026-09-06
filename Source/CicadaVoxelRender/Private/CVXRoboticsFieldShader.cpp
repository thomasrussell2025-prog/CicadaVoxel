#include "CVXRoboticsFieldShader.h"

IMPLEMENT_GLOBAL_SHADER(
    FCVXApplyFieldPatchesCS,
    "/Plugin/CicadaVoxel/Private/CVXRoboticsField.usf",
    "ApplyFieldPatchesCS",
    SF_Compute);

IMPLEMENT_GLOBAL_SHADER(
    FCVXRoboticsFieldPS,
    "/Plugin/CicadaVoxel/Private/CVXRoboticsField.usf",
    "MainPS",
    SF_Pixel);
