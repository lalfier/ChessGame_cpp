// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "History/CGHistoryStruct.h"
#include "CGGameState.generated.h"

class UCGHud;

/**
 * GameState class to specify turns, HUD reference and victory.
 */
UCLASS()
class CHESSGAME_CPP_API ACGGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	/** Variable to hold the widget After Creating it. */
	UPROPERTY(BlueprintReadOnly)
	UCGHud* HUD;

	virtual void BeginPlay() override;
	
public:
	ACGGameState();

	/** Variable to hold HUD class asset (blueprint from CGHud class). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
	TSubclassOf<UCGHud> HUDClass;

	/**
	* Set who is on turn.
	* @param bIsTurn - true for White.
	*/
	void SetIsWhiteTurn(bool bIsTurn);

	/**
	* Send Team that won to HUD.
	* @param Team - 0 for White.
	*/
	void Checkmate(int32 Team);

	/**
	* Send History struct to HUD.
	* @param History - Struct with all necessary data to display on HUD.
	*/
	void DisplayHistory(FCGHistoryStruct History);

	/**
	* Call clear history list on HUD.
	*/
	void ResetHistoryHUD();

private:
	/** Variable to hold who is on turn. */
	bool bWhiteTurn;

public:
	/** Returns is White on turn. */
	FORCEINLINE bool GetIsWhiteTurn() const
	{
		return bWhiteTurn;
	}
};
