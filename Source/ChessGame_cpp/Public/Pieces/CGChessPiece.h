// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGChessPiece.generated.h"

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

UCLASS()
class CHESSGAME_CPP_API ACGChessPiece : public AActor
{
	GENERATED_BODY()
	
public:		
	ACGChessPiece();

	virtual void Tick(float DeltaSeconds) override;

	/** StaticMesh component for the chess piece */
	UPROPERTY(Category = Piece, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PieceMesh;

	/** 0 - White, 1 - Black */
	UPROPERTY(Category = Piece, EditAnywhere, BlueprintReadOnly)
	int32 Team;

	/** Type of piece */
	UPROPERTY(Category = Piece, EditAnywhere, BlueprintReadOnly)
	TEnumAsByte <ChessPieceType> Type;

	int32 CurrentX;

	int32 CurrentY;

	virtual void SetPiecePosition(FVector Position, bool bForce = false);

	virtual void SetPieceScale(FVector Scale, bool bForce = false);

protected:
	/** Dummy root component */
	UPROPERTY(Category = Piece, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

private:
	FVector DesiredPosition;

	FVector DefaultScale;
	FVector DesiredScale;
};
