// Fill out your copyright notice in the Description page of Project Settings.


#include "CPSController.h"
#include "HttpObject.h"
#include "CPSActorManager.h"
#include "CPSWidgetManager.h"
#include "ButtonWidget.h"

ACPSController::ACPSController()
	:bIsRunning(false), WaitSeconds(0)
{
}

void ACPSController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
	SetShowMouseCursor(true);
}

void ACPSController::Tick(float DeltaTime)
{
	if (IsValid(HttpObject) && bIsRunning)
	{
		WaitSeconds += DeltaTime;
		if (WaitSeconds >= 1)
		{
			RequestData(EAPIType::UPDATE_VCMDATA);
			WaitSeconds = 0;
		}
	}
}

void ACPSController::RequestData(EAPIType APIType, FString Parameters)
{
	if (!IsValid(HttpObject))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Http Object is not valid"));
		return;
	}

	FString TargetURL;
	EHttpType Method;
	HttpObject->OnGetResponse.Clear();
	switch (APIType)
	{
	case EAPIType::GETMAIN:
		Method = EHttpType::GET;
		TargetURL = "http://3.34.116.91:8501/api/vcmdata/getmain";
		HttpObject->OnGetResponse.AddDynamic(this, &ACPSController::JSONParser_GETMAIN);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("HTTP OnGetResponse Delegate : GETMAIN"));
		break;
	case EAPIType::HIERARCHY:
		Method = EHttpType::GET;
		TargetURL = "http://133.186.215.97:51000/hierarchy/";
		HttpObject->OnGetResponse.AddDynamic(this, &ACPSController::JSONParser_HIERARCHY);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("HTTP OnGetResponse Delegate : HIERARCHY"));
		break;
	case EAPIType::HIERARCHY_PARENT:
		Method = EHttpType::GET;
		TargetURL = "http://133.186.215.97:51000/hierarchy/parent/";
		HttpObject->OnGetResponse.AddDynamic(this, &ACPSController::JSONParser_HIERARCHY_PARENT);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("HTTP OnGetResponse Delegate : HIERARCHY_PARENT"));
		break;
	case EAPIType::HIERARCHY_CHILD:
		Method = EHttpType::GET;
		TargetURL = "http://133.186.215.97:51000/hierarchy/child/";
		HttpObject->OnGetResponse.AddDynamic(this, &ACPSController::JSONParser_HIERARCHY_CHILD);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("HTTP OnGetResponse Delegate : HIERARCHY_CHILD"));
		break;
	case EAPIType::HIERARCHY_NODE:
		Method = EHttpType::GET;
		TargetURL = "http://133.186.215.97:51000/hierarchy/node/";
		HttpObject->OnGetResponse.AddDynamic(this, &ACPSController::JSONParser_HIERARCHY_NODE);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("HTTP OnGetResponse Delegate : HIERARCHY_NODE"));
		break;
	case EAPIType::UPDATE_VCMDATA:
		Method = EHttpType::GET;
		TargetURL = "http://3.34.116.91:8501/api/vcmdata/getmain";
		HttpObject->OnGetResponse.AddDynamic(this, &ACPSController::JSONParser_UpdateVCMData);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("HTTP OnGetResponse Delegate : UPDATE_VCMDATA"));
		break;
	case EAPIType::defaultValue:
		break;
	}

	TargetURL = TargetURL.Append(Parameters);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("URL Path : %s"), *TargetURL));
	HttpObject->HttpRequest(Method, TargetURL);

	if(bIsRunning == false)
	{
		bIsRunning = true;
	}
}


void ACPSController::BindActorWithButton(UButtonWidget* TargetButtonWidget)
{
	int32 TargetId = TargetButtonWidget->GetIndex();
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("BindActorWithButton executed")));
	TObjectPtr<ACPSActor> TempActor = GetActorManager()->GetActorMap().FindRef(TargetId);

	if (TempActor != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Button which can bind Actor : %i"), TargetId));
		TargetButtonWidget->SetTargetActor(TempActor);
	}
}

void ACPSController::JSONParser_GETMAIN(FString StringifiedResponseData)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<TCHAR>::Create(StringifiedResponseData);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Get Data Failed"));
		return;
	}
	TArray < TSharedPtr<FJsonValue>> DataArray = JsonObject->GetArrayField("data");

	FVCMStruct TempVCMStruct;
	TArray<FVCMStruct> ParsedVCMStructArray;
	for (auto data : DataArray)
	{
		TempVCMStruct.ItemIndex = data->AsObject()->GetIntegerField("itemId");
		TempVCMStruct.DataName = data->AsObject()->GetStringField("dataName");
		TempVCMStruct.DataValue = data->AsObject()->GetStringField("dataValue");
		TempVCMStruct.VcId = data->AsObject()->GetIntegerField("vcId");
		TempVCMStruct.VcName = data->AsObject()->GetStringField("vcName");
		TempVCMStruct.Type = data->AsObject()->GetIntegerField("type");
		
		ParsedVCMStructArray.Add(TempVCMStruct);
	}

	OnParseVCMData.Clear();
	OnParseVCMData.AddDynamic(WidgetManager, &UCPSWidgetManager::SetVCMDataArray);
	OnParseVCMData.Broadcast(ParsedVCMStructArray); // BroadCast
}

void ACPSController::JSONParser_HIERARCHY(FString StringifiedResponseData)
{	
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<TCHAR>::Create(StringifiedResponseData);
	if (!FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Get Data Failed"));
		return;
	}
	
	TArray< FHStruct> ParsedHStructArray;
	FHStruct Temp;
	
	for (auto data : JsonArray)
	{
		TSharedPtr<FJsonObject> dataObject = data->AsObject();
		if (dataObject->GetIntegerField("type") > 0)
		{
			continue;
		}
		Temp = ParseStructAll<TSharedPtr<FJsonObject>, FHStruct, FHHStruct, FHHHStruct>(dataObject);
		ParsedHStructArray.Add(Temp);
	}

	if (!OnParseVCMData.IsBound())
	{
		OnParseHDataArray.AddDynamic(WidgetManager, &UCPSWidgetManager::SetHDataArray);
	}

	OnParseHDataArray.Broadcast(ParsedHStructArray); // BroadCast
}

void ACPSController::JSONParser_HIERARCHY_PARENT(FString StringifiedResponseData)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<TCHAR>::Create(StringifiedResponseData);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Get Data Failed"));
		return;
	}
	
	FHStruct ParsedHStruct = ParseStructAll<TSharedPtr<FJsonObject>, FHStruct, FHHStruct, FHHHStruct>(JsonObject);
	// child ; 
	// 파라미터로 입력한 노드가 부모 오브젝트의 child array 안에 들어가 있음. 
	// 즉, 최상위 노드 자체가 입력한 id의 노드가 아닌 부모 노드.
	
	
	if (!OnParseHData.IsBound())
	{
		//OnParseHData.AddDynamic(WidgetManager, &UCPSWidgetManager::PrintHData);
	}

	OnParseHData.Broadcast(ParsedHStruct); // BroadCast
}

void ACPSController::JSONParser_HIERARCHY_CHILD(FString StringifiedResponseData)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<TCHAR>::Create(StringifiedResponseData);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Get Data Failed"));
		return;
	}

	FHStruct ParsedHStruct = ParseStructAll<TSharedPtr<FJsonObject>, FHStruct, FHHStruct, FHHHStruct>(JsonObject);
	
	if (!OnParseHData.IsBound())
	{
		//OnParseHData.AddDynamic(WidgetManager, &UCPSWidgetManager::PrintHData);
	}

	OnParseHData.Broadcast(ParsedHStruct); // BroadCast
}

void ACPSController::JSONParser_HIERARCHY_NODE(FString StringifiedResponseData)
{
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<TCHAR>::Create(StringifiedResponseData);
	if (!FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Get Data Failed"));
		return;
	}

	TArray< FHStruct> ParsedHStructArray;
	FHStruct Temp;
	for (auto data : JsonArray)
	{
		TSharedPtr< FJsonObject > dataObject = data->AsObject();
		Temp = ParseStructAll<TSharedPtr<FJsonObject>, FHStruct, FHHStruct, FHHHStruct>(dataObject);
		
		// child
		ParsedHStructArray.Add(Temp);
	}

	if (!OnParseVCMData.IsBound())
	{
		//OnParseHDataArray.AddDynamic(WidgetManager, &UCPSWidgetManager::PrintHDataArray);
	}

	OnParseHDataArray.Broadcast(ParsedHStructArray); // BroadCast

}

void ACPSController::JSONParser_UpdateVCMData(FString StringifiedResponseData)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<TCHAR>::Create(StringifiedResponseData);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Get Data Failed"));
		return;
	}
	TArray < TSharedPtr<FJsonValue>> DataArray = JsonObject->GetArrayField("data");

	FVCMStruct TempVCMStruct;
	TArray<FVCMStruct> ParsedVCMStructArray;
	for (auto data : DataArray)
	{
		TempVCMStruct.ItemIndex = data->AsObject()->GetIntegerField("itemId");
		TempVCMStruct.DataName = data->AsObject()->GetStringField("dataName");
		TempVCMStruct.DataValue = data->AsObject()->GetStringField("dataValue");
		TempVCMStruct.VcId = data->AsObject()->GetIntegerField("vcId");
		TempVCMStruct.VcName = data->AsObject()->GetStringField("vcName");
		TempVCMStruct.Type = data->AsObject()->GetIntegerField("type");

		ParsedVCMStructArray.Add(TempVCMStruct);
	}

	OnParseVCMData.Clear();
	OnParseVCMData.AddDynamic(WidgetManager, &UCPSWidgetManager::UpdateVCMData);
	OnParseVCMData.Broadcast(ParsedVCMStructArray); // BroadCast
}


