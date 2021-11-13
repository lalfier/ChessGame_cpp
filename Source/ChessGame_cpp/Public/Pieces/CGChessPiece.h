// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGChessPiece.generated.h"

#ifndef GRID_SIZE
#define GRID_SIZE  8
#endif

/** Types for chess piece. */
UENUM()
enum ChessPieceType
{
	None = 0,
	Pawn = 1,
	Rook = 2,
	Knight = 3,
	Bishop = 4,
	Queen = 5,
	King = 6
};

/** Special moves for chess piece. */
UENUM()
enum ChessSpecialMove
{
	Basic = 0,
	Promotion = 1,
	Castling = 2,
	EnPassant = 3
};

/**
 * Base class to specify chess piece and calculate available/special moves.
 */
UCLASS()
class CHESSGAME_CPP_API ACGChessPiece : public AActor
{
	GENERATED_BODY()
	
public:		
	ACGChessPiece();	

	/** 0 - White, 1 - Black. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Piece)
	int32 Team;

	/** Type of chess piece (Pawn, Rook, ...). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Piece)
	TEnumAsByte<ChessPieceType> Type;

	/** X position on grid. */
	int32 CurrentX;

	/** Y position on grid. */
	int32 CurrentY;

	virtual void Tick(float DeltaSeconds) override;

	/**
	* Move piece to desired position.
	* @param Position - Position in World Units.
	* @param bForce - If true, move piece instantly.
	*/
	virtual void SetPiecePosition(FVector Position, bool bForce = false);

	/**
	* Set piece to desired scale.
	* @param Scale - Desired scale.
	* @param bForce - If true, scale piece instantly.
	*/
	virtual void SetPieceScale(FVector Scale, bool bForce = false);

	/**
	* Find all available moves for this chess piece.
	* @param PiecesOnBoard - Reference to current board state with all chess piece positions.
	* @param GridSize - Tiles per X or Y.
	* @return - Array of available moves for this piece (X and Y grid position).
	*/
	virtual TArray<FIntPoint> GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize);

	/**
	* Find all special moves for this chess piece.
	* @param PiecesOnBoard - Reference to current board state with all chess piece positions.
	* @param MoveList - Reference to list of all moves done by players.
	* @param AvailableMoves - Reference to list of all available moves for this piece (we are adding to this list if there are special moves).
	* @return - Type of special chess move for this piece. Return Basic if there is no special moves.
	*/
	virtual ChessSpecialMove GetSpecialMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], TArray<TArray<FIntPoint>>& MoveList, TArray<FIntPoint>& AvailableMoves);

protected:
	/** Dummy root component. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Piece, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DummyRoot;

	/** StaticMesh component for the chess piece. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Piece, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PieceMesh;

private:
	/** Position to set. */
	FVector DesiredPosition;

	/** Starting scale (default). */
	FVector DefaultScale;

	/** Scale to set. */
	FVector DesiredScale;

public:
	/** Returns DummyRoot sub-object. */
	FORCEINLINE USceneComponent* GetDummyRoot() const
	{
		return DummyRoot;
	}

	/** Returns PieceMesh sub-object. */
	FORCEINLINE UStaticMeshComponent* GetPieceMesh() const
	{
		return PieceMesh;
	}

	/** Returns Default object scale. */
	FORCEINLINE FVector GetDefaultScale() const
	{
		return DefaultScale;
	}
};
