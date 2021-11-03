// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGKing.h"

// Sets default values
ACGKing::ACGKing()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh;
		FConstructorStatics()
			: ModelMesh(TEXT("/Game/Meshes/Pieces/SM_King.SM_King"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	// Set static mesh
	PieceMesh->SetStaticMesh(ConstructorStatics.ModelMesh.Get());
}

TArray<FIntPoint> ACGKing::GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize)
{
	TArray<FIntPoint> Moves;
		
	if(CurrentY + 1 < GridSize)
	{
		// Move Right
		if(PiecesOnBoard[CurrentX][CurrentY + 1] == nullptr)
		{
			Moves.Add(FIntPoint(CurrentX, CurrentY + 1));
		}
		else
		{
			if(PiecesOnBoard[CurrentX][CurrentY + 1]->Team != Team)
			{
				Moves.Add(FIntPoint(CurrentX, CurrentY + 1));
			}
		}

		// Move Right Up
		if(CurrentX + 1 < GridSize)
		{
			if(PiecesOnBoard[CurrentX + 1][CurrentY + 1] == nullptr)
			{
				Moves.Add(FIntPoint(CurrentX + 1, CurrentY + 1));
			}
			else
			{
				if(PiecesOnBoard[CurrentX + 1][CurrentY + 1]->Team != Team)
				{
					Moves.Add(FIntPoint(CurrentX + 1, CurrentY + 1));
				}
			}
		}

		// Move Right Down
		if(CurrentX - 1 >= 0)
		{
			if(PiecesOnBoard[CurrentX - 1][CurrentY + 1] == nullptr)
			{
				Moves.Add(FIntPoint(CurrentX - 1, CurrentY + 1));
			}
			else
			{
				if(PiecesOnBoard[CurrentX - 1][CurrentY + 1]->Team != Team)
				{
					Moves.Add(FIntPoint(CurrentX - 1, CurrentY + 1));
				}
			}
		}
	}

	if(CurrentY - 1 >= 0)
	{
		// Move Left
		if(PiecesOnBoard[CurrentX][CurrentY - 1] == nullptr)
		{
			Moves.Add(FIntPoint(CurrentX, CurrentY - 1));
		}
		else
		{
			if(PiecesOnBoard[CurrentX][CurrentY - 1]->Team != Team)
			{
				Moves.Add(FIntPoint(CurrentX, CurrentY - 1));
			}
		}

		// Move Left Up
		if(CurrentX + 1 < GridSize)
		{
			if(PiecesOnBoard[CurrentX + 1][CurrentY - 1] == nullptr)
			{
				Moves.Add(FIntPoint(CurrentX + 1, CurrentY - 1));
			}
			else
			{
				if(PiecesOnBoard[CurrentX + 1][CurrentY - 1]->Team != Team)
				{
					Moves.Add(FIntPoint(CurrentX + 1, CurrentY - 1));
				}
			}
		}

		// Move Left Down
		if(CurrentX - 1 >= 0)
		{
			if(PiecesOnBoard[CurrentX - 1][CurrentY - 1] == nullptr)
			{
				Moves.Add(FIntPoint(CurrentX - 1, CurrentY - 1));
			}
			else
			{
				if(PiecesOnBoard[CurrentX - 1][CurrentY - 1]->Team != Team)
				{
					Moves.Add(FIntPoint(CurrentX - 1, CurrentY - 1));
				}
			}
		}
	}

	// Move Up
	if(CurrentX + 1 < GridSize)
	{
		if(PiecesOnBoard[CurrentX + 1][CurrentY] == nullptr)
		{
			Moves.Add(FIntPoint(CurrentX + 1, CurrentY));
		}
		else
		{
			if(PiecesOnBoard[CurrentX + 1][CurrentY]->Team != Team)
			{
				Moves.Add(FIntPoint(CurrentX + 1, CurrentY));
			}
		}
	}

	// Move Down
	if(CurrentX - 1 >= 0)
	{
		if(PiecesOnBoard[CurrentX - 1][CurrentY] == nullptr)
		{
			Moves.Add(FIntPoint(CurrentX - 1, CurrentY));
		}
		else
		{
			if(PiecesOnBoard[CurrentX - 1][CurrentY]->Team != Team)
			{
				Moves.Add(FIntPoint(CurrentX - 1, CurrentY));
			}
		}
	}

	return Moves;
}
