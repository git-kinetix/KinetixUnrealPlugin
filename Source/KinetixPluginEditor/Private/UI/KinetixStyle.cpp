// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KinetixStyle.h"

#include "KinetixPluginEditorModule.h"
#include "Styling/SlateStyleRegistry.h"

TUniquePtr<FSlateStyleSet> FKinetixStyle::KinetixStyleInstance;

void FKinetixStyle::Initialize()
{
	if (KinetixStyleInstance.IsValid())
		return;

	KinetixStyleInstance = Create();
	FSlateStyleRegistry::RegisterSlateStyle(*KinetixStyleInstance);
}

void FKinetixStyle::Shutdown()
{
	if (!KinetixStyleInstance.IsValid())
		return;

	FSlateStyleRegistry::UnRegisterSlateStyle(*KinetixStyleInstance);
	KinetixStyleInstance.Reset();
}

const ISlateStyle& FKinetixStyle::Get()
{
	check(KinetixStyleInstance);
	return *KinetixStyleInstance;
}

FName FKinetixStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("KinetixStyle"));
	return StyleSetName;
}

FName FKinetixStyle::GetContextName()
{
	static FName ContextName(TEXT("Kinetix"));
	return ContextName;
}

void FKinetixStyle::SetIcon(const FString& StyleName)
{
}

void FKinetixStyle::SetSVGIcon(const FString& StyleName, const FString& ResourceName)
{
	FSlateStyleSet* Style = KinetixStyleInstance.Get();

	const FString IconPath = Style->RootToContentDir(ResourceName, TEXT(".svg"));
	
	FString Name(GetContextName().ToString());
	Name = Name + "." + StyleName;
	Style->Set(*Name, new FSlateVectorImageBrush(IconPath, CoreStyleConstants::Icon16x16));
}

TUniquePtr<FSlateStyleSet> FKinetixStyle::Create()
{
	TUniquePtr<FSlateStyleSet> Style = MakeUnique<FSlateStyleSet>(GetStyleSetName());
	Style->SetContentRoot(FKinetixPluginEditorModule::GetPluginRoot() / TEXT("Resources"));
	return Style;
}
