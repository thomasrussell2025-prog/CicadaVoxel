#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogCicadaVoxel, Log, All);

class FCicadaVoxelModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        UE_LOG(LogCicadaVoxel, Log, TEXT("CICADA Voxel CVX-003C Realtime Surface + Async Pages loaded."));
    }

    virtual void ShutdownModule() override
    {
    }
};

IMPLEMENT_MODULE(FCicadaVoxelModule, CicadaVoxel)
