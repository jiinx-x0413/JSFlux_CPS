// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VCMStruct.h"
#include "HStruct.h"
#include "ButtonWidget.h"
#include <Map>
#include "CPSWidgetManager.generated.h"

/**
 * 
 */
class ACPSController;
class UUserWidget;
class UButtonWidget;
class UViewWidget;
class PopupWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_SetVCMDataArray, TArray<FVCMStruct>&, TargetDataArray);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_SetHDataArray, TArray<FHStruct>&, TargetDataArray);


UCLASS(BlueprintType, Blueprintable)
class CPS_JSFLUX_API UCPSWidgetManager : public UObject
{
	GENERATED_BODY()

protected:
	UCPSWidgetManager();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Reference", meta = (AllowPrivateAccess = "true"))
	TObjectPtr< ACPSController > PCRef;

	/* TreeView : VCM */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category="Data", meta = (AllowPrivateAccess = "true"))
	TArray<FVCMStruct> VCMDataArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Widget", meta = (AllowPrivateAccess = "true"))
	TMap< int32, UViewWidget* > TreeViewMap;

	/* TreeButton : Hierarchy */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Data", meta = (AllowPrivateAccess = "true"))
	TArray<FHStruct> HDataArray;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Widget", meta = (AllowPrivateAccess = "true"))
	TArray<UButtonWidget*> TreeButtonArray;
	
public:
	/* PC */
	UFUNCTION(BlueprintCallable)
	ACPSController* GetPlayerController() { return PCRef; }

	/* VCM Data */
	UFUNCTION(BlueprintCallable)
	TArray<FVCMStruct> GetVCMDataArray()const { return VCMDataArray; }

	UFUNCTION()
	void SetVCMDataArray(TArray<FVCMStruct>& TargetDataArray);

	UFUNCTION(BlueprintCallable)
	TMap< int32, UViewWidget* > GetTreeViewMap() { return TreeViewMap; }

	UFUNCTION(BlueprintCallable)
	void AddViewWidgetToMap(UViewWidget* TargetViewWidget);

	UFUNCTION(BlueprintCallable)
	void UpdateVCMData(TArray<FVCMStruct>& TargetDataArray);


	/* Hierarchy Data */
	UFUNCTION(BlueprintCallable)
	TArray<FHStruct> GetHDataArray()const { return HDataArray; }

	UFUNCTION()
	void SetHDataArray(TArray<FHStruct>& TargetDataArray);
	
	UFUNCTION(BlueprintCallable)
	TArray<UButtonWidget*> GetTreeButtonArray() const { return TreeButtonArray; }

	UFUNCTION(BlueprintCallable)
	void AddTreeButtonToArray(UButtonWidget* TargetWidget);

public:

	UPROPERTY(BlueprintAssignable)
	FDele_SetVCMDataArray OnSetVCMDataArray;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CreateButtonTreeToUI(TArray<FHStruct>& TargetArray);
	void CreateButtonTreeToUI_Implementation(TArray<FHStruct>& TargetArray);

	UPROPERTY(BlueprintAssignable)
	FDele_SetHDataArray OnSetHDataArray;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CreateViewTreeToUI(TArray<FVCMStruct>& TargetArray);
	void CreateViewTreeToUI_Implementation(TArray<FVCMStruct>& TargetArray);

public:
	UFUNCTION(BlueprintCallable)
	void DeselectAllButtons();
};
