// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CPSWidgetManager.h"
#include "CPSController.h"
#include "CPSActor.h"
#include "CPSPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsOnClicked = false;
}



void UButtonWidget::ToggleButton() // child button clicked
{
	// exception
	if (
		!IsValid(TargetActor) && GetTargetActorLocation().IsZero() &&
		(IsValid(ParentButton) && (!ParentButton->GetTargetActorLocation().IsZero() || NodeDepth == 2))
		)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Exception Case")));
		ParentButton->ToggleButton();
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("General Case")));
	if (bIsOnClicked == false || (IsValid(ParentButton) && ParentButton->bIsOnClicked == true)) // 해당 태그 버튼을 전체선택 중일 때
	{
		GetWidgetManager()->DeselectAllButtons(); // Only for child button onclick	
	}

	ToggleButton(!bIsOnClicked);

	// camera
	MoveCameraToActor(bIsOnClicked);
}

void UButtonWidget::ToggleButton(bool OnClick) // parent button clicked
{
	// self
	HighlightButton(OnClick);
	HighlightActor(OnClick);
	bIsOnClicked = OnClick;

	// Children
	if (Children.IsEmpty())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Children Empty : %i"), Children.Num()));
		return;
	}

	for (auto child : Children)
	{
		if (IsValid(child->ButtonWidgetComponent))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Child who has Valid Button Component : %s"), *child.GetName()));
			child->ToggleButton(OnClick);
		}
	}
	
}

void UButtonWidget::MoveCameraToActor(bool OnClick)
{	
	TObjectPtr<ACPSPawn> MyPawn = Cast<ACPSPawn>(GetWidgetManager()->GetPlayerController()->GetPawn());
	
	if (IsValid(MyPawn))
	{
		if (OnClick && !GetTargetActorLocation().IsZero())
		{
			MyPawn->Move(GetTargetActorLocation());
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Location : %i, %i, %i"), GetTargetActorLocation().X, GetTargetActorLocation().Y, GetTargetActorLocation().Z));
		}
		else
		{
			MyPawn->DefaultCamera();
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pawn is not valid at ButtonWidget"));
	}
}

void UButtonWidget::SetTargetActor(ACPSActor* TargetActorObject)
{
	if (!IsValid(TargetActorObject))
	{
		return;
	}
	TargetActor = TargetActorObject;
	TargetActor->SetTargetButton(this);
	SetTargetActorLocation();
}

void UButtonWidget::SetTargetActorLocation()
{
	
	if (TargetActor && IsValid(TargetActor))
	{
		//TargetActorLocation = TargetActor->GetActorLocation();
		TargetActorLocation = TargetActor->TargetLocation;
	}
	else if (!this->GetChildren().IsEmpty())
	{
		SetTargetActorLocation(this);
	}
	else
	{
		TargetActorLocation = FVector(0, 0, 0);
	} 

	if (IsValid(ParentButton) && !IsValid(ParentButton->GetTargetActor()))
	{
		SetTargetActorLocation(ParentButton);
	}
}

void UButtonWidget::SetTargetActorLocation(UButtonWidget* ParentButtonWidget)
{
	if (ParentButtonWidget->GetChildren().IsEmpty())
	{
		return;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Set Parent Button : %i (CHildren : %i)/ from : %i"), ParentButtonWidget->GetIndex(), ParentButtonWidget->GetChildren().Num(), GetIndex()));

	TArray<FVector> TempVectorArray;
	for (auto Child : ParentButtonWidget->GetChildren())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Parent : %i / Sibling : %i"), ParentButtonWidget->GetIndex(), Child->GetIndex()));
		if (!Child->GetTargetActorLocation().IsZero())
		{
			TempVectorArray.Add(Child->GetTargetActorLocation());
			
		}
		
	}
	ParentButtonWidget->TargetActorLocation = UKismetMathLibrary::GetVectorArrayAverage(TempVectorArray);
}

void UButtonWidget::HighlightButton(bool OnClick)
{
	// button highlight (toggle)
	if (OnClick == true)
	{
		ButtonWidgetComponent->SetBackgroundColor(FColor::Blue);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("highlight button on"));
	}
	else
	{
		ButtonWidgetComponent->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("highlight button off"));
	}

}

void UButtonWidget::HighlightActor(bool OnClick)
{
	// get target actor ; To ActorManager
	if (TargetActor)
	{
		// highlightActor
		if (OnClick == true)
		{
			TargetActor->ToggleHighlight(true);
		}
		else
		{
			TargetActor->ToggleHighlight(false);
		}
		return;
	}
}
