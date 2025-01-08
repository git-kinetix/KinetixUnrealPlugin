// Fill out your copyright notice in the Description page of Project Settings.

#include "KinetixApi.h"
#include "KinetixDeveloperSettings.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HAL/FileManagerGeneric.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"


#define KINETIX_STAGING


//MACRO for "https://"

#define __MACRO_SLASH__() /
#define __DOUBLE_MACRO_SLASH__() __MACRO_SLASH__()__MACRO_SLASH__()
#define __HTTPS_MACRO__() https:__DOUBLE_MACRO_SLASH__()

//To stringify macro's result
#ifndef xstr
	//This macro will stringify a macro's result
	#define xstr(s) __str__(s)
	#define __str__(s) #s
#endif


//Environement define
#ifdef KINETIX_STAGING
	#define	ENVIRONEMENT staging
#endif 

//Raw url define
#ifdef ENVIRONEMENT
	#define __LOGIN_URL_RAW__ animate.ENVIRONEMENT.kinetix.tech
	#define __API_URL_RAW__ animate-api.ENVIRONEMENT.kinetix.tech
#else
	#define __LOGIN_URL_RAW__ animate.kinetix.tech
	#define __API_URL_RAW__ animate-api.kinetix.tech
#endif // ifdef ENVIRONEMENT

//Final LOGIN and API url defines

#define LOGIN_URL TEXT( xstr(__HTTPS_MACRO__()__LOGIN_URL_RAW__) )
#define API_URL(relativePath) FString::Printf( TEXT( xstr(__HTTPS_MACRO__()__API_URL_RAW__%s)), relativePath)

#define SET_API_KEY(variable) FString _apiKey;\
UKinetixDeveloperSettings::GetAPIKey(_apiKey); \
variable->SetHeader(TEXT("x-api-key"), _apiKey)

#define READ_JSON_VALUE(toReturnVar, jsonVar, method, jsonVariable, upperJsonVariable)\
toReturnVar.b##upperJsonVariable##IsDefined = jsonVar->method(TEXT(#jsonVariable), toReturnVar.jsonVariable)


/**
 * Create a http request.
 * 
 * @variable name of the local variable
 * @url url to request
 * @verb GET / POST / PUT / DELETE
 * @lambda Lambda's code
 * @... Lambda's capture variables
 * 
 * NOTE : If you're working with Visual Studio Community you can comment the macro while working on the lambda to get auto completion inside the lambda
 */
#define HTTP_REQUEST(variable, url, verb, lambda, ...) TSharedRef<IHttpRequest, ESPMode::ThreadSafe> variable = FHttpModule::Get().CreateRequest();\
variable->OnProcessRequestComplete().BindLambda([__VA_ARGS__](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)\
{\
	bWasSuccessful = bWasSuccessful && Response != nullptr;\
	if (bWasSuccessful)\
	{\
		int32 responseCode = Response->GetResponseCode();\
		bWasSuccessful = bWasSuccessful && responseCode >= 200 && responseCode < 400;\
	}\
	lambda\
});\
variable->SetURL(url);\
variable->SetVerb(verb);\
SET_API_KEY(variable);

void UKinetixApi::ListProcesses(const FKinetixProcessArrayDelegate &OnComplete)
{
	FString url = API_URL(TEXT("/v1/process"));
	HTTP_REQUEST(HttpRequest, url, "GET",
		TArray<FKinetixProcess> toReturn;
		if (bWasSuccessful && Response.IsValid())
		{
			const FString jsonResponse = Response->GetContentAsString();

			TArray<TSharedPtr<FJsonValue>> jsonArray;
			TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonResponse);

			const bool bDeserializationResult = FJsonSerializer::Deserialize(jsonReader, jsonArray);
			if (!bDeserializationResult)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("[UKinetixApi] Unable to deserialize response ! %s"),
					*jsonReader.Get().GetErrorMessage());
	
				OnComplete.Execute(toReturn, false);
				return;
			}

			toReturn.Reset(); //security, ensure array is empty
			int32 num = jsonArray.Num();
			
			FKinetixProcess currentReturnValue;
			TSharedPtr<FJsonObject> jsonValue;
			for (int32 i = 0; i < num; i++)
			{
				jsonValue = jsonArray[i].Get()->AsObject();

				currentReturnValue = FKinetixProcess();

				ParseKinetixProcess(jsonValue, currentReturnValue);

				toReturn.Add(currentReturnValue);
			}

			OnComplete.Execute(toReturn, true);
		}
		else
		{
			ParseAndLogError(Response);
			OnComplete.Execute(toReturn, false);
		}
	, OnComplete);

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("accept"), TEXT("application/json"));
	HttpRequest->ProcessRequest();
}

void UKinetixApi::GetProcess(const FString& processUuid, const FKinetixProcessDelegate& OnComplete)
{
	FString url = FString::Printf(TEXT("/v1/process/%s"), *processUuid);
	url = API_URL(*url);

	HTTP_REQUEST(HttpRequest, url, "GET",
		FKinetixProcess toReturn;
		if (bWasSuccessful && Response.IsValid())
		{
			const FString jsonResponse = Response->GetContentAsString();

			TSharedPtr<FJsonObject> jsonValue;
			TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonResponse);

			const bool bDeserializationResult = FJsonSerializer::Deserialize(jsonReader, jsonValue);
			if (!bDeserializationResult)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("[UKinetixApi] Unable to deserialize response ! %s"),
					*jsonReader.Get().GetErrorMessage());
	
				OnComplete.Execute(toReturn, false);
				return;
			}

			ParseKinetixProcess(jsonValue, toReturn);

			OnComplete.Execute(toReturn, true);
		}
		else
		{
			ParseAndLogError(Response);
			OnComplete.Execute(toReturn, false);
		}
	, OnComplete);

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("accept"), TEXT("application/json"));
	HttpRequest->ProcessRequest();
}

void UKinetixApi::CreateProcessToken(const FKinetixProcessTokenDelegate &OnComplete)
{
	FString url = API_URL(TEXT("/v1/process/token"));
	HTTP_REQUEST(HttpRequest, url, "GET",
		FKinetixProcessToken toReturn;
		if (bWasSuccessful && Response.IsValid())
		{
			const FString jsonResponse = Response->GetContentAsString();
			
			TSharedPtr<FJsonObject> jsonValue;
			TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonResponse);

			const bool bDeserializationResult = FJsonSerializer::Deserialize(jsonReader, jsonValue);
			if (!bDeserializationResult)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("[UKinetixApi] Unable to deserialize response ! %s"),
					*jsonReader.Get().GetErrorMessage());

				OnComplete.Execute(toReturn, false);
				return;
			}
			
			//FString uuid;
			READ_JSON_VALUE(toReturn, jsonValue, TryGetStringField, uuid, Uuid);
			//FString url;
			READ_JSON_VALUE(toReturn, jsonValue, TryGetStringField, url, Url);
			//FString expire;
			READ_JSON_VALUE(toReturn, jsonValue, TryGetStringField, expire, Expire);

			OnComplete.Execute(toReturn, true);
		}
		else
		{
			ParseAndLogError(Response);
			OnComplete.Execute(toReturn, false);
		}
	, OnComplete);

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->ProcessRequest();
}

void UKinetixApi::GetTokenInfo(const FString& tokenUuid, const FKinetixGetTokenInfo& OnComplete)
{
	FString url = FString::Printf(TEXT("/v1/process/token/%s"), *tokenUuid);
	url = API_URL(*url);

	HTTP_REQUEST(HttpRequest, url, "GET",
	if (bWasSuccessful && Response.IsValid())
	{
		/**/
		/* Uncoment this json template to handle JSON's response */
		/**/
		/*const FString jsonResponse = Response->GetContentAsString();

		TSharedPtr<FJsonObject> jsonValue;
		TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonResponse);

		const bool bDeserializationResult = FJsonSerializer::Deserialize(jsonReader, jsonValue);
		if (!bDeserializationResult)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("[UKinetixApi] Unable to deserialize response ! %s"),
				*jsonReader.Get().GetErrorMessage());

			OnComplete.Execute(false);
			return;
		}*/

		int32 responseCode = Response->GetResponseCode();
		OnComplete.Execute(responseCode >= 200 && responseCode < 400);
	}
	else
	{
		OnComplete.Execute(false);
	}
	, OnComplete);

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->ProcessRequest();
}

void UKinetixApi::DownloadProcessesOutput(const FString &SavePath, const FString &processUuid, const FKinetixDownload &OnComplete)
{
	FString url = FString::Printf(TEXT("/v1/process/%s/download?format=fbx&rig=rpm"), *processUuid);
	url = API_URL(*url);

	HTTP_REQUEST(HttpRequest, url, "GET",
		if (bWasSuccessful && Response.IsValid())
		{
			//Download file
			TArray<uint8> content = Response->GetContent();
			if (FFileHelper::SaveArrayToFile(content, *SavePath))
			{
				UE_LOG(LogTemp, Log, TEXT("[UKinetixApi] FBX file saved successfully at: %s"), *SavePath);
				OnComplete.Execute(true);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[UKinetixApi] Failed to save FBX file at: %s"), *SavePath);
				OnComplete.Execute(false);
			}
		}
		else
		{
			ParseAndLogError(Response);
			OnComplete.Execute(false);
		}
	, OnComplete, SavePath);

	//Add query params
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/octect-stream"));
	HttpRequest->SetHeader(TEXT("Accept"), TEXT("application/json, text/plain, */*"));
	HttpRequest->SetHeader(TEXT("Accept-Encoding"), TEXT("gzip, deflate, br, zstd"));
	HttpRequest->ProcessRequest();
}

void UKinetixApi::GetLoginUrl(FString& url)
{
	url = LOGIN_URL;
}

void UKinetixApi::ParseAndLogError(FHttpResponsePtr Response)
{
	if (Response == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[UKinetixApi] HTTP Request failed with error code ERR_INTERNET_DISCONNECTED and message: No internet connection"));
		return;
	}

	int32 responseCode = Response->GetResponseCode();

	const FString jsonResponse = Response->GetContentAsString();

	TSharedPtr<FJsonObject> jsonValue;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonResponse);

	const bool bDeserializationResult = FJsonSerializer::Deserialize(jsonReader, jsonValue);
	if (!bDeserializationResult)
	{
		UE_LOG(LogTemp, Error, TEXT("[UKinetixApi] HTTP Request failed with error code %d and message: ?"), responseCode);
		UE_LOG(LogTemp, Warning,
			TEXT("[UKinetixApi] Unable to deserialize error ! %s"),
			*jsonReader.Get().GetErrorMessage());
		UE_LOG(LogTemp, Error, TEXT("[UKinetixApi] Server error returned : '%s'"), *jsonResponse);
		return;
	}
	
	FString message, details;
	jsonValue->TryGetStringField(TEXT("message"), message);
	UE_LOG(LogTemp, Error, TEXT("[UKinetixApi] HTTP Request failed (%d) with error message: %s."), responseCode, *message);
	jsonValue->TryGetStringField(TEXT("details"), details);
	
	if (!details.IsEmpty())
		UE_LOG(LogTemp, Error, TEXT("[UKinetixApi] HTTP Request Error Details: '%s'."), *details);
}

void UKinetixApi::ParseKinetixProcess(TSharedPtr<FJsonObject> jsonValue, FKinetixProcess& process)
{
	//FString uuid;
	READ_JSON_VALUE(process, jsonValue, TryGetStringField, uuid, Uuid);
	//FString name;
	READ_JSON_VALUE(process, jsonValue, TryGetStringField, name, Name);
	//FString thumbnailUrl;
	READ_JSON_VALUE(process, jsonValue, TryGetStringField, thumbnailUrl, ThumbnailUrl);
	// FString animationThumbnailAnimated;
	READ_JSON_VALUE(process, jsonValue, TryGetStringField, animationThumbnailAnimated, AnimationThumbnailAnimated);
	// FString animationThumbnailStatic;
	READ_JSON_VALUE(process, jsonValue, TryGetStringField, animationThumbnailStatic, AnimationThumbnailStatic);
	//FString animation;
	READ_JSON_VALUE(process, jsonValue, TryGetStringField, animation, Animation);
	//FString video;
	READ_JSON_VALUE(process, jsonValue, TryGetStringField, video, Video);
	//FString text;
	READ_JSON_VALUE(process, jsonValue, TryGetStringField, text, Text);
	//FString step;
	READ_JSON_VALUE(process, jsonValue, TryGetStringField, step, Step);
	//int32 application;
	READ_JSON_VALUE(process, jsonValue, TryGetNumberField, application, Application);
	//int32 progression;
	READ_JSON_VALUE(process, jsonValue, TryGetNumberField, progression, Progression);
	//FString createdAt;
	READ_JSON_VALUE(process, jsonValue, TryGetStringField, createdAt, CreatedAt);

	//FKinetixMLProgression ml;
	const TSharedPtr<FJsonObject>* mlJsonValuePointer;
	process.bMlIsDefined = jsonValue->TryGetObjectField(TEXT("ml"), mlJsonValuePointer);
	if (process.bMlIsDefined)
	{
		TSharedPtr<FJsonObject> mlJsonValue = mlJsonValuePointer->ToSharedRef();

		//int32 progression
		READ_JSON_VALUE(process.ml, mlJsonValue, TryGetNumberField, progression, Progression);
		//int32 mlRemaining
		READ_JSON_VALUE(process.ml, mlJsonValue, TryGetNumberField, mlRemaining, MlRemaining);
		//int32 mlElapsed
		READ_JSON_VALUE(process.ml, mlJsonValue, TryGetNumberField, mlElapsed, MlElapsed);
	}
}
