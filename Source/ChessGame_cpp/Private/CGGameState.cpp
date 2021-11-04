// Fill out your copyright notice in the Description page of Project Settings.


#include "CGGameState.h"
#include "UI/CGHud.h"

ACGGameState::ACGGameState()
{
	// Structure to hold one-time initialization
	static ConstructorHelpers::FClassFinder<UCGHud> BPHUDAsset(TEXT("/Game/Blueprints/UI/WBP_HUD"));

	// Set class
	HUDClass = BPHUDAsset.Class;
}

void ACGGameState::BeginPlay()
{
	Super::BeginPlay();
	
	// Create the widget and store it.
	HUD = CreateWidget<UCGHud>(GetWorld(), HUDClass);
	if(HUD)
	{
		// Add it to the view port
		HUD->AddToViewport();
	}

	SetIsWhiteTurn(true);
}

void ACGGameState::SetIsWhiteTurn(bool bIsTurn)
{
	bWhiteTurn = bIsTurn;
	if(HUD)
	{
		HUD->ShowPlayerTurn(bWhiteTurn);
	}
}

bool ACGGameState::GetIsWhiteTurn()
{
	return bWhiteTurn;
}

void ACGGameState::CheckMate(int32 Team)
{
	if(HUD)
	{
		HUD->ShowVictoryMenu(Team);
	}
}

void ACGGameState::DisplayHistory(FCGHistoryStruct History)
{
	if(HUD)
	{
		HUD->AddHistoryRow(History);
	}
}
