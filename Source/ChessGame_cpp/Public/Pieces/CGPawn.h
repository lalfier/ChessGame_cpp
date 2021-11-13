// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pieces/CGChessPiece.h"
#include "CGPawn.generated.h"

/**
 * Derived class for each chess piece type.
 */
UCLASS()
class CHESSGAME_CPP_API ACGPawn : public ACGChessPiece
{
	GENERATED_BODY()
	
public:
	ACGPawn();

	virtual TArray<FIntPoint> GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize) override;

	virtual ChessSpecialMove GetSpecialMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], TArray<TArray<FIntPoint>>& MoveList, TArray<FIntPoint>& AvailableMoves) override;
};
