// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pieces/CGChessPiece.h"
#include "CGKnight.generated.h"

UCLASS()
class CHESSGAME_CPP_API ACGKnight : public ACGChessPiece
{
	GENERATED_BODY()
	
public:
	ACGKnight();

	virtual TArray<FIntPoint> GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize) override;
};
