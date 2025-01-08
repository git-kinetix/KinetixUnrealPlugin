// Fill out your copyright notice in the Description page of Project Settings.

#include "KinetixFbxFactory.h"

#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Factories/FbxAnimSequenceImportData.h"
#include "Factories/FbxImportUI.h"
#include "FbxMeshUtils.h"

void UKinetixFbxFactory::SetSkeleton(USkeleton* Skeleton)
{
	ImportUI->Skeleton = Skeleton;
}

USkeleton* UKinetixFbxFactory::GetSkeleton()
{
	return ImportUI->Skeleton;
}

void UKinetixFbxFactory::SetFilename(FString Filename)
{
	ImportUI->OverrideAnimationName = Filename;
}

FString UKinetixFbxFactory::GetFilename()
{
	return ImportUI->OverrideAnimationName;
}

void UKinetixFbxFactory::ImportAnimation()
{
	//Source : https://github.com/20tab/UnrealEnginePython/blob/4b5da5bf4ca598e8d0f67eb938749bc381a67d6c/Source/UnrealEnginePython/Private/UEPyEditor.cpp#L2096C13-L2096C168

	if (OutputPackage[0] != '/')
		OutputPackage = TEXT("/") + OutputPackage;

	FString objectName = ObjectTools::SanitizeObjectName(FPaths::GetBaseFilename(InputFilePath));
	FString pkgName = FString(OutputPackage) + TEXT("/") + objectName;

	bool bCanceled = false; //By ref var
	UPackage *outer = CreatePackage(*pkgName);

	UObject* toReturn = ImportObject(
		ResolveSupportedClass(), 
		outer,
		FName(*objectName),
		RF_Public | RF_Standalone,
		InputFilePath,
		nullptr,
		bCanceled
	);
	
	if (toReturn)
	{
		FAssetRegistryModule::AssetCreated(toReturn);
		outer->MarkPackageDirty();
		
		FString filepath = FPaths::ConvertRelativePathToFull(
			FPackageName::LongPackageNameToFilename(
				pkgName,
				FPackageName::GetAssetPackageExtension()
			)
		);

		FSavePackageArgs savePackageArgs;
		savePackageArgs.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
		
		bool bSucceeded = UPackage::SavePackage(outer, toReturn, *filepath, savePackageArgs);
		if (!bSucceeded)
		{
			UE_LOG(LogTemp, Error, TEXT("Package '%s' at path '%s' wasn't saved!"), *pkgName, *filepath)
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Package '%s' at path '%s' was successfully saved"), *pkgName, *filepath)
		}

		ReloadPackage(outer, 0);
	}
}

bool UKinetixFbxFactory::ConfigureProperties()
{
	bDetectImportTypeOnImport = false;
	bShowOption = false;
	return true;
}

void UKinetixFbxFactory::PostInitProperties()
{
	Super::PostInitProperties();

	ImportUI->MeshTypeToImport = FBXIT_Animation;
	ImportUI->AnimSequenceImportData->bSnapToClosestFrameBoundary = true;
}

UObject* UKinetixFbxFactory::FactoryCreateFile(
	UClass* _InClass,
	UObject* _InParent,
	FName _InName,
	EObjectFlags _Flags,
	const FString& _Filename,
	const TCHAR* _Parms,
	FFeedbackContext* _Warn,
	bool& _bOutOperationCanceled
)
{
	FbxMeshUtils::SetImportOption(ImportUI);
	// ensure auto-detect is skipped
	bDetectImportTypeOnImport = false;

	return Super::FactoryCreateFile(
		_InClass, 
		_InParent, 
		_InName, 
		_Flags, 
		_Filename, 
		_Parms, 
		_Warn, 
		_bOutOperationCanceled
	);
}

UObject* UKinetixFbxFactory::FactoryCreateBinary(
	UClass* _InClass,
	UObject* _InParent,
	FName _InName,
	EObjectFlags _Flags,
	UObject* _Context,
	const TCHAR* _Type,
	const uint8*& _Buffer,
	const uint8* _BufferEnd,
	FFeedbackContext* _Warn,
	bool& _bOutOperationCanceled
)
{
	FbxMeshUtils::SetImportOption(ImportUI);
	// ensure auto-detect is skipped
	bDetectImportTypeOnImport = false;

	return Super::FactoryCreateBinary(
		_InClass,
		_InParent,
		_InName,
		_Flags,
		_Context,
		_Type,
		_Buffer,
		_BufferEnd,
		_Warn,
		_bOutOperationCanceled
	);
}
