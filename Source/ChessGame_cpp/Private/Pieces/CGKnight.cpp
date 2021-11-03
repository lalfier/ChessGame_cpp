// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGKnight.h"

// Sets default values
ACGKnight::ACGKnight()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh;
		FConstructorStatics()
			: ModelMesh(TEXT("/Game/Meshes/Pieces/SM_Knight.SM_Knight"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	// Set static mesh
	PieceMesh->SetStaticMesh(ConstructorStatics.ModelMesh.Get());
}

TArray<FIntPoint> ACGKnight::GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize)
{
	TArray<FIntPoint> Moves;

	// Top Right Movement
	int32 XPos = CurrentX + 2;
	int32 YPos = CurrentY + 1;
	if(XPos < GridSize && YPos < GridSize)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Right Top Movement
	XPos = CurrentX + 1;
	YPos = CurrentY + 2;
	if(XPos < GridSize && YPos < GridSize)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Top Left Movement
	XPos = CurrentX + 2;
	YPos = CurrentY - 1;
	if(XPos < GridSize && YPos >= 0)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Left Top Movement
	XPos = CurrentX + 1;
	YPos = CurrentY - 2;
	if(XPos < GridSize && YPos >= 0)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Bottom Right Movement
	XPos = CurrentX - 2;
	YPos = CurrentY + 1;
	if(XPos >= 0 && YPos < GridSize)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Right Bottom Movement
	XPos = CurrentX - 1;
	YPos = CurrentY + 2;
	if(XPos >= 0 && YPos < GridSize)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Bottom Left Movement
	XPos = CurrentX - 2;
	YPos = CurrentY - 1;
	if(XPos >= 0 && YPos >= 0)
	{
		if(PiecesOnBoard[XPos][YPos] == nullptr || PiecesOnBoard[XPos][YPos]->Team != Team)
		{
			Moves.Add(FIntPoint(XPos, YPos));
		}
	}

	// Left Bottom Movement
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
