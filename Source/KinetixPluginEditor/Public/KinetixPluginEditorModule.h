#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IPluginManager.h"
#include "Modules/ModuleManager.h"

class IPlugin;

class FKinetixPluginEditorModule : public IModuleInterface
{
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    void MainFrameCreationFinished(TSharedPtr<SWindow> Window, bool bArg);
    bool IsAPIKeyFilled();
    
    void LaunchWindowOnMissingKey();
    
public:
    static FString GetModuleName() { return TEXT("KinetixPluginEditor"); }
    static FString GetPluginName() { return TEXT("KinetixPlugin"); }
    static FString GetPluginRoot() { return IPluginManager::Get().FindPlugin(GetPluginName())->GetBaseDir();};
};