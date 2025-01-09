// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Interfaces/IHttpRequest.h"

#include "KinetixApi.generated.h"

/*
 * Declare a "isDefined" field for the struct 
 * 
 * Unreal doesn't allow the use of this macro.
 * Please use expand this macro befor compilation
 */
#define __KINETIX_DECLARE_IS_DEFINED(nameUpperCamel)\
b##nameUpperCamel##IsDefined\

#pragma region KinetixProcess
USTRUCT(BlueprintType)
struct FKinetixMLProgression
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 progression;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bProgressionIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mlRemaining;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMlRemainingIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mlElapsed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMlElapsedIsDefined;
};

USTRUCT(BlueprintType)
struct FKinetixProcess
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString uuid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bUuidIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bNameIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString thumbnailUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bThumbnailUrlIsDefined;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString animationThumbnailAnimated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bAnimationThumbnailAnimatedIsDefined;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString animationThumbnailStatic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bAnimationThumbnailStaticIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString animation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bAnimationIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString video;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bVideoIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bTextIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString step;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bStepIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	int32 application;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bApplicationIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	int32 progression;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bProgressionIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString createdAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bCreatedAtIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FKinetixMLProgression ml;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bMlIsDefined;

};

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_TwoParams(FKinetixProcessDelegate, const FKinetixProcess&, processInfo, bool, bIsSuccess);
UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_TwoParams(FKinetixProcessArrayDelegate, const TArray<FKinetixProcess>&, processesInfo, bool, bIsSuccess);
#pragma endregion

#pragma region KinetixProcessToken
USTRUCT(BlueprintType)
struct FKinetixProcessToken
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString uuid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bUuidIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString url;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bUrlIsDefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString expire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bExpireIsDefined;

};

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_TwoParams(FKinetixProcessTokenDelegate, FKinetixProcessToken, generatedTokenInfo, bool, bIsSuccess);
#pragma endregion

/**
 * Delegate called when a download completes
 *
 * @param bIsSuccess
 */
UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_OneParam(FKinetixDownload, bool, bIsSuccess);

/**
 * Delegate called when a download completes
 *
 * @param bIsSuccess
 */
UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_OneParam(FKinetixGetTokenInfo, bool, bIsSuccess);

/**
 * Delegate called when upgrading the api (used to validate the api key and setup the animate app settings)
 *
 * @param bIsSuccess
 */
UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_OneParam(FKinetixUpgradeApiVersion, bool, bIsSuccess);

UCLASS()
class KINETIXPLUGINEDITOR_API UKinetixApi : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:	
	/**
	* Get all the processes progression information.
	* When ML is running, ml field will display its progression in percentage 
	* and elapsed time as well as remaining time estimation in seconds.
	*/
	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor/API")
	static void ListProcesses(const FKinetixProcessArrayDelegate &OnComplete);
	
	/**
	* Get the process progression information.
	* When ML is running, ml field will display its progression in percentage
	* and elapsed time as well as remaining time estimation in seconds.
	*/
	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor/API")
	static void GetProcess(const FString& processUuid, const FKinetixProcessDelegate&OnComplete);

	/**
	* Get a token to authenticate a process creation
	*/
	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor/API")
	static void CreateProcessToken(const FKinetixProcessTokenDelegate &OnComplete);

	/**
	* Get the state of a token. On success return the expiration time, on error means token was used or never existed
	*/
	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor/API")
	static void GetTokenInfo(const FString& tokenUuid, const FKinetixGetTokenInfo& OnComplete);

	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor/API")
	static void DownloadProcessesOutput(const FString &SavePath, const FString &processUuid, const FKinetixDownload &OnComplete);
	
	UFUNCTION(BlueprintCallable, Category = "Kinetix Editor/API")
	static void UpgradeApiVersion(const FKinetixUpgradeApiVersion& OnComplete);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kinetix Editor/API")
	static void GetLoginUrl(FString& url);

private:
	static void ParseAndLogError(FHttpResponsePtr Response, const FString MethodName);

	static void ParseKinetixProcess(TSharedPtr<FJsonObject> jsonValue, FKinetixProcess& process);
};

#undef __KINETIX_DECLARE_IS_DEFINED
