// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pieces/CGChessPiece.h"
#include "History/CGHistoryStruct.h"
#include "CGGameState.h"
#include "CGChessBoard.generated.h"

#ifndef GRID_SIZE
#define GRID_SIZE  8
#endif

class ACGBoardTile;
class UCGSaveGame;

/**
 * Class to specify chess board with tiles, pieces and handling chess logic with simulation.
 */
UCLASS()
class CHESSGAME_CPP_API ACGChessBoard : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ACGChessBoard();

	virtual void Tick(float DeltaSeconds) override;

	/** Size of tile in units. */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float TileUnitSize;

	/** Tile offset from board. */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float ZOffset;

	/** Tile offset from board. */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float DragOffset;

	/** Scale for dead pieces. */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float DeadPieceScale;

	/** Offset between dead pieces on board side. */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float DeadPieceSpacing;

	/** List of chess piece assets. */
	UPROPERTY(Category = Pieces, EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<ACGChessPiece>> ChessPiecesToSpawn;

	/** List of Team materials. */
	UPROPERTY(Category = Pieces, EditAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> TeamMaterials;

	/** Reset Chess board and pieces to starting positions. */
	UFUNCTION(Category = Board, BlueprintCallable)
	void ResetBoard();

	/**
	* Undo last move.
	* @return - Number of history rows removed that must be removed from UI.
	*/
	UFUNCTION(Category = Board, BlueprintCallable)
	int32 UndoMove();

	/** Load game if there is one. */
	UFUNCTION(Category = Board, BlueprintCallable)
	void LoadGame();

	/** Save current game. */
	UFUNCTION(Category = Board, BlueprintCallable)
	void SaveGame();

	/**
	* Check is selected tile a valid move.
	* @param Tile - Reference to selected tile.
	*/
	bool ContainsValidMove(ACGBoardTile* Tile);

	/**
	* Logic for clicking mouse button over tile with chess piece.
	* @param Tile - Reference to tile mouse button is pressed over.
	*/
	void HandleTileClicked(ACGBoardTile* Tile);

	/**
	* Logic for releasing mouse button while dragging chess piece.
	* @param Tile - Reference to tile mouse button is released over.
	*/
	void HandleTileReleased(ACGBoardTile* Tile);

private:
	/** Dummy root component. */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DummyRoot;

	/** StaticMesh component for the board. */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BoardMesh;

	/** Reference to chess game state. */
	ACGGameState* GS;

	/** Chess board bounds. */
	FVector Bounds;

	/** Current mouse position over board plane. */
	FVector MousePosition;

	/** Reference to piece currently dragged by mouse. */
	ACGChessPiece* CurrentPieceDragging;

	/** 2D grid list of chess board tiles. */
	ACGBoardTile* ChessTiles[GRID_SIZE][GRID_SIZE];

	/** 2D grid list of chess pieces on board. */
	ACGChessPiece* ChessPiecesOnBoard[GRID_SIZE][GRID_SIZE];

	/** List of dead white pieces. */
	TArray<ACGChessPiece*> WhitePiecesDead;

	/** List of dead black pieces. */
	TArray<ACGChessPiece*> BlackPiecesDead;

	/** List of Available moves for piece. */
	TArray<FIntPoint> AvailableMoves;

	/** Available special move for piece. */
	TEnumAsByte<ChessSpecialMove> SpecialMove;

	/** List of all moves done by players. */
	TArray<TArray<FIntPoint>> MoveList;

	/** History struct populated after every player move. */
	UPROPERTY()
	TArray<FCGHistoryStruct> GameHistory;

	/** Custom name for save game slot. */
	FString SaveGameSlotName;

	/** Reference for save game object. */
	UCGSaveGame* SaveGameObject;	

	/** Generate and spawn all tiles for board. */
	void GenerateTiles(int32 GridSize, float TileSize);

	/** Spawn all chess pieces for both teams. */
	void SpawnAllChessPieces();

	/** Spawn specific chess piece for team. */
	ACGChessPiece* SpawnChessPiece(ChessPieceType Type, int32 Team);

	/** Position all chess pieces on starting positions for both teams. */
	void PositionAllChessPieces(int32 GridSize);

	/** Position specific chess piece on X, Y grid location. Instant positioning if bForce is true. */
	void PositionChessPiece(int32 X, int32 Y, bool bForce = false);

	/** Move logic for dragged chess piece to X, Y grid location if there is valid move option. Instant move if bForce is true. */
	bool MovePieceTo(ACGChessPiece* PieceDragging, int32 XIndex, int32 YIndex, bool bForce = false);

	/** Remove and scale eaten chess piece. Instant move and scale if bForce is true. */
	void EatChessPiece(ACGChessPiece* TargetPiece, bool bForce = false);

	/** Check is selected move valid. */
	bool ContainsValidMove(TArray<FIntPoint>& Moves, int32 XIndex, int32 YIndex);

	/** Handle chess pieces movement if dragged piece made a special move. Instant move if bForce is true. */
	void ProcessSpecialMove(bool bForce = false);

	/** Remove all available moves that can put king in checkmate position. */
	void PreventCheckmate();

	/** Simulate all moves that dragged piece can take, including special moves and remove those that put king in check position. */
	void SimulateMoveForPieceDragging(ACGChessPiece* Dragging, TArray<TArray<FIntPoint>>& AllMoves, TArray<FIntPoint>& AllowableMoves, ACGChessPiece* King);

	/** Return victory condition if player has no available moves. */
	ChessGameOver GameOverConditionMet();

	/** Prepare history row so it can be shown on UI. */
	void SetHistoryRow(ACGChessPiece* Piece, FString Action, FIntPoint PrevPosition);

	/** Get Tile center in Units. */
	FVector GetTileCenter(int32 X, int32 Y);

	/** Get Tile X and Y index from array. */
	FIntPoint GetTileIndex(int32 GridSize, ACGBoardTile* Tile);	

	/** Highlight tiles for available moves. */
	void HighlightAvailableTiles();

	/** Remove Highlight from tiles. */
	void RemoveHighlightTiles();

public:
	/** Returns DummyRoot sub-object. */
	FORCEINLINE USceneComponent* GetDummyRoot() const
	{
		return DummyRoot;
	}

	/** Returns BoardMesh sub-object. */
	FORCEINLINE UStaticMeshComponent* GetBoardMesh() const
	{
		return BoardMesh;
	}

	/** Sets mouse position. */
	FORCEINLINE void SetMousePosition(FVector Position)
	{
		MousePosition = Position;
	}
};
