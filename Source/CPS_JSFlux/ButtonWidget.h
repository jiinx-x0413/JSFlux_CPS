// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HStruct.h"
#include "ButtonWidget.generated.h"

/**
 * 
 */

class UButton;
class UTextBlock;
class UTreeButtonWidget;
class UCPSWidgetManager;
class ACPSActor;

UCLASS()
class CPS_JSFLUX_API UButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

	/* Data */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ExposeOnSpawn = "true"))
	int32 NodeId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ExposeOnSpawn = "true"))
	FText NodeName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ExposeOnSpawn = "true"))
	int32 ParentId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ExposeOnSpawn = "true"))
	int32 Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ExposeOnSpawn = "true"))
	int32 NodeDepth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ExposeOnSpawn = "true"))
	FHStruct TopNodeData;

	

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = "true"))
	bool bAllChildrenOnClicked;*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta= (AllowPrivateAccess = "true"))
	FVector TargetActorLocation;

	/* Reference */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reference")
	TObjectPtr<UButton> ButtonWidgetComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reference", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TObjectPtr<UButtonWidget> ParentButton = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UButtonWidget>> Children; /* Tree View UI */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACPSActor> TargetActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reference", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TObjectPtr<UCPSWidgetManager> WidgetManager;

	
public:
	/* Data */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bIsOnClicked;

	UFUNCTION(BlueprintCallable)
	int64 GetIndex() const { return NodeId; }

	UFUNCTION(BlueprintCallable)
	void SetIndex(int32 NewIndex) { NodeId = NewIndex; }

	UFUNCTION(BlueprintCallable)
	FText GetName() const { return NodeName; }

	UFUNCTION(BlueprintCallable)
	void SetName(FText NewName) { NodeName = NewName; }

	/* Reference */
	UFUNCTION(BlueprintCallable)
	UCPSWidgetManager* GetWidgetManager() { return WidgetManager; }

	UFUNCTION(BlueprintCallable)
	void SetWidgetManager(UCPSWidgetManager* TargetWidgetManager) { WidgetManager = TargetWidgetManager; }

	UFUNCTION(BlueprintCallable)
	UButtonWidget* GetParentButton() const { return ParentButton; }

	UFUNCTION(BlueprintCallable)
	void SetParentButton(UButtonWidget* ButtonWidget) { ParentButton = ButtonWidget; }

	UFUNCTION(BlueprintCallable)
	TArray<UButtonWidget*> GetChildren() { return Children; }

	/* Actor */
	UFUNCTION(BlueprintCallable)
	ACPSActor* GetTargetActor() const { return TargetActor; }

	UFUNCTION(BlueprintCallable)
	void SetTargetActor(ACPSActor* TargetActorObject);
	
	UFUNCTION(BlueprintCallable)
	FVector GetTargetActorLocation() { return TargetActorLocation; }

	UFUNCTION(BlueprintCallable)
	void SetTargetActorLocation();
	void SetTargetActorLocation(UButtonWidget* ParentButtonWidget);

public:
	UFUNCTION(BlueprintCallable)
	void ToggleButton();
	void ToggleButton(bool OnClick); // overloading

	/* Button */
	UFUNCTION()
	void HighlightButton(bool OnClick);

	/* Actor */
	UFUNCTION()
	void HighlightActor(bool OnClick);

	/* Camera */
	UFUNCTION()
	void MoveCameraToActor(bool OnClick);

};
