// Fill out your copyright notice in the Description page of Project Settings.


#include "CPSActorManager.h"
#include "Kismet/GameplayStatics.h"
#include "CPSActor.h"


UCPSActorManager::UCPSActorManager()
{
	InitActor();
}

void UCPSActorManager::InitActor()
{
	TArray<AActor*> TempArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPSActor::StaticClass(), TempArray);
	TObjectPtr<ACPSActor> TempActor;
	for (auto actor : TempArray)
	{
		TempActor = Cast<ACPSActor>(actor);
		ActorMap.Add({ TempActor->Index , TempActor });
	}
}
