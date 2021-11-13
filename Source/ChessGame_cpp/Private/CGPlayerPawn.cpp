// Fill out your copyright notice in the Description page of Project Settings.


#include "CGPlayerPawn.h"
#include "Board/CGBoardTile.h"
#include "Board/CGChessBoard.h"
#include "CGPlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

ACGPlayerPawn::ACGPlayerPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;	
}

void ACGPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<ACGPlayerController>(GetController());
}

void ACGPlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(PC && PC->bUpdateTrace)
	{
		// Get mouse position in world and check is over tiles.
		FVector Start, Dir, End;
		PC->DeprojectMousePositionToWorld(Start, Dir);
		End = Start + (Dir * 8000.0f);
		TraceForTile(Start, End, false);

		// Get mouse position in relation to board plane for chess piece dragging.
		if(CurrentTileClick)
		{
			FVector NewMousePos = FMath::LinePlaneIntersection(Start, End, CurrentTileClick->GetOwningGrid()->GetActorLocation(), FVector::UpVector);
			CurrentTileClick->GetOwningGrid()->SetMousePosition(NewMousePos);
		}
	}
}

void ACGPlayerPawn::TraceForTile(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	// Shoot line trace toward mouse
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if(bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}

	if(HitResult.Actor.IsValid())
	{
		// Mouse is over tile
		ACGBoardTile* HitTile = Cast<ACGBoardTile>(HitResult.Actor.Get());
		if(CurrentTileHover != HitTile)
		{
			if(CurrentTileHover)
			{
				// If tile is part of available move highlight green
				if(CurrentTileHover->GetOwningGrid()->ContainsValidMove(CurrentTileHover))
				{
					CurrentTileHover->AvailableHighlight(true);
				}
				else
				{
					CurrentTileHover->HoverHighlight(false);
				}				
			}
			// Highlight hit tile yellow
			if(HitTile)
			{
				HitTile->HoverHighlight(true);
			}
			CurrentTileHover = HitTile;
		}

		// Handle mouse clicked or released over tile
		if(PC->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			if(CurrentTileClick == nullptr && HitTile)
			{
				CurrentTileClick = HitTile;
				CurrentTileClick->HandleClicked();
				CurrentTileClick->GetOwningGrid()->HandleTileClicked(HitTile);
			}
		}
		else
		{
			if(CurrentTileClick)
			{
				bool bHovered = CurrentTileClick == HitTile ? true : false;
				CurrentTileClick->HandleReleased(bHovered);
				CurrentTileClick->GetOwningGrid()->HandleTileReleased(HitTile);
				CurrentTileClick = nullptr;
			}
		}
	}
	else if(CurrentTileHover)
	{
		// Handle highlight if mouse is outside of board
		if(CurrentTileHover->GetOwningGrid()->ContainsValidMove(CurrentTileHover))
		{
			CurrentTileHover->AvailableHighlight(true);
		}
		else
		{
			CurrentTileHover->HoverHighlight(false);
		}
		CurrentTileHover = nullptr;
	}
	else
	{
		// Handle release button if mouse is outside of board
		if(!PC->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			if(CurrentTileClick)
			{
				CurrentTileClick->HandleReleased(false);
				CurrentTileClick->GetOwningGrid()->HandleTileReleased(nullptr);
				CurrentTileClick = nullptr;
			}
		}
	}
}
