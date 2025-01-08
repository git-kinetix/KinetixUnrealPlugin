// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_CLASS(LogKinetixUIManager, Log, All);

/**
 * 
 */
class FKinetixUIManagerImpl : public TSharedFromThis<FKinetixUIManagerImpl>
{
public:
	void Initialize();
	void Shutdown();

private:
	void SetupMenu();
	void CreateWindow();

private:
	static const FName KinetixTabID;
	static const FText KinetixLabel;
	static const FText KinetixTooltip;
};


class FKinetixUIManager
{
public:
	static void Initialize();
	static void Shutdown();

public:
	static TSharedPtr<FKinetixUIManagerImpl> Instance;
	
};
