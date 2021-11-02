// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGBoardTile.generated.h"

UCLASS()
class CHESSGAME_CPP_API ACGBoardTile : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the click-able tile */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TileMesh;

public:
	ACGBoardTile();

	/** Are we currently active? */
	bool bIsActive;

	/** Pointer to white material used on inactive tiles */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	/** Pointer to green material used on allowed tile */
	UPROPERTY()
	class UMaterialInstance* GreenMaterial;

	/** Pointer to red material used on forbidden tile */
	UPROPERTY()
	class UMaterialInstance* RedMaterial;

	/** Pointer to red material used on hover tile */
	UPROPERTY()
	class UMaterialInstance* YellowMaterial;

	/** Grid that owns us */
	UPROPERTY()
	class ACGChessBoard* OwningGrid;

	/** Handle the tile being clicked */
	UFUNCTION()
	void TileClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	void HandleClicked();

	void Highlight(bool bOn);

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const
	{
		return DummyRoot;
	}

	/** Returns TileMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetTileMesh() const
	{
		return TileMesh;
	}
};
