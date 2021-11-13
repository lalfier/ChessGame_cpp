// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "History/CGHistoryStruct.h"
#include "CGHud.generated.h"

/**
 * Base HUD class holding blueprint implementable events.
 */
UCLASS()
class CHESSGAME_CPP_API UCGHud : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	* Shows player turn on HUD.
	* @param bIsWhiteOnTurn - Is White player on turn.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = ChessHUD)
	void ShowPlayerTurn(bool bIsWhiteOnTurn);

	/**
	* Show victory screen on game end.
	* @param VictoryTeam - 0 for White, 1 for Black team.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = ChessHUD)
	void ShowVictoryMenu(int32 VictoryTeam);

	/**
	* Add history row widget to HUD.
	* @param History - History struct with all necessary variables.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = ChessHUD)
	void AddHistoryRow(FCGHistoryStruct History);

	/**
	* Clear all History Row widgets from HUD.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = ChessHUD)
	void ClearHistoryList();
};
