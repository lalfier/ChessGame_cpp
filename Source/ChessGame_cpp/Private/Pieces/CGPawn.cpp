// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGPawn.h"

// Sets default values
ACGPawn::ACGPawn()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh;
		FConstructorStatics()
			: ModelMesh(TEXT("/Game/Meshes/Pieces/SM_Pawn.SM_Pawn"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	// Set static mesh
	PieceMesh->SetStaticMesh(ConstructorStatics.ModelMesh.Get());
}

TArray<FIntPoint> ACGPawn::GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize)
{
	TArray<FIntPoint> Moves;
	int32 Direction = (Team == 0) ? 1 : -1;

	// Move two in direction if starting pos
	if(PiecesOnBoard[CurrentX + Direction][CurrentY] == nullptr)
	{
		// White team
		if(Team == 0 && CurrentX == 1 && PiecesOnBoard[CurrentX + (Direction * 2)][CurrentY] == nullptr)
		{
			Moves.Add(FIntPoint(CurrentX + (Direction * 2), CurrentY));
		}
		// Black team
		if(Team == 1 && CurrentX == 6 && PiecesOnBoard[CurrentX + (Direction * 2)][CurrentY] == nullptr)
		{
			Moves.Add(FIntPoint(CurrentX + (Direction * 2), CurrentY));
		}
	}

	if((CurrentX + Direction) < GridSize && (CurrentX + Direction) >= 0)
	{
		// Move one in direction
		if(PiecesOnBoard[CurrentX + Direction][CurrentY] == nullptr)
		{
			Moves.Add(FIntPoint(CurrentX + Direction, CurrentY));
		}

		// Kill Move (diagonal) Right
		if(CurrentY != GridSize - 1)
		{
			if(PiecesOnBoard[CurrentX + Direction][CurrentY + 1] != nullptr && PiecesOnBoard[CurrentX + Direction][CurrentY + 1]->Team != Team)
			{
				Moves.Add(FIntPoint(CurrentX + Direction, CurrentY + 1));
			}
		}
		// Kill Move (diagonal) Left
		if(CurrentY != 0)
		{
			if(PiecesOnBoard[CurrentX + Direction][CurrentY - 1] != nullptr && PiecesOnBoard[CurrentX + Direction][CurrentY - 1]->Team != Team)
			{
				Moves.Add(FIntPoint(CurrentX + Direction, CurrentY - 1));
			}
		}
	}

	return Moves;
}
