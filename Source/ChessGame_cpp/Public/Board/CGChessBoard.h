// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pieces/CGChessPiece.h"
#include "CGChessBoard.generated.h"

class ACGBoardTile;

UCLASS()
class CHESSGAME_CPP_API ACGChessBoard : public AActor
{
	GENERATED_BODY()

	/** Number of tiles along each side of grid */
	const static int32 GRID_SIZE = 8;

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the board */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BoardMesh;

public:
	ACGChessBoard();

	/** Size of tile in units */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float TileUnitSize;

	/** Tile offset from board */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float ZOffset;

	UPROPERTY(Category = Pieces, EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<ACGChessPiece>> ChessPiecesToSpawn;

	UPROPERTY(Category = Pieces, EditAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> TeamMaterials;

private:
	FVector Bounds;

	ACGBoardTile* ChessTiles[GRID_SIZE][GRID_SIZE];

	ACGChessPiece* ChessPiecesOnBoard[GRID_SIZE][GRID_SIZE];

	/** Tile Generation */
	void GenerateTiles(int32 GridSize, float TileSize);

	/** Chess Piece Spawning */
	void SpawnAllChessPieces();
	ACGChessPiece* SpawnChessPiece(ChessPieceType Type, int32 Team);

	/** Chess Piece Positioning */
	void PositionAllChessPieces(int32 GridSize);
	void PositionChessPiece(int32 X, int32 Y, bool bForce = false);

	/** Get Tile center in Units */
	FVector GetTileCenter(int32 X, int32 Y);

	/** Get Tile X and Y index from array */
	FIntPoint GetTileIndex(int32 GridSize, ACGBoardTile* Tile);

protected:
	virtual void BeginPlay() override;

public:
	/** Returns DummyRoot sub-object */
	FORCEINLINE class USceneComponent* GetDummyRoot() const
	{
		return DummyRoot;
	}
};
