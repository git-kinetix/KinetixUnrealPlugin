// Fill out your copyright notice in the Description page of Project Settings.


#include "KinetixDeveloperSettings.h"

#include "Kismet/KismetSystemLibrary.h"

UKinetixDeveloperSettings::UKinetixDeveloperSettings()
{
	CategoryName = TEXT("Plugins");
	SectionName = TEXT("Kinetix Settings");
	UE_LOG(LogKinetixDeveloperSettings, Warning, TEXT("Kinetix Developer Settings created"));
}

bool UKinetixDeveloperSettings::IsAPIKeyFilled()
{
	FString APIKey;
	GetAPIKey(APIKey);

	return !APIKey.IsEmpty();
}

void UKinetixDeveloperSettings::GetAPIKey(FString& OutAPIKey)
{
	OutAPIKey = GetDefault<UKinetixDeveloperSettings>()->APIKey;
}

void UKinetixDeveloperSettings::SetAPIKey(const FString& InAPIKey)
{
	UKinetixDeveloperSettings* KinetixSettings = GetMutableDefault<UKinetixDeveloperSettings>();
	if (!IsValid(KinetixSettings))
	{
		UE_LOG(LogKinetixDeveloperSettings, Warning, TEXT("Unable to retreive Kinetix settings, abort..."));
		return;
	}

	KinetixSettings->APIKey = InAPIKey;

	FName APIKeyMemberName = GET_MEMBER_NAME_CHECKED(UKinetixDeveloperSettings, APIKey);
	UE_LOG(LogKinetixDeveloperSettings, Log, TEXT("Kinetix Developer Settings APIKey member name %s"),
	       *APIKeyMemberName.ToString());
	FPropertyChangedEvent PropertyChangedEvent(StaticClass()->FindPropertyByName(APIKeyMemberName));
	KinetixSettings->PostEditChangeProperty(PropertyChangedEvent);

	KinetixSettings->TryUpdateDefaultConfigFile();
}

void UKinetixDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UE_LOG(LogKinetixDeveloperSettings, Log, TEXT("Kinetix Developer Settings %s property changed"),
	       *PropertyChangedEvent.GetPropertyName().ToString());
}
