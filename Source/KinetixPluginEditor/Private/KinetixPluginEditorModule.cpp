#include "KinetixPluginEditorModule.h"

#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "KinetixDeveloperSettings.h"

#include "Interfaces/IMainFrameModule.h"

#include "UI/KinetixUIManager.h"

void FKinetixPluginEditorModule::StartupModule()
{
	if (GIsEditor && !IsRunningCommandlet())
	{
		FKinetixUIManager::Initialize();
	}

	// This is how to know the editor is REALLY initialized
	IMainFrameModule& MainFrameModule = IMainFrameModule::Get();
	if (MainFrameModule.IsWindowInitialized())
	{
		LaunchWindowOnMissingKey();
	}
	else
	{
		MainFrameModule.OnMainFrameCreationFinished().AddRaw(this, &FKinetixPluginEditorModule::MainFrameCreationFinished);
	}
}

void FKinetixPluginEditorModule::ShutdownModule()
{
	FKinetixUIManager::Shutdown();
}

void FKinetixPluginEditorModule::MainFrameCreationFinished(TSharedPtr<SWindow> Window, bool bArg)
{
	LaunchWindowOnMissingKey();
}

bool FKinetixPluginEditorModule::IsAPIKeyFilled()
{
	FString APIKey;
	UKinetixDeveloperSettings::GetAPIKey(APIKey);

	return !APIKey.IsEmpty();
}

void FKinetixPluginEditorModule::LaunchWindowOnMissingKey()
{
	if (IsAPIKeyFilled())
		return;
	
	// /Script/Blutility.EditorUtilityWidgetBlueprint'/KinetixPlugin/Editor/EW_KinetixAPIKey.EW_KinetixAPIKey'
	UEditorUtilityWidgetBlueprint* KinetixAPIKeyWidget = LoadObject<UEditorUtilityWidgetBlueprint>(
		GEditor, 
		TEXT("/KinetixPlugin/Editor/EW_KinetixAPIKey.EW_KinetixAPIKey"));

	if (!IsValid(KinetixAPIKeyWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find Kinetix API Widget !"));
		return;
	}

	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	if (!IsValid(EditorUtilitySubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find Editor subsystem !"));
		return;
	}
	EditorUtilitySubsystem->SpawnAndRegisterTab(KinetixAPIKeyWidget);
}

IMPLEMENT_MODULE(FKinetixPluginEditorModule, KinetixPluginEditor)
