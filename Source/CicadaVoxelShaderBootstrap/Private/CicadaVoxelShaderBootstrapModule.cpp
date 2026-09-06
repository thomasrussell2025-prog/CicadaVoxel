#include "Modules/ModuleManager.h"

#include "HAL/FileManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

DEFINE_LOG_CATEGORY_STATIC(LogCicadaVoxelShaderBootstrap, Log, All);

class FCicadaVoxelShaderBootstrapModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        const TSharedPtr<IPlugin> Plugin =
            IPluginManager::Get().FindPlugin(TEXT("CicadaVoxel"));

        if (!Plugin.IsValid())
        {
            UE_LOG(
                LogCicadaVoxelShaderBootstrap,
                Error,
                TEXT("CVX shader bootstrap: CicadaVoxel plugin descriptor was not found."));
            return;
        }

        const FString ShaderDirectory =
            FPaths::Combine(Plugin->GetBaseDir(), TEXT("Shaders"));

        const FString RequiredShader =
            FPaths::Combine(
                ShaderDirectory,
                TEXT("Private/CVXRoboticsField.usf"));

        if (!IFileManager::Get().DirectoryExists(*ShaderDirectory))
        {
            UE_LOG(
                LogCicadaVoxelShaderBootstrap,
                Error,
                TEXT("CVX shader bootstrap: shader directory missing: %s"),
                *ShaderDirectory);
            return;
        }

        if (!IFileManager::Get().FileExists(*RequiredShader))
        {
            UE_LOG(
                LogCicadaVoxelShaderBootstrap,
                Error,
                TEXT("CVX shader bootstrap: required shader missing: %s"),
                *RequiredShader);
            return;
        }

        AddShaderSourceDirectoryMapping(
            TEXT("/Plugin/CicadaVoxel"),
            ShaderDirectory);

        UE_LOG(
            LogCicadaVoxelShaderBootstrap,
            Log,
            TEXT("CVX R4 R2 bootstrap: physical shader verified; mapped /Plugin/CicadaVoxel -> %s"),
            *ShaderDirectory);

        FModuleManager::Get().LoadModuleChecked<IModuleInterface>(
            TEXT("CicadaVoxelRender"));

        UE_LOG(
            LogCicadaVoxelShaderBootstrap,
            Log,
            TEXT("CVX R4 R2 bootstrap: CicadaVoxelRender loaded AFTER shader mapping."));
    }

    virtual void ShutdownModule() override
    {
    }
};

IMPLEMENT_MODULE(
    FCicadaVoxelShaderBootstrapModule,
    CicadaVoxelShaderBootstrap)
