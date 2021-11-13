// Fill out your copyright notice in the Description page of Project Settings.


#include "CGGameMode.h"
#include "CGPlayerPawn.h"
#include "CGPlayerController.h"
#include "CGGameState.h"

ACGGameMode::ACGGameMode()
{
	// Set our classes as default for chess game mode
	DefaultPawnClass = ACGPlayerPawn::StaticClass();
	PlayerControllerClass = ACGPlayerController::StaticClass();
	GameStateClass = ACGGameState::StaticClass();
}
