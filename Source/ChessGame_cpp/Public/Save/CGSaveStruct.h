// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CGSaveStruct.generated.h"

/**
 * The struct for save game moves list.
 */
USTRUCT()
struct FCGSaveStruct
{
	GENERATED_BODY()

public:
	/** [0] is previous position, [1] is current position. */
	UPROPERTY()
	TArray<FIntPoint> Moves;
};
