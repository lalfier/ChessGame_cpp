// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGBoardTile.generated.h"

class ACGChessBoard;

/**
 * Class to specify chess tile and handle highlight.
 */
UCLASS()
class CHESSGAME_CPP_API ACGBoardTile : public AActor
{
	GENERATED_BODY()

private:
	/** Dummy root component. */
	UPROPERTY(Category = Tile, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the click-able tile. */
	UPROPERTY(Category = Tile, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TileMesh;

	/** Pointer to white material used on inactive tiles. */
	UPROPERTY(Category = Tile, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMaterial* BaseMaterial;

	/** Pointer to green material used on allowed tile. */
	UPROPERTY(Category = Tile, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* GreenMaterial;

	/** Pointer to red material used on currently clicked tile. */
	UPROPERTY(Category = Tile, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* RedMaterial;

	/** Pointer to red material used on hover tile. */
	UPROPERTY(Category = Tile, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* YellowMaterial;

	/** Grid that owns this tile. */
	ACGChessBoard* OwningGrid;

	/** Is tile currently active? */
	bool bIsActive;

public:
	ACGBoardTile();

	/** Change material for clicked tile to red. */
	void HandleClicked();

	/**
	* Change material to base when mouse button is released.
	* @param bIsHovered - If tile is hovered change material to yellow.
	*/
	void HandleReleased(bool bIsHovered);

	/**
	* Change material to yellow when tile is hovered.
	* @param bOn - true if tile is hovered.
	*/
	void HoverHighlight(bool bOn);

	/**
	* Change material to green when tile is available to move.
	* @param bOn - true if tile is available to move.
	*/
	void AvailableHighlight(bool bOn);

public:
	/** Returns DummyRoot sub-object. */
	FORCEINLINE USceneComponent* GetDummyRoot() const
	{
		return DummyRoot;
	}

	/** Returns TileMesh sub-object. */
	FORCEINLINE UStaticMeshComponent* GetTileMesh() const
	{
		return TileMesh;
	}

	/** Returns Owning Grid. */
	FORCEINLINE ACGChessBoard* GetOwningGrid() const
	{
		return OwningGrid;
	}

	/** Sets Owning Grid. */
	FORCEINLINE void SetOwningGrid(ACGChessBoard* BoardGrid)
	{
		OwningGrid = BoardGrid;
	}
};
