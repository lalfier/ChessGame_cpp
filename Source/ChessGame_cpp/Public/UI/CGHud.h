// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGHud.generated.h"

/** HUD class */
UCLASS()
class CHESSGAME_CPP_API UCGHud : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = ChessHUD)
	void ShowPlayerTurn(bool bIsWhiteOnTurn);

	UFUNCTION(BlueprintImplementableEvent, Category = ChessHUD)
	void ShowVictoryMenu(int32 VictoryTeam);
};
