// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CGPlayerPawn.generated.h"

class ACGPlayerController;

UCLASS(config=Game)
class CHESSGAME_CPP_API ACGPlayerPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	void TraceForTile(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ACGBoardTile* CurrentTileHover;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ACGBoardTile* CurrentTileClick;

	ACGPlayerController* PC;
};
