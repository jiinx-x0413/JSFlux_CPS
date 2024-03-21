// Fill out your copyright notice in the Description page of Project Settings.

#include "CPSPawn.h"
#include "CPSActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPSPawn::ACPSPawn()
	: DefaultRootOffset(FVector(1000, 1200, 100)), DefaultSpringArmRotation(FRotator(-30, 0, 0)), DefaultSpringArmLength(500),
	CameraCurrentLocation(FVector(0, 0, 0)), CameraTargetLocation(DefaultRootOffset),
	SpringArmCurrentRotation(FRotator(0, 0, 0)), SpringArmTargetRotation(DefaultSpringArmRotation),
	SpringArmCurrentLength(0), SpringArmTargetLength(DefaultSpringArmLength),
	OrbitMultiplier(5), ZoomMultiplier(50)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	SpringArm->bDoCollisionTest = false; // camera bug

	// move to Actor Manager
	TArray<AActor*> TempAllActorsArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPSActor::StaticClass(), TempAllActorsArray);
	for (auto e : TempAllActorsArray)
	{
		ActorsLocation.Add(e->GetActorLocation());
	}
	DefaultRootOffset = UKismetMathLibrary::GetVectorArrayAverage(ActorsLocation);

	// input
	InputMappingContext = CreateDefaultSubobject<UInputMappingContext>(TEXT("InputMappingContext"));
}

// Called when the game starts or when spawned
void ACPSPawn::BeginPlay()
{
	Super::BeginPlay();

	// input
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		Cast<APlayerController>(GetOwner())->GetLocalPlayer()
	);
	if (LocalPlayerSubsystem)
	{
		LocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
	}

	// camera
	DefaultCamera();
}

// Called every frame
void ACPSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CameraCurrentLocation != CameraTargetLocation)
	{
		this->SetActorLocation(FMath::VInterpTo(this->GetActorLocation(), CameraTargetLocation, DeltaTime, 5));
		if (FVector::Dist(CameraCurrentLocation, CameraTargetLocation) < 0.5f)
		{
			this->SetActorLocation(CameraTargetLocation);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Move Finished")));
		}
		CameraCurrentLocation = this->GetActorLocation();
	}

	if ((int)SpringArmCurrentRotation.Yaw != (int)SpringArmTargetRotation.Yaw || (int)SpringArmCurrentRotation.Pitch != (int)SpringArmTargetRotation.Pitch)
	{
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArmCurrentRotation, SpringArmTargetRotation, DeltaTime, 5));
		if (FMath::Abs(SpringArmCurrentRotation.Yaw - SpringArmTargetRotation.Yaw) < 0.5f)
		{
			SpringArm->SetRelativeRotation(SpringArmTargetRotation);
			/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rotation Finished : %f / %f ...... same? : %i"), 
				SpringArmCurrentRotation.Yaw, SpringArmTargetRotation.Yaw, ((int)SpringArmCurrentRotation.Yaw == (int)SpringArmTargetRotation.Yaw)? 1:0
			));*/
		}
		SpringArmCurrentRotation = SpringArm->GetRelativeRotation();
	}

	if (SpringArmCurrentLength != SpringArmTargetLength)
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArmCurrentLength, SpringArmTargetLength, DeltaTime, 5);
		if (FMath::Abs(SpringArmCurrentLength - SpringArmTargetLength) < 0.5f)
		{
			SpringArm->TargetArmLength = SpringArmTargetLength;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Zoom Finished : %f / %f"), SpringArm->TargetArmLength, SpringArmCurrentLength));
		}
		SpringArmCurrentLength = SpringArm->TargetArmLength;
	}
}

// Called to bind functionality to input
void ACPSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	TObjectPtr<UEnhancedInputComponent> EnhancedInput = Cast< UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(IA_MouseWheelDown, ETriggerEvent::Triggered, this, &ACPSPawn::Zoom);
		EnhancedInput->BindAction(IA_LeftMouseDown, ETriggerEvent::Started, this, &ACPSPawn::StartOrbit);
		EnhancedInput->BindAction(IA_LeftMouseDown, ETriggerEvent::Completed, this, &ACPSPawn::EndOrbit);
		EnhancedInput->BindAction(IA_MouseMove, ETriggerEvent::Triggered, this, &ACPSPawn::Orbit);
	}
}

void ACPSPawn::DefaultCamera()
{
	Move(DefaultRootOffset, true);
	/*this->SetActorLocation(DefaultRootOffset);
	SpringArm->TargetArmLength = DefaultSpringArmLength;
	SpringArm->SetRelativeRotation(DefaultSpringArmRotation);*/ 

}

void ACPSPawn::Move(const FVector ActorLocation, bool bDefault)
{
	CameraTargetLocation = ActorLocation; // move by tick event
	if (bDefault == false && ActorLocation != FVector(0,0,10))
	{
		SpringArmTargetLength = 100;
	}
	else
	{
		SpringArmTargetLength = DefaultSpringArmLength;
	}
	
	SpringArmTargetRotation = DefaultSpringArmRotation;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("InterPolation Distance = %f"), FVector::Dist(this->GetActorLocation(), ActorLocation)));
}

void ACPSPawn::Zoom(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	SpringArmTargetLength += (ZoomMultiplier * InputValue);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("InputValue = %f / TargetLength = %f"), InputValue, SpringArmTargetLength));
}

void ACPSPawn::StartOrbit(const FInputActionValue& Value)
{
	bIsOrbiting = true;
}

void ACPSPawn::EndOrbit(const FInputActionValue& Value)
{
	bIsOrbiting = false;
}

void ACPSPawn::Orbit(const FInputActionValue& Value)
{
	if (bIsOrbiting)
	{
		float InputValue = Value.Get<float>();

		// signed value : -180~180
		float ConvertedValue = SpringArmTargetRotation.Yaw;
		if (ConvertedValue >= 180 || ConvertedValue <= -180) // 0~360 vs -180 ~ 180
		{
			ConvertedValue = -1 * ConvertedValue; // Rotation % 360?
		}
		SpringArmTargetRotation.Yaw = ConvertedValue + (OrbitMultiplier * InputValue);
	}
}

