// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/FbxFactory.h"
#include "KinetixFbxFactory.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class KINETIXPLUGINEDITOR_API UKinetixFbxFactory : public UFbxFactory
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinetix Editor")
	FString InputFilePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinetix Editor")
	FString OutputPackage;

	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor")
	void SetSkeleton(USkeleton* Skeleton);
	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor")
	USkeleton* GetSkeleton();
	
	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor")
	void SetFilename(FString Filename);
	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor")
	FString GetFilename();
	
	//Fire a Weapon
	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor")
	void ImportAnimation();
	virtual bool ConfigureProperties() override;
	virtual void PostInitProperties() override;
	
	virtual UObject* FactoryCreateFile
	(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		const FString& Filename,
		const TCHAR* Parms,
		FFeedbackContext* Warn,
		bool& bOutOperationCanceled
	) override;

	virtual UObject* FactoryCreateBinary
	(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		const TCHAR* Type,
		const uint8*& Buffer,
		const uint8* BufferEnd,
		FFeedbackContext* Warn,
		bool& bOutOperationCanceled
	) override;
};
