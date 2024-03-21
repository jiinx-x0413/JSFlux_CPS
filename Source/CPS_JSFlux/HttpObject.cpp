// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpObject.h"


UHttpObject::UHttpObject()
	: BaseURL("http://3.34.116.91:8501/")
{
	HttpModule = &FHttpModule::Get();
}

void UHttpObject::HttpRequest(EHttpType Method, FString TargetURL)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UHttpObject::GetDataCallBack);
	
	
	FString EnumString = StaticEnum<EHttpType>()->GetValueAsString(Method);
	TArray<FString> EnumStringArray;
	EnumString.ParseIntoArray(EnumStringArray, TEXT("::"));
	EnumString = EnumStringArray[1];

	
	Request->SetURL(TargetURL);
	Request->SetVerb(EnumString);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}



void UHttpObject::GetDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	/*if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Success")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Failed")));
	}*/

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Responsed Data %s"), *Response->GetContentAsString()));
	CatchResponseData(Response->GetContentAsString());
	
	OnGetResponse.Broadcast(GetResponseData()); // BroadCast
}

void UHttpObject::PostDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}

void UHttpObject::PutDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}

void UHttpObject::DeleteDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}


