// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHStruct.h"
#include "HStruct.generated.h"

/**
 * 
 */
class CPS_JSFLUX_API HStruct
{
public:
	HStruct();
	~HStruct();
};

USTRUCT(BlueprintType)
struct FHStruct : public FHHStruct
{
	//GENERATED_USTRUCT_BODY()
	GENERATED_BODY()

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NodeId = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ParentId = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString NodeName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Type = 0;*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FHHStruct> Child;

};
