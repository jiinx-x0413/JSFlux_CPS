// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "CPSPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USceneComponent;

class UInputMappingContext;
class UInputAction;

class UButtonWidget;

UCLASS()
class CPS_JSFLUX_API ACPSPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACPSPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	TObjectPtr< USceneComponent > MyRoot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Components")
	TObjectPtr< USpringArmComponent > SpringArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	TObjectPtr< UCameraComponent > Camera;

	// move to camera manager
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FVector DefaultRootOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FRotator DefaultSpringArmRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int64 DefaultSpringArmLength;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void DefaultCamera();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_MouseWheelDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_LeftMouseDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_MouseMove;

	FVector CameraCurrentLocation;
	FVector CameraTargetLocation;
	FRotator SpringArmCurrentRotation;
	FRotator SpringArmTargetRotation;
	float SpringArmCurrentLength;
	float SpringArmTargetLength;
	bool bIsOrbiting;
	float ZoomMultiplier;
	float OrbitMultiplier;

	TArray<FVector> ActorsLocation;

public:
	UFUNCTION(BlueprintCallable)
	FVector GetDefaultRootOffset() { return DefaultRootOffset; }

	UFUNCTION(BlueprintCallable)
	void Move(const FVector ActorLocation, bool bDefault =false);

	UFUNCTION(BlueprintCallable)
	void Zoom(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void StartOrbit(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void EndOrbit(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Orbit(const FInputActionValue& Value);
};
