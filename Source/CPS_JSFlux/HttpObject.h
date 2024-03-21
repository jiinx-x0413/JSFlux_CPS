// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "HttpObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_GetResponse, FString, StringifiedResponseData);
/**
 * 
 */

UENUM(BlueprintType)
enum class EHttpType : uint8
{
	GET UMETA(DisplayName = "GET"),
	POST UMETA(DisplayName = "POST"),
	PUT UMETA(DisplayName = "PUT"),
	DELETE UMETA(DisplayName = "DELETE"),

	defaultValue
};

UCLASS(BlueprintType)
class CPS_JSFLUX_API UHttpObject : public UObject
{
	GENERATED_BODY()

public:
	UHttpObject();

	UFUNCTION(BlueprintCallable)
	void HttpRequest(EHttpType Method, FString TargetURL);

	UPROPERTY(BlueprintAssignable)
	FDele_GetResponse OnGetResponse;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString StringifiedResponseData;

	UFUNCTION(BlueprintCallable)
	void CatchResponseData(FString NewResponseData) { StringifiedResponseData = NewResponseData; }

	UFUNCTION(BlueprintCallable)
	FString GetResponseData() { return StringifiedResponseData; }

private:
	FHttpModule* HttpModule;

	void GetDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void PostDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void PutDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void DeleteDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FString BaseURL;


};

