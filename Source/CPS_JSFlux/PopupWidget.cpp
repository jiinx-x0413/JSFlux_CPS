// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupWidget.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"


bool UPopupWidget::Initialize()
{
	Super::Initialize();

	BTN_Quit->OnClicked.AddDynamic(this, &UPopupWidget::RemoveWidget);
	BTN_Cancel->OnClicked.AddDynamic(this, &UPopupWidget::RemoveWidget);
	
	HideAllButton();

	return false;
}

void UPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// set position in viewport
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ReticleSize(this->GetDesiredSize().X, this->GetDesiredSize().Y);

	const float CenterX = ViewportSize.X / 2;
	const float CenterY = ViewportSize.Y / 2;
	const float ReticleCenterX = ReticleSize.X / 2;
	const float ReticleCenterY = ReticleSize.Y / 2;

	this->SetPositionInViewport(FVector2D(CenterX - ReticleCenterX, CenterY - ReticleCenterY), false);
}

void UPopupWidget::RemoveWidget()
{
	
	if (this)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPopupWidget::OnclickConfirm()
{
	if(this)
	{ 
		this->RemoveWidget();
	}
}

void UPopupWidget::Popup(EPopupType Etype)
{
	BTN_Ok->OnClicked.RemoveAll(this);
	
	// type
	switch (Etype)
	{
	case EPopupType::defaultValue:
		break;
	case EPopupType::Confirm:
		HideAllButton();
		ShowMessage();
		ShowDoubleButton();
		BTN_Ok->OnClicked.AddDynamic(this, &UPopupWidget::OnclickConfirm);
		break;
	case EPopupType::Warning:
		HideAllButton();
		ShowMessage();
		ShowSingleButton();
		BTN_Ok->OnClicked.AddDynamic(this, &UPopupWidget::OnclickConfirm);
		break;
	case EPopupType::ContentOnly:
		HideAllButton();
		ShowContent();
		break;
	}

	this->SetVisibility(ESlateVisibility::Visible);
}

void UPopupWidget::HideAllButton()
{
	BTN_Ok->SetVisibility(ESlateVisibility::Collapsed);
	BTN_Cancel->SetVisibility(ESlateVisibility::Collapsed);
	TB_Message->SetVisibility(ESlateVisibility::Collapsed);
	WB_Content->SetVisibility(ESlateVisibility::Collapsed);
}

void UPopupWidget::ShowSingleButton()
{
	if (BTN_Ok)
	{
		BTN_Ok->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPopupWidget::ShowDoubleButton()
{
	if (BTN_Ok && BTN_Cancel)
	{
		BTN_Ok->SetVisibility(ESlateVisibility::Visible);
		BTN_Cancel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPopupWidget::ShowMessage()
{
	if (TB_Message)
	{
		TB_Message->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPopupWidget::ShowContent()
{
	if (WB_Content)
	{
		WB_Content->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPopupWidget::SetMessage(FText InText)
{
	if (TB_Message)
	{
		TB_Message->SetText(InText);
	}
}

void UPopupWidget::SetTitle(FText InText)
{
	if (TB_Title)
	{
		TB_Title->SetText(InText);
	}
}

void UPopupWidget::Append(UUserWidget* ChildWidget)
{
	if (IsValid(WB_Content))
	{
		WB_Content->ClearChildren();
		WB_Content->AddChildToWrapBox(ChildWidget);
	}
}
