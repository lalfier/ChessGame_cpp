// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save/CGSaveStruct.h"
#include "CGSaveGame.generated.h"

/**
 * Save game object with list of move structs.
 */
UCLASS()
class CHESSGAME_CPP_API UCGSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	/** Every move in list has previous and current position. */
	UPROPERTY()
	TArray<FCGSaveStruct> SavedMoveList;
};
