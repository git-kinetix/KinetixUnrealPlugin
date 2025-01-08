// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KinetixUIManager.h"

#include "KinetixEditorSubsystem.h"
#include "KinetixStyle.h"
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "Kinetix"

// Mandatory, otherwise FKinetixUIManager is not aware of FKinetixUIManagerImpl declarations
TSharedPtr<FKinetixUIManagerImpl> FKinetixUIManager::Instance;

const FName FKinetixUIManagerImpl::KinetixTabID = TEXT("KinetixTab");

const FText FKinetixUIManagerImpl::KinetixLabel = LOCTEXT("Kinetix.Label", "Kinetix");
const FText FKinetixUIManagerImpl::KinetixTooltip = LOCTEXT("Kinetix.Tooltip", "Opens the Kinetix's toolbox");

void FKinetixUIManagerImpl::Initialize()
{
	FKinetixStyle::Initialize();
	SetupMenu();
}

void FKinetixUIManagerImpl::Shutdown()
{
}

void FKinetixUIManagerImpl::SetupMenu()
{
	// Registering the Kinetix cross located inside plugin's "Resources" folder 
	FKinetixStyle::SetSVGIcon("Logo", "KinetixLogo");

	// Setup of the Content Browser entry
	FToolMenuEntry& ContentBrowserEntry =
		UToolMenus::Get()->ExtendMenu("ContentBrowser.Toolbar")->FindOrAddSection("New").AddEntry(
			FToolMenuEntry::InitToolBarButton(
				"OpenKinetixWindow",
				FUIAction(
					FExecuteAction::CreateRaw(
						this, &FKinetixUIManagerImpl::CreateWindow), FCanExecuteAction()),
				KinetixLabel,
				KinetixTooltip,
				FSlateIcon(FKinetixStyle::GetStyleSetName(),
				           FName(FKinetixStyle::GetContextName().ToString() + ".Logo")),
				EUserInterfaceActionType::Button
			));
}

void FKinetixUIManagerImpl::CreateWindow()
{
	UE_LOG(LogKinetixUIManager, Log, TEXT("Asking to create Kinetix's window..."));

	UKinetixEditorSubsystem* KinetixEditorSubsystem = GEditor->GetEditorSubsystem<UKinetixEditorSubsystem>();
	if (!IsValid(KinetixEditorSubsystem))
	{
		UE_LOG(LogKinetixUIManager, Warning, TEXT("Kinetix editor subsystem is null !"));
		return;
	}

	KinetixEditorSubsystem->LaunchKinetixPluginWidget();
}

#undef LOCTEXT_NAMESPACE

void FKinetixUIManager::Initialize()
{
	if (Instance.IsValid())
		return;

	Instance = MakeShareable(new FKinetixUIManagerImpl);
	Instance->Initialize();
}

void FKinetixUIManager::Shutdown()
{
}
