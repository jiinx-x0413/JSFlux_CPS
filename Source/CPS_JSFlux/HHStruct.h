// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHHStruct.h"
#include "HHStruct.generated.h"
/**
 * 
 */
class CPS_JSFLUX_API HHStruct
{
public:
	HHStruct();
	~HHStruct();
};

USTRUCT(BlueprintType)
struct FHHStruct : public FHHHStruct
{
	GENERATED_USTRUCT_BODY()

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NodeId = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ParentId = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString NodeName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Type = 0;*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FHHHStruct> GrandChild;

};