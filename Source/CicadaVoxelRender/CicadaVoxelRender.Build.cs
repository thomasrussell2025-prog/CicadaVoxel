using UnrealBuildTool;

public class CicadaVoxelRender : ModuleRules
{
    public CicadaVoxelRender(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Engine",
            "RenderCore",
            "Renderer",
            "RHI"
        });
    }
}
