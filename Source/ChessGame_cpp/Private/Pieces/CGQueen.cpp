// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGQueen.h"

// Sets default values
ACGQueen::ACGQueen()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh;
		FConstructorStatics()
			: ModelMesh(TEXT("/Game/Meshes/Pieces/SM_Queen.SM_Queen"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	// Set static mesh
	PieceMesh->SetStaticMesh(ConstructorStatics.ModelMesh.Get());
}

TArray<FIntPoint> ACGQueen::GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize)
{
	TArray<FIntPoint> Moves;

	// Move Down
	for(int32 i = CurrentX - 1; i >= 0; i--)
	{
		if(PiecesOnBoard[i][CurrentY] == nullptr)
		{
			Moves.Add(FIntPoint(i, CurrentY));
		}
		else
		{
			if(PiecesOnBoard[i][CurrentY]->Team != Team)
			{
				Moves.Add(FIntPoint(i, CurrentY));
			}

			break;
		}
	}

	// Move Up
	for(int32 i = CurrentX + 1; i < GridSize; i++)
	{
		if(PiecesOnBoard[i][CurrentY] == nullptr)
		{
			Moves.Add(FIntPoint(i, CurrentY));
		}
		else
		{
			if(PiecesOnBoard[i][CurrentY]->Team != Team)
			{
				Moves.Add(FIntPoint(i, CurrentY));
			}

			break;
		}
	}

	// Move Left
	for(int32 i = CurrentY - 1; i >= 0; i--)
	{
		if(PiecesOnBoard[CurrentX][i] == nullptr)
		{
			Moves.Add(FIntPoint(CurrentX, i));
		}
		else
		{
			if(PiecesOnBoard[CurrentX][i]->Team != Team)
			{
				Moves.Add(FIntPoint(CurrentX, i));
			}

			break;
		}
	}

	// Move Right
	for(int32 i = CurrentY + 1; i < GridSize; i++)
	{
		if(PiecesOnBoard[CurrentX][i] == nullptr)
		{
			Moves.Add(FIntPoint(CurrentX, i));
		}
		else
		{
			if(PiecesOnBoard[CurrentX][i]->Team != Team)
			{
				Moves.Add(FIntPoint(CurrentX, i));
			}

			break;
		}
	}

	// Top Right Movement
	for(int32 x = CurrentX + 1, y = CurrentY + 1; x < GridSize && y < GridSize; x++, y++)
	{
		if(PiecesOnBoard[x][y] == nullptr)
		{
			Moves.Add(FIntPoint(x, y));
		}
		else
		{
			if(PiecesOnBoard[x][y]->Team != Team)
			{
				Moves.Add(FIntPoint(x, y));
			}

			break;
		}
	}

	// Top Left Movement
	for(int32 x = CurrentX + 1, y = CurrentY - 1; x < GridSize && y >= 0; x++, y--)
	{
		if(PiecesOnBoard[x][y] == nullptr)
		{
			Moves.Add(FIntPoint(x, y));
		}
		else
		{
			if(PiecesOnBoard[x][y]->Team != Team)
			{
				Moves.Add(FIntPoint(x, y));
			}

			break;
		}
	}

	// Bottom Right Movement
	for(int32 x = CurrentX - 1, y = CurrentY + 1; x >= 0 && y < GridSize; x--, y++)
	{
		if(PiecesOnBoard[x][y] == nullptr)
		{
			Moves.Add(FIntPoint(x, y));
		}
		else
		{
			if(PiecesOnBoard[x][y]->Team != Team)
			{
				Moves.Add(FIntPoint(x, y));
			}

			break;
		}
	}

	// Bottom Left Movement
	for(int32 x = CurrentX - 1, y = CurrentY - 1; x >= 0 && y >= 0; x--, y--)
	{
		if(PiecesOnBoard[x][y] == nullptr)
		{
			Moves.Add(FIntPoint(x, y));
		}
		else
		{
			if(PiecesOnBoard[x][y]->Team != Team)
			{
				Moves.Add(FIntPoint(x, y));
			}

			break;
		}
	}

	return Moves;
}
