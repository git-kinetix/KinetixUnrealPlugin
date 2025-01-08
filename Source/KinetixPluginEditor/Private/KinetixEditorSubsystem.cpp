// Fill out your copyright notice in the Description page of Project Settings.

#include "KinetixEditorSubsystem.h"

#include "EditorUtilityWidgetBlueprint.h"
#include "EditorUtilitySubsystem.h"

UKinetixEditorSubsystem::UKinetixEditorSubsystem()
{}

void UKinetixEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{}

void UKinetixEditorSubsystem::LaunchKinetixPluginWidget()
{
	if (!IsValid(KinetixPluginEntry))
	{
		KinetixPluginEntry =
			LoadObject<UEditorUtilityWidgetBlueprint>(
				this, TEXT("/KinetixPlugin/Editor/EW_KinetixPluginWindow.EW_KinetixPluginWindow"));
	}

	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();

	if (!IsValid(EditorUtilitySubsystem))
	{
		UE_LOG(LogKinetixPluginEditor, Error, TEXT("EditorUtilitySubsystem is null"));
		return;
	}

	EditorUtilitySubsystem->SpawnAndRegisterTab(KinetixPluginEntry);
}
