// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VCMStruct.h"
#include "HStruct.h"
#include "CPSController.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_ParseVCMData, TArray<FVCMStruct>&, ParsedVCMData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_ParseHData, FHStruct, ParsedHData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_ParseHDataArray, TArray<FHStruct>&, ParsedHDataArray);

class UHttpObject;
class UCPSWidgetManager;
class UCPSActorManager;

UENUM(BlueprintType)
enum class EAPIType : uint8
{
	GETMAIN, // api/vcmdata/getmain
	HIERARCHY, // hierarchy
	HIERARCHY_PARENT, // hierarchy/parent/{node_id}
	HIERARCHY_CHILD, // hierarchy/child/{node_id}
	HIERARCHY_NODE, // hierarchy/node/{node_id}
	
	UPDATE_VCMDATA, // 

	defaultValue
};


UCLASS()
class CPS_JSFLUX_API ACPSController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACPSController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void CreateLogFile(FString FileName, FString LogMessage);

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHttpObject> HttpObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Object", meta = (AllowPrivateAccess = "true"))
	TObjectPtr< UCPSWidgetManager > WidgetManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TObjectPtr< UCPSActorManager > ActorManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bIsRunning;

	float WaitSeconds;
public:
	UFUNCTION(BlueprintCallable)
	void RequestData(EAPIType APIType, FString Parameters = "");

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UHttpObject* GetHttpObject() const { return HttpObject; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCPSWidgetManager* GetWidgetManager() const { return WidgetManager; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCPSActorManager* GetActorManager() const { return ActorManager; }

	UFUNCTION(BlueprintCallable)
	void BindActorWithButton(UButtonWidget* TargetButtonWidget);

	/* VCM Data */
	UPROPERTY(BlueprintAssignable)
	FDele_ParseVCMData OnParseVCMData;

	UFUNCTION()
	void JSONParser_GETMAIN(FString StringifiedResponseData);


	/* Hierarchy Data */
	UPROPERTY(BlueprintAssignable)
	FDele_ParseHDataArray OnParseHDataArray;

	UPROPERTY(BlueprintAssignable)
	FDele_ParseHData OnParseHData;

	UFUNCTION()
	void JSONParser_HIERARCHY(FString StringifiedResponseData);

	UFUNCTION()
	void JSONParser_HIERARCHY_PARENT(FString StringifiedResponseData);

	UFUNCTION()
	void JSONParser_HIERARCHY_CHILD(FString StringifiedResponseData);

	UFUNCTION()
	void JSONParser_HIERARCHY_NODE(FString StringifiedResponseData);

	UFUNCTION()
	void JSONParser_UpdateVCMData(FString StringifiedResponseData);
};

template<typename ShrPtr, typename StructType>
StructType ParseStruct(ShrPtr TargetJsonObject)
{
	StructType Temp;
	Temp.NodeId = TargetJsonObject->GetIntegerField("node_id");
	Temp.ParentId = TargetJsonObject->GetIntegerField("parent_id");
	Temp.NodeName = TargetJsonObject->GetStringField("node_name");
	Temp.Type = TargetJsonObject->GetIntegerField("type");

	return Temp;
}

template<typename ShrPtr, typename StructType, typename ChildStructType, typename GrandChildStructType>
StructType ParseStructAll(ShrPtr TargetJsonObject)
{
	StructType Temp;
	ChildStructType TempChild;
	GrandChildStructType TempGrandChild;

	Temp = ParseStruct <ShrPtr&, StructType >(TargetJsonObject);
	if (TargetJsonObject->GetArrayField("child").Num() > 0) // child
	{
		for (auto ChildData : TargetJsonObject->GetArrayField("child"))
		{
			ShrPtr ChildDataObject = ChildData->AsObject();
			if (ChildDataObject->GetIntegerField("type") > 0)
			{
				continue;
			}
			TempChild = ParseStruct <ShrPtr&, ChildStructType >(ChildDataObject);
			if (ChildDataObject->GetArrayField("child").Num() > 0) // grandchild
			{
				for (auto GrandChildData : ChildDataObject->GetArrayField("child"))
				{
					ShrPtr GrandChildDataObject = GrandChildData->AsObject();
					if (GrandChildDataObject->GetIntegerField("type") > 0)
					{
						continue;
					}
					TempGrandChild = ParseStruct <ShrPtr&, GrandChildStructType>(GrandChildDataObject);
					TempChild.GrandChild.Add(TempGrandChild);
				}
			}
			Temp.Child.Add(TempChild);
		}
	}
	
	return Temp;
}