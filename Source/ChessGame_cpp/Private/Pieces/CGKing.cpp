// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGKing.h"

// Sets default values
ACGKing::ACGKing()
{
	// Structure to hold one-time initialization
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh(TEXT("/Game/Meshes/Pieces/SM_King.SM_King"));

	// Set static mesh
	PieceMesh->SetStaticMesh(ModelMesh.Get());
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

ChessSpecialMove ACGKing::GetSpecialMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], TArray<TArray<FIntPoint>>& MoveList, TArray<FIntPoint>& AvailableMoves)
{
	ChessSpecialMove SpecialMove = ChessSpecialMove::Basic;

	int32 StartingX = (Team == 0) ? 0 : 7;
	TArray<FIntPoint>* KingMove = MoveList.FindByPredicate([&](TArray<FIntPoint> m) {return m[0].X == StartingX && m[0].Y == 4;});
	TArray<FIntPoint>* LeftRookMove = MoveList.FindByPredicate([&](TArray<FIntPoint> m) {return m[0].X == StartingX && m[0].Y == 0;});
	TArray<FIntPoint>* RightRookMove = MoveList.FindByPredicate([&](TArray<FIntPoint> m) {return m[0].X == StartingX && m[0].Y == 7;});

	// King has not been moved before
	if(KingMove == nullptr && CurrentY == 4)
	{
		// Left Rook has not been moved before
		if(LeftRookMove == nullptr)
		{
			if(PiecesOnBoard[StartingX][0]->Type == ChessPieceType::Rook)
			{
				if(PiecesOnBoard[StartingX][0]->Team == Team)
				{
					// Check are all tiles empty to the King
					if(PiecesOnBoard[StartingX][1] == nullptr)
					{
						if(PiecesOnBoard[StartingX][2] == nullptr)
						{
							if(PiecesOnBoard[StartingX][3] == nullptr)
							{
								AvailableMoves.Add(FIntPoint(StartingX, 2));
								SpecialMove = ChessSpecialMove::Castling;
							}
						}
					}
				}
			}
		}

		// Right Rook has not been moved before
		if(RightRookMove == nullptr)
		{
			if(PiecesOnBoard[StartingX][7]->Type == ChessPieceType::Rook)
			{
				if(PiecesOnBoard[StartingX][7]->Team == Team)
				{
					// Check are all tiles empty to the King
					if(PiecesOnBoard[StartingX][6] == nullptr)
					{
						if(PiecesOnBoard[StartingX][5] == nullptr)
						{
							AvailableMoves.Add(FIntPoint(StartingX, 6));
							SpecialMove = ChessSpecialMove::Castling;
						}
					}
				}
			}
		}
	}

	return SpecialMove;
}
