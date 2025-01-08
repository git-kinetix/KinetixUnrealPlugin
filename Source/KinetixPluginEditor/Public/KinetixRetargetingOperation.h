// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorAnimUtils.h"
#include "Retargeter/IKRetargeter.h"
#include "Animation/AnimationAsset.h"
#include "Animation/AnimSequence.h"
#include "Engine/SkeletalMesh.h"
#include "RetargetEditor/IKRetargetBatchOperation.h"

#include "KinetixRetargetingOperation.generated.h"	

class UIKRetargeter;
class USkeletalMesh;
class UAnimationAsset;
class UAnimSequence;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)	
class KINETIXPLUGINEDITOR_API UKinetixRetargetingOperation : public UObject
{
	GENERATED_BODY()

	UIKRetargeter* RetargetAsset;
	FIKRetargetBatchOperationContext BatchContext;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinetix Editor")
	FString SourcePackage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinetix Editor")
	FString DestinationPackage;

	UFUNCTION(BlueprintCallable, DisplayName = "Create Kinetix Retargeting Operation", Category = "Kinetix Editor")
	static UKinetixRetargetingOperation* Create(UObject* Outer, UIKRetargeter* IKRetargetAsset, const FString& _SourcePackage = "", const FString& _DestinationPackage = "");

	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor")
	void Retarget(TArray<UAnimSequence*> Animation);

	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor")
	void RetargetWith1SkeletalMesh(TArray<UAnimSequence*> Animation, USkeletalMesh* SkeletalMesh, ERetargetSourceOrTarget ProvidedMeshKind);

	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor")
	void RetargetWith2SkeletalMeshes(TArray<UAnimSequence*> Animation, USkeletalMesh* InputSkeletalMesh, USkeletalMesh* OutputSkeletalMesh);
};
