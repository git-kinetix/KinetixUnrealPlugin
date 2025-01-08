// Fill out your copyright notice in the Description page of Project Settings.

#include "KinetixRetargetingOperation.h"

#include "ObjectTools.h"
#include "EngineUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"

UKinetixRetargetingOperation* UKinetixRetargetingOperation::Create(UObject* _Outer, UIKRetargeter* _IKRetargetAsset, const FString& _SourcePackage, const FString& _DestinationPackage)
{
	UKinetixRetargetingOperation* toReturn = NewObject<UKinetixRetargetingOperation>(_Outer);
	toReturn->RetargetAsset = _IKRetargetAsset;
	toReturn->SourcePackage = _SourcePackage;
	toReturn->DestinationPackage = _DestinationPackage;

	return toReturn;
}

void UKinetixRetargetingOperation::Retarget(TArray<UAnimSequence*> _Animation)
{
	USkeletalMesh* inputSkeletalMesh = RetargetAsset->GetPreviewMesh(ERetargetSourceOrTarget::Source);
	USkeletalMesh* outputSkeletalMesh = RetargetAsset->GetPreviewMesh(ERetargetSourceOrTarget::Target);

	RetargetWith2SkeletalMeshes(_Animation, inputSkeletalMesh, outputSkeletalMesh);
}

void UKinetixRetargetingOperation::RetargetWith1SkeletalMesh(TArray<UAnimSequence*> Animation, USkeletalMesh* SkeletalMesh, ERetargetSourceOrTarget ProvidedMeshKind)
{
	USkeletalMesh* inputSkeletalMesh = RetargetAsset->GetPreviewMesh(ERetargetSourceOrTarget::Source);
	USkeletalMesh* outputSkeletalMesh = RetargetAsset->GetPreviewMesh(ERetargetSourceOrTarget::Target);

	switch (ProvidedMeshKind)
	{
		case ERetargetSourceOrTarget::Source:
			RetargetWith2SkeletalMeshes(Animation, SkeletalMesh, outputSkeletalMesh);
			break;
		default:
		case ERetargetSourceOrTarget::Target:
			RetargetWith2SkeletalMeshes(Animation, inputSkeletalMesh, SkeletalMesh);
			break;
	}
}

void UKinetixRetargetingOperation::RetargetWith2SkeletalMeshes(TArray<UAnimSequence*> Animation, USkeletalMesh* InputSkeletalMesh, USkeletalMesh* OutputSkeletalMesh)
{
	BatchContext.AssetsToRetarget.Reset();
	BatchContext.NameRule.FolderPath = DestinationPackage; //Export folder
	BatchContext.NameRule.ReplaceFrom = TEXT("_"); //Name_Uuid
	BatchContext.NameRule.ReplaceTo = TEXT("_") + FPaths::GetBaseFilename(DestinationPackage) + TEXT("_"); //Name_Skeleton_Uuid
	BatchContext.IKRetargetAsset = RetargetAsset;
	BatchContext.SourceMesh = InputSkeletalMesh;
	BatchContext.TargetMesh = OutputSkeletalMesh;
	
	BatchContext.bOverwriteExistingFiles = true;
	BatchContext.bIncludeReferencedAssets = true;
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION > 4
	BatchContext.bRetainAdditiveFlags = true;
#endif
	BatchContext.bExportOnlyAnimatedBones = true;
	BatchContext.bUseSourcePath = false;

	for (UAnimSequence* Asset : Animation)
	{
		BatchContext.AssetsToRetarget.Add(Asset);
	}

	//Retarget
	const TStrongObjectPtr<UIKRetargetBatchOperation> BatchOperation(NewObject<UIKRetargetBatchOperation>());
	BatchOperation->RunRetarget(BatchContext);	
}