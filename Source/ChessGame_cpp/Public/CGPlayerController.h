// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CGPlayerController.generated.h"

/** PlayerController class used to enable cursor */
UCLASS()
class CHESSGAME_CPP_API ACGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACGPlayerController();
};
