// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ViewWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_UpdateValue, const FString&, NewValue);

UCLASS()
class CPS_JSFLUX_API UViewWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	int32 ItemIndex;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	FString DataValue;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetIndex() { return ItemIndex; }

	UPROPERTY(BlueprintAssignable)
	FDele_UpdateValue OnValueUpdated;

	UFUNCTION(BlueprintCallable)
	FString GetDataValue() { return DataValue; }

	UFUNCTION(BlueprintCallable)
	void SetDataValue(FString NewValue) { DataValue = NewValue; }

	UFUNCTION(BlueprintCallable)
	void UpdateValue(FString NewValue);

	UFUNCTION(BlueprintNativeEvent)
	void SetTextUpdateValue(const FString& NewValue);
	void SetTextUpdateValue_Implementation(const FString& NewValue);

};
