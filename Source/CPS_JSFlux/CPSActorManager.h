// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CPSActorManager.generated.h"

/**
 * 
 */

class ACPSController;
class ACPSActor;

UCLASS(Blueprintable, BlueprintType)
class CPS_JSFLUX_API UCPSActorManager : public UObject
{
	GENERATED_BODY()

public:
	UCPSActorManager();

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACPSController> PCRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data", meta = (AllowPrivateAccess = "true"))
	TMap<int32, ACPSActor*> ActorMap;

public:
	UFUNCTION(BlueprintCallable)
	TMap<int32, ACPSActor*>& GetActorMap() { return ActorMap; }

	UFUNCTION(BlueprintCallable)
	void InitActor();

};
