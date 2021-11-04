// Fill out your copyright notice in the Description page of Project Settings.


#include "CGPlayerController.h"

ACGPlayerController::ACGPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bUpdateTrace = true;
}
