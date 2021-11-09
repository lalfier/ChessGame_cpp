// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGChessPiece.generated.h"

#ifndef GRID_SIZE
#define GRID_SIZE  8
#endif

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

UENUM()
enum ChessSpecialMove
{
	Basic = 0,
	Promotion = 1,
	Castling = 2,
	EnPassant = 3
};

UCLASS()
class CHESSGAME_CPP_API ACGChessPiece : public AActor
{
	GENERATED_BODY()
	
public:		
	ACGChessPiece();

	virtual void Tick(float DeltaSeconds) override;

	/** 0 - White, 1 - Black */
	UPROPERTY(Category = Piece, EditAnywhere, BlueprintReadOnly)
	int32 Team;

	/** Type of piece */
	UPROPERTY(Category = Piece, EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ChessPieceType> Type;

	int32 CurrentX;

	int32 CurrentY;

	virtual void SetPiecePosition(FVector Position, bool bForce = false);

	virtual void SetPieceScale(FVector Scale, bool bForce = false);

	virtual TArray<FIntPoint> GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize);

	virtual ChessSpecialMove GetSpecialMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], TArray<TArray<FIntPoint>>& MoveList, TArray<FIntPoint>& AvailableMoves);

protected:
	/** Dummy root component */
	UPROPERTY(Category = Piece, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the chess piece */
	UPROPERTY(Category = Piece, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PieceMesh;

private:
	FVector DesiredPosition;

	FVector DefaultScale;
	FVector DesiredScale;

public:
	/** Returns DummyRoot sub-object */
	FORCEINLINE class USceneComponent* GetDummyRoot() const
	{
		return DummyRoot;
	}

	/** Returns PieceMesh sub-object */
	FORCEINLINE class UStaticMeshComponent* GetPieceMesh() const
	{
		return PieceMesh;
	}

	/** Returns Default object scale */
	FORCEINLINE FVector GetDefaultScale() const
	{
		return DefaultScale;
	}
};
