// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CGSaveStruct.generated.h"

USTRUCT()
struct FCGSaveStruct
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FIntPoint> Moves;
};
