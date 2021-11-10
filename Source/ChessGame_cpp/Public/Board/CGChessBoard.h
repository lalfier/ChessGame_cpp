// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pieces/CGChessPiece.h"
#include "History/CGHistoryStruct.h"
#include "CGChessBoard.generated.h"

#ifndef GRID_SIZE
#define GRID_SIZE  8
#endif

class ACGBoardTile;
class ACGGameState;
class UCGSaveGame;

UCLASS()
class CHESSGAME_CPP_API ACGChessBoard : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the board */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BoardMesh;

	ACGGameState* GS;

public:
	ACGChessBoard();

	virtual void Tick(float DeltaSeconds) override;

	/** Size of tile in units */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float TileUnitSize;

	/** Tile offset from board */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float ZOffset;

	/** Tile offset from board */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float DragOffset;

	/** Scale for dead pieces */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float DeadPieceScale;

	/** Offset between dead pieces on board side */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float DeadPieceSpacing;

	UPROPERTY(Category = Pieces, EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<ACGChessPiece>> ChessPiecesToSpawn;

	UPROPERTY(Category = Pieces, EditAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> TeamMaterials;

	FVector MousePosition;

	UFUNCTION(Category = Board, BlueprintCallable)
	void ResetBoard();

	UFUNCTION(Category = Board, BlueprintCallable)
	int32 UndoMove();

	UFUNCTION(Category = Board, BlueprintCallable)
	void LoadGame();

	UFUNCTION(Category = Board, BlueprintCallable)
	void SaveGame();

	bool ContainsValidMove(ACGBoardTile* Tile);

	void HandleTileClicked(ACGBoardTile* Tile);

	void HandleTileReleased(ACGBoardTile* Tile);

private:
	FVector Bounds;

	ACGChessPiece* CurrentPieceDragging;

	ACGBoardTile* ChessTiles[GRID_SIZE][GRID_SIZE];

	ACGChessPiece* ChessPiecesOnBoard[GRID_SIZE][GRID_SIZE];

	TArray<ACGChessPiece*> WhitePiecesDead;

	TArray<ACGChessPiece*> BlackPiecesDead;

	TArray<FIntPoint> AvailableMoves;

	/** Special moves in chess */
	TEnumAsByte<ChessSpecialMove> SpecialMove;

	TArray<TArray<FIntPoint>> MoveList;

	UPROPERTY()
	TArray<FCGHistoryStruct> GameHistory;

	FString SaveGameSlotName;

	UCGSaveGame* SaveGameObject;

	void SetHistoryRow(ACGChessPiece* Piece, FString Action, FIntPoint PrevPosition);

	/** Tile Generation */
	void GenerateTiles(int32 GridSize, float TileSize);

	/** Chess Piece Spawning */
	void SpawnAllChessPieces();
	ACGChessPiece* SpawnChessPiece(ChessPieceType Type, int32 Team);

	/** Chess Piece Positioning */
	void PositionAllChessPieces(int32 GridSize);
	void PositionChessPiece(int32 X, int32 Y, bool bForce = false);

	bool MovePieceTo(ACGChessPiece* PieceDragging, int32 XIndex, int32 YIndex, bool bForce = false);

	void EatChessPiece(ACGChessPiece* TargetPiece, bool bForce = false);

	bool ContainsValidMove(TArray<FIntPoint>& Moves, int32 XIndex, int32 YIndex);

	void ProcessSpecialMove(bool bForce = false);

	void PreventCheckmate();

	void SimulateMoveForPieceDragging(ACGChessPiece* Dragging, TArray<TArray<FIntPoint>>& AllMoves, TArray<FIntPoint>& AllowableMoves, ACGChessPiece* King);

	bool IsCheckmate();

	/** Get Tile center in Units */
	FVector GetTileCenter(int32 X, int32 Y);

	/** Get Tile X and Y index from array */
	FIntPoint GetTileIndex(int32 GridSize, ACGBoardTile* Tile);	

	/** Highlight tiles for available moves */
	void HighlightAvailableTiles();

	/** REmove Highlight tiles */
	void RemoveHighlightTiles();

protected:
	virtual void BeginPlay() override;

public:
	/** Returns DummyRoot sub-object */
	FORCEINLINE class USceneComponent* GetDummyRoot() const
	{
		return DummyRoot;
	}

	/** Returns BoardMesh sub-object */
	FORCEINLINE class UStaticMeshComponent* GetBoardMesh() const
	{
		return BoardMesh;
	}
};
