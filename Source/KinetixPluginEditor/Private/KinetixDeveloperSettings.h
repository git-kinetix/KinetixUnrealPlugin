// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "KinetixDeveloperSettings.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogKinetixDeveloperSettings, Log, All);

/**
 * 
 */
UCLASS(Config=Editor, DefaultConfig, meta = (DisplayName = "Kinetix Settings"))
class KINETIXPLUGINEDITOR_API UKinetixDeveloperSettings : public UDeveloperSettings
{
public:

private:
	GENERATED_BODY()

public:
    
    UKinetixDeveloperSettings();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kinetix Developer Settings")
	static bool IsAPIKeyFilled();
	
	static void GetAPIKey(FString& OutAPIKey);

	UFUNCTION(BlueprintCallable, Category = "Kinetix Developer Settings")
	static void SetAPIKey(const FString& InAPIKey);
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
protected:
    
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
    FString APIKey;
    
};
