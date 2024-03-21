// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewWidget.h"


void UViewWidget::UpdateValue(FString NewValue)
{
	SetDataValue(NewValue);
	if (!OnValueUpdated.IsBound())
	{
		OnValueUpdated.AddDynamic(this, &UViewWidget::SetTextUpdateValue);
	}
	OnValueUpdated.Broadcast(NewValue);
}

void UViewWidget::SetTextUpdateValue_Implementation(const FString& NewValue)
{

}

