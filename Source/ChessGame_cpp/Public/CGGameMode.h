// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGGameMode.generated.h"

/** GameMode class to specify pawn and player-controller */
UCLASS()
class CHESSGAME_CPP_API ACGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACGGameMode();
};
