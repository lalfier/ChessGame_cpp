// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save/CGSaveStruct.h"
#include "CGSaveGame.generated.h"

/**
 * Save all moves in a list
 */
UCLASS()
class CHESSGAME_CPP_API UCGSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FCGSaveStruct> SavedMoveList;
};
