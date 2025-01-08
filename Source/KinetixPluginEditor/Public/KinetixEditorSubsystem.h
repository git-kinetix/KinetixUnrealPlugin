// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "KinetixEditorSubsystem.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogKinetixPluginEditor, Log, All);

class UEditorUtilityWidgetBlueprint;

/**
 * 
 */
UCLASS()
class KINETIXPLUGINEDITOR_API UKinetixEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	UKinetixEditorSubsystem();

	// USubsystem inheritance
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// void LaunchKinetixPluginBlueprintScript(double X);
	void LaunchKinetixPluginWidget();
	
protected:

	TObjectPtr<UEditorUtilityWidgetBlueprint> KinetixPluginEntry;
	
};
