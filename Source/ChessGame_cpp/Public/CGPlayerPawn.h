// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CGPlayerPawn.generated.h"

class ACGPlayerController;
class ACGBoardTile;

/**
 * Class for handling mouse input with tiles.
 */
UCLASS(config=Game)
class CHESSGAME_CPP_API ACGPlayerPawn : public APawn
{
	GENERATED_UCLASS_BODY()

protected:
	/** Hovered tile reference. */
	UPROPERTY(BlueprintReadOnly)
	ACGBoardTile* CurrentTileHover;

	/** Clicked tile reference. */
	UPROPERTY(BlueprintReadOnly)
	ACGBoardTile* CurrentTileClick;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	/** Player controller reference. */
	ACGPlayerController* PC;

	/** Checks is mouse over tile and is mouse button clicked. */
	void TraceForTile(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);
};
