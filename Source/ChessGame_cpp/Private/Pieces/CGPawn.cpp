// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGPawn.h"

ACGPawn::ACGPawn()
{
	// Structure to hold one-time initialization
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh(TEXT("/Game/Meshes/Pieces/SM_Pawn.SM_Pawn"));

	// Set static mesh
	PieceMesh->SetStaticMesh(ModelMesh.Get());
}

TArray<FIntPoint> ACGPawn::GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize)
{
	// Add move if tile is empty or diagonal if kill move, else check other positions.
	TArray<FIntPoint> Moves;
	int32 Direction = (Team == 0) ? 1 : -1;

	// Move two in direction if tiles are empty and starting position
	if(PiecesOnBoard[CurrentX + Direction][CurrentY] == nullptr)
	{
		if((Team == 0 && CurrentX == 1) || (Team == 1 && CurrentX == 6))
		{
			if(PiecesOnBoard[CurrentX + (Direction * 2)][CurrentY] == nullptr)
			{
				Moves.Add(FIntPoint(CurrentX + (Direction * 2), CurrentY));
			}
		}
	}

	if((CurrentX + Direction) < GridSize && (CurrentX + Direction) >= 0)
	{
		// Move one in direction if tile is empty
		if(PiecesOnBoard[CurrentX + Direction][CurrentY] == nullptr)
		{
			Moves.Add(FIntPoint(CurrentX + Direction, CurrentY));
		}

		// Kill Move (diagonal) Right if there is enemy piece on tile
		if(CurrentY != GridSize - 1)
		{
			if(PiecesOnBoard[CurrentX + Direction][CurrentY + 1] != nullptr && PiecesOnBoard[CurrentX + Direction][CurrentY + 1]->Team != Team)
			{
				Moves.Add(FIntPoint(CurrentX + Direction, CurrentY + 1));
			}
		}
		// Kill Move (diagonal) Left if there is enemy piece on tile
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

ChessSpecialMove ACGPawn::GetSpecialMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], TArray<TArray<FIntPoint>>& MoveList, TArray<FIntPoint>& AvailableMoves)
{
	// Set special move if conditions are met
	int32 Direction = (Team == 0) ? 1 : -1;

	// Promotion (Queening)
	if((Team == 0 && CurrentX == 6) || (Team == 1 && CurrentX == 1))
	{
		return ChessSpecialMove::Promotion;
	}

	// En Passant
	if(MoveList.Num() > 0)
	{
		TArray<FIntPoint> LastMove = MoveList[MoveList.Num() - 1];
		// Check piece on end position of LastMove is a Pawn
		if(PiecesOnBoard[LastMove[1].X][LastMove[1].Y]->Type == ChessPieceType::Pawn)
		{
			// Check if Pawn moved for 2 tiles
			if(FMath::Abs(LastMove[0].X - LastMove[1].X) == 2)
			{
				// Check if the move was from other team
				if(PiecesOnBoard[LastMove[1].X][LastMove[1].Y]->Team != Team)
				{
					// Check if both Pawns are on same X
					if(LastMove[1].X == CurrentX)
					{
						// Check is enemy Pawn left or right from this Pawn
						if(LastMove[1].Y == CurrentY - 1)
						{
							// Add move to list so that tile can be highlighted
							AvailableMoves.Add(FIntPoint(CurrentX + Direction, CurrentY - 1));
							return ChessSpecialMove::EnPassant;
						}
						if(LastMove[1].Y == CurrentY + 1)
						{
							// Add move to list so that tile can be highlighted
							AvailableMoves.Add(FIntPoint(CurrentX + Direction, CurrentY + 1));
							return ChessSpecialMove::EnPassant;
						}
					}
				}
			}
		}
	}

	return ChessSpecialMove::Basic;
}
