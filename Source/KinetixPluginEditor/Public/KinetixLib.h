// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KinetixLib.generated.h"

UCLASS()
class KINETIXPLUGINEDITOR_API UKinetixLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Kinetix Editor|Utility")
	static FString SanitizeObjectName(const FString& InObjectName);

	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor|Utility")
	static UTexture2D* GetQRCode(const FString& QrCodeContent);
};