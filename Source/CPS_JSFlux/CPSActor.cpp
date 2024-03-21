// Fill out your copyright notice in the Description page of Project Settings.


#include "CPSActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACPSActor::ACPSActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MySceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MySceneComponent"));
	//MySceneComponent->SetupAttachment(RootComponent);
	RootComponent = MySceneComponent;

	SM_Highlight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMHighlight"));
	SM_HighlightSub = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMHighlightSub"));
	SK_Highlight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKHighlight"));
	SK_HighlightSub = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKHighlightSub"));
	SM_Highlight->SetupAttachment(MySceneComponent);
	SM_HighlightSub->SetupAttachment(MySceneComponent);
	SK_Highlight->SetupAttachment(MySceneComponent);
	SK_HighlightSub->SetupAttachment(MySceneComponent);
}

// Called when the game starts or when spawned
void ACPSActor::BeginPlay()
{
	Super::BeginPlay();
	 
	SetTargetLocation();
	
}

// Called every frame
void ACPSActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPSActor::ToggleHighlight()
{
	if (bIsHighlight)
	{
		if (IsValid(SM_Highlight))
		{
			SM_Highlight->SetRenderCustomDepth(false);
		}
		if (IsValid(SM_HighlightSub))
		{
			SM_HighlightSub->SetRenderCustomDepth(false);
		}
		if (IsValid(SK_Highlight))
		{
			SK_Highlight->SetRenderCustomDepth(false);
		}
		if (IsValid(SK_HighlightSub))
		{
			SK_HighlightSub->SetRenderCustomDepth(false);
		}
		bIsHighlight = false;
	}
	else
	{
		if (IsValid(SM_Highlight))
		{
			SM_Highlight->SetRenderCustomDepth(true);
		}
		if (IsValid(SM_HighlightSub))
		{
			SM_HighlightSub->SetRenderCustomDepth(true);
		}
		if (IsValid(SK_Highlight))
		{
			SK_Highlight->SetRenderCustomDepth(true);
		}
		if (IsValid(SK_HighlightSub))
		{
			SK_HighlightSub->SetRenderCustomDepth(true);
		}
		bIsHighlight = true;
	}
}

void ACPSActor::ToggleHighlight(bool OnClick)
{
	if (IsValid(SM_Highlight))
	{
		SM_Highlight->SetRenderCustomDepth(OnClick);
	}
	if (IsValid(SM_HighlightSub))
	{
		SM_HighlightSub->SetRenderCustomDepth(OnClick);
	}
	if (IsValid(SK_Highlight))
	{
		SK_Highlight->SetRenderCustomDepth(OnClick);
	}
	if (IsValid(SK_HighlightSub))
	{
		SK_HighlightSub->SetRenderCustomDepth(OnClick);
	}
	bIsHighlight = OnClick;
}

bool ACPSActor::HasMeshComponent()
{
	if (!IsValid(SM_Highlight) && !IsValid(SM_HighlightSub) && !IsValid(SK_Highlight) && !IsValid(SK_HighlightSub))
	{
		return false;
	}
	return true;
}

void ACPSActor::SetTargetLocation()
{
	TArray<FVector> TempVectorArray;
	if (IsValid(SM_Highlight) && SM_Highlight->GetStaticMesh() != nullptr)
	{
		TempVectorArray.Add(SM_Highlight->Bounds.Origin);
	}
	if (IsValid(SM_HighlightSub) && SM_Highlight->GetStaticMesh() != nullptr)
	{
		TempVectorArray.Add(SM_Highlight->Bounds.Origin);
	}
	if (IsValid(SK_Highlight) && SK_Highlight->MeshObject != nullptr)
	{
		TempVectorArray.Add(SK_Highlight->Bounds.Origin);
	}
	if (IsValid(SK_HighlightSub) && SK_Highlight->MeshObject != nullptr)
	{
		TempVectorArray.Add(SK_Highlight->Bounds.Origin);
	}

	TargetLocation = UKismetMathLibrary::GetVectorArrayAverage(TempVectorArray);
}

