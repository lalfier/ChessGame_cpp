// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pieces/CGChessPiece.h"
#include "CGRook.generated.h"

/**
 * Derived class for each chess piece type.
 */
UCLASS()
class CHESSGAME_CPP_API ACGRook : public ACGChessPiece
{
	GENERATED_BODY()
	
public:
	ACGRook();

	virtual TArray<FIntPoint> GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize) override;
};
