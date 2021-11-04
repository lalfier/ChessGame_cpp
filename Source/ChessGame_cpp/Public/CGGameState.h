// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CGGameState.generated.h"

class UCGHud;

/** GameState class to specify turns */
UCLASS()
class CHESSGAME_CPP_API ACGGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	ACGGameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCGHud> HUDClass;

	void SetIsWhiteTurn(bool bIsTurn);

	bool GetIsWhiteTurn();

	void CheckMate(int32 Team);

private:
	// Variable to hold the widget After Creating it.
	UCGHud* HUD;

	bool bWhiteTurn;
};
