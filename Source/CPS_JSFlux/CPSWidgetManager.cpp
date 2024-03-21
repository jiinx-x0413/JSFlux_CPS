// Fill out your copyright notice in the Description page of Project Settings.


#include "CPSWidgetManager.h"
#include "CPSController.h"
#include "ButtonWidget.h"
#include "ViewWidget.h"
#include "VCMStruct.h"
#include "HStruct.h"
#include "HHStruct.h"
#include "HHHStruct.h"
#include <Map>


UCPSWidgetManager::UCPSWidgetManager()
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("WidgetManager Construct")
	));*/
}

void UCPSWidgetManager::SetVCMDataArray(TArray<FVCMStruct>& TargetDataArray)
{
	VCMDataArray = TargetDataArray;

	if (!OnSetVCMDataArray.IsBound())
	{
		OnSetVCMDataArray.AddDynamic(this, &UCPSWidgetManager::CreateViewTreeToUI);
	}
	OnSetVCMDataArray.Broadcast(VCMDataArray);
}

void UCPSWidgetManager::AddViewWidgetToMap(UViewWidget* TargetViewWidget)
{
	int32 TargetId = TargetViewWidget->GetIndex();
	if (TreeViewMap.Find(TargetId))
	{
		return;
	}
	TreeViewMap.Add({ TargetId , TargetViewWidget });
}

void UCPSWidgetManager::UpdateVCMData(TArray<FVCMStruct>& TargetDataArray)
{
	VCMDataArray = TargetDataArray;

	for (auto Data : TargetDataArray)
	{
		TreeViewMap[Data.ItemIndex]->UpdateValue(Data.DataValue);	
	}
}


void UCPSWidgetManager::SetHDataArray(TArray<FHStruct>& TargetDataArray)
{
	HDataArray = TargetDataArray;

	if (!OnSetHDataArray.IsBound())
	{
		OnSetHDataArray.AddDynamic(this, &UCPSWidgetManager::CreateButtonTreeToUI);
	}
	OnSetHDataArray.Broadcast(HDataArray);
}

//void UCPSWidgetManager::AddTreeButtonToArray(UTreeButtonWidget* TargetWidget)
void UCPSWidgetManager::AddTreeButtonToArray(UButtonWidget* TargetWidget)
{
	TargetWidget->SetWidgetManager(this);
	TreeButtonArray.Add(TargetWidget);
}

void UCPSWidgetManager::CreateButtonTreeToUI_Implementation(TArray<FHStruct>& TargetArray)
{
	// Create and Add Button Widget at BP_WidgetManager
}

void UCPSWidgetManager::CreateViewTreeToUI_Implementation(TArray<FVCMStruct>& TargetArray)
{
	// Create and Add View Widget at BP_WidgetManager
}

void UCPSWidgetManager::DeselectAllButtons()
{
	for (auto TreeButton : GetTreeButtonArray())
	{
		TreeButton->ToggleButton(false);
	}
}


