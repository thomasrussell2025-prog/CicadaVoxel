#include "Modules/ModuleManager.h"

#include "Misc/CoreDelegates.h"
#include "SceneViewExtension.h"

#include "CVXRoboticsFieldViewExtension.h"

DEFINE_LOG_CATEGORY_STATIC(LogCicadaVoxelRender, Log, All);

class FCicadaVoxelRenderModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        // The DLL must load during PostConfigInit so IMPLEMENT_GLOBAL_SHADER has
        // registered the CVX shader before shader compilation begins.
        //
        // But FSceneViewExtensions::NewExtension requires GEngine, which does not
        // exist yet this early in UE startup. Defer only the view-extension creation.
        PostEngineInitHandle =
            FCoreDelegates::GetOnPostEngineInit().AddRaw(
                this,
                &FCicadaVoxelRenderModule::OnPostEngineInit);

        UE_LOG(
            LogCicadaVoxelRender,
            Log,
            TEXT("CICADA Voxel Render CVX-004B persistent GPU field loaded at shader-registration phase; waiting for PostEngineInit before creating direct-field SceneViewExtension."));
    }

    virtual void ShutdownModule() override
    {
        if (PostEngineInitHandle.IsValid())
        {
            FCoreDelegates::GetOnPostEngineInit().Remove(PostEngineInitHandle);
            PostEngineInitHandle.Reset();
        }

        ViewExtension.Reset();
    }

private:
    void OnPostEngineInit()
    {
        if (!ViewExtension.IsValid())
        {
            ViewExtension =
                FSceneViewExtensions::NewExtension<FCVXRoboticsFieldViewExtension>();

            UE_LOG(
                LogCicadaVoxelRender,
                Log,
                TEXT("CICADA Voxel CVX-004B PostEngineInit: PERSISTENT GPU DIRECT FIELD ACTIVE."));
        }
    }

    FDelegateHandle PostEngineInitHandle;
    TSharedPtr<FCVXRoboticsFieldViewExtension, ESPMode::ThreadSafe> ViewExtension;
};

IMPLEMENT_MODULE(FCicadaVoxelRenderModule, CicadaVoxelRender)
