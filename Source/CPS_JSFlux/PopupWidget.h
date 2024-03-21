// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupWidget.generated.h"

/**
 * 
 */

class UHorizontalBox;
class UButton;
class UTextBlock;
class UWrapBox;

UENUM(BlueprintType)
enum class EPopupType : uint8
{
	Confirm,
	Warning,
	ContentOnly,

	defaultValue
};

UCLASS()
class CPS_JSFLUX_API UPopupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(SpawnOnExpose))
	EPopupType type = EPopupType::defaultValue;

	UPROPERTY(EditAnywhere, Category = "Widget Class")
	TSubclassOf<UUserWidget> WidgetClass;

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	FVector2D ViewportSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (BindWidget))
	TObjectPtr< UWrapBox > WB_Content;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (BindWidget))
	TObjectPtr< UHorizontalBox > HB_TopBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (BindWidget))
	TObjectPtr< UButton > BTN_Ok;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (BindWidget))
	TObjectPtr< UButton > BTN_Cancel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (BindWidget))
	TObjectPtr< UButton > BTN_Quit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (BindWidget))
	TObjectPtr< UTextBlock > TB_Title;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (BindWidget))
	TObjectPtr< UTextBlock > TB_Message;

	UFUNCTION(BlueprintCallable)
	void SetMessage(FText InText);

	UFUNCTION(BlueprintCallable)
	void SetTitle(FText InText);


public:
	UFUNCTION(BlueprintCallable)
	void Append(UUserWidget* ChildWidget);

	UFUNCTION(BlueprintCallable)
	void RemoveWidget();

	UFUNCTION()
	virtual void OnclickConfirm();

	UFUNCTION(BlueprintCallable)
	void Popup(EPopupType Etype);

	UFUNCTION()
	void HideAllButton();

	UFUNCTION(BlueprintCallable)
	void ShowSingleButton();

	UFUNCTION(BlueprintCallable)
	void ShowDoubleButton();

	UFUNCTION(BlueprintCallable)
	void ShowMessage();

	UFUNCTION(BlueprintCallable)
	void ShowContent();

};
