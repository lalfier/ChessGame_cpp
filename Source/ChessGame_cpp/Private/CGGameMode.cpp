// Fill out your copyright notice in the Description page of Project Settings.


#include "CGGameMode.h"
#include "CGPlayerPawn.h"
#include "CGPlayerController.h"

ACGGameMode::ACGGameMode()
{
	// no pawn by default
	DefaultPawnClass = ACGPlayerPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ACGPlayerController::StaticClass();
}
