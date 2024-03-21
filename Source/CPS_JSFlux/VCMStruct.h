// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VCMStruct.generated.h"
/**
 * 
 */
class CPS_JSFLUX_API VCMStruct
{
public:
	VCMStruct();
	~VCMStruct();
};

USTRUCT(BlueprintType)
struct FVCMStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ItemIndex = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString DataName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString DataValue = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 VcId = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString VcName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Type = 0;

};