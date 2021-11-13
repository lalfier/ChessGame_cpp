// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGKnight.h"

ACGKnight::ACGKnight()
{
	// Structure to hold one-time initialization
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh(TEXT("/Game/Meshes/Pieces/SM_Knight.SM_Knight"));

	// Set static mesh
	PieceMesh->SetStaticMesh(ModelMesh.Get());
}

TArray<FIntPoint> ACGKnight::GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize)
{
	// Add move if tile is empty or there is enemy piece on tile, else check other positions.
	TArray<FIntPoint> Moves;

	// Move Top Right
	int32 XPos = CurrentX + 2;
	int32 YPos = CurrentY + 1;
	if(XPos < GridSize && YPos < GridSize)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Move Right Top
	XPos = CurrentX + 1;
	YPos = CurrentY + 2;
	if(XPos < GridSize && YPos < GridSize)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Move Top Left
	XPos = CurrentX + 2;
	YPos = CurrentY - 1;
	if(XPos < GridSize && YPos >= 0)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Move Left Top
	XPos = CurrentX + 1;
	YPos = CurrentY - 2;
	if(XPos < GridSize && YPos >= 0)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Move Bottom Right
	XPos = CurrentX - 2;
	YPos = CurrentY + 1;
	if(XPos >= 0 && YPos < GridSize)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Move Right Bottom
	XPos = CurrentX - 1;
	YPos = CurrentY + 2;
	if(XPos >= 0 && YPos < GridSize)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Move Bottom Left
	XPos = CurrentX - 2;
	YPos = CurrentY - 1;
	if(XPos >= 0 && YPos >= 0)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Move Left Bottom
	XPos = CurrentX - 1;
	YPos = CurrentY - 2;
	if(XPos >= 0 && YPos >= 0)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	return Moves;
}
