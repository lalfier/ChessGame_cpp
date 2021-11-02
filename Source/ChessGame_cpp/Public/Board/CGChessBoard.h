// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGChessBoard.generated.h"

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

public:
	ACGChessBoard();

	/** Number of tiles along each side of grid */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of tiles */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float TileSize;

	/** Tile offset from board */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float ZOffset;

private:
	FVector Bounds;

protected:
	virtual void BeginPlay() override;

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const
	{
		return DummyRoot;
	}
};
