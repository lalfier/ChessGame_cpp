// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGBishop.h"

// Sets default values
ACGBishop::ACGBishop()
{
	// Structure to hold one-time initialization
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh(TEXT("/Game/Meshes/Pieces/SM_Bishop.SM_Bishop"));

	// Set static mesh
	PieceMesh->SetStaticMesh(ModelMesh.Get());
}

TArray<FIntPoint> ACGBishop::GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize)
{
	TArray<FIntPoint> Moves;

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
