// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Templates/UniquePtr.h"

class FSlateStyleSet;
class ISlateStyle;

/**
 * 
 */
class FKinetixStyle
{
public:
	static void Initialize();
	static void Shutdown();

	static const ISlateStyle& Get();
	static FName GetStyleSetName();
	static FName GetContextName();

	static void SetIcon(const FString& StyleName);
	static void SetSVGIcon(const FString& StyleName, const FString& ResourceName);
	
private:
	static TUniquePtr<FSlateStyleSet> Create();
	static TUniquePtr<FSlateStyleSet> KinetixStyleInstance;	
};
