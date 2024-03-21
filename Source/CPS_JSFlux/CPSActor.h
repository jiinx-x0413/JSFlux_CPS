// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPSActor.generated.h"

class UButtonWidget;

UCLASS()
class CPS_JSFLUX_API ACPSActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPSActor();

	// Actor의 Index를 Button 과 일치시켜야 함. 그래야 클릭 이벤트로 액터에 접근 가능
	// 초기화 하는 지점이 Button 추가하는 지점? : ActorManager 생성시 인덱스를 매니저에서 통합 관리
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta=(ExposeOnSpawn = "true"))
	int64 Index;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ExposeOnSpawn = "true"))
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FVector TargetLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	int64 GetIndex() const { return Index; };

	UFUNCTION(BlueprintCallable)
	void SetIndex(int64 NewIndex) { Index = NewIndex; }

	UFUNCTION(BlueprintCallable)
	void ToggleHighlight();
	void ToggleHighlight(bool OnClick); // overloading

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr< USceneComponent > MySceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	UStaticMeshComponent* SM_Highlight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* SM_HighlightSub = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USkeletalMeshComponent* SK_Highlight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USkeletalMeshComponent* SK_HighlightSub = nullptr;

	UPROPERTY(EditAnywhere)
	bool bIsHighlight = false;

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButtonWidget> TargetButton;

public:
	UFUNCTION()
	void SetTargetButton(UButtonWidget* TargetWidget) { TargetButton = TargetWidget; }

	//UFUNCTION()
	//void SetAvgLocation();

	UFUNCTION()
	void SetTargetLocation();

	UFUNCTION()
	bool HasMeshComponent();
};
