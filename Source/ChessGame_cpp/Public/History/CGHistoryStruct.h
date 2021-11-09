// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pieces/CGChessPiece.h"
#include "CGHistoryStruct.generated.h"

USTRUCT(BlueprintType)
struct FCGHistoryStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 Team;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ChessPieceType> PieceType;

	UPROPERTY(BlueprintReadWrite)
	FIntPoint GridLocation;

	UPROPERTY(BlueprintReadWrite)
	FString ActionStr;

	UPROPERTY(BlueprintReadWrite)
	FIntPoint PrevLocation;
};
