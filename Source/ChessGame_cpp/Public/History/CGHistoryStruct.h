// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pieces/CGChessPiece.h"
#include "CGHistoryStruct.generated.h"

/**
 * The struct for history list.
 */
USTRUCT(BlueprintType)
struct FCGHistoryStruct
{
	GENERATED_BODY()

public:
	/** 0 for White, 1 for Black. */
	UPROPERTY(BlueprintReadWrite)
	int32 Team;

	/** Enum holding chess piece type (Pawn, Rook, ...). */
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ChessPieceType> PieceType;

	/** Current grid location. */
	UPROPERTY(BlueprintReadWrite)
	FIntPoint GridLocation;

	/** Action done with movement (Special Move, Eat, ...). */
	UPROPERTY(BlueprintReadWrite)
	FString ActionStr;

	/** Previous grid location. */
	UPROPERTY(BlueprintReadWrite)
	FIntPoint PrevLocation;
};
