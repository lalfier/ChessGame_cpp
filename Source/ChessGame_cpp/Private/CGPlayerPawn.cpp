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

	if(PC)
	{
		FVector Start, Dir, End;
		PC->DeprojectMousePositionToWorld(Start, Dir);
		End = Start + (Dir * 8000.0f);
		TraceForTile(Start, End, false);

		if(CurrentTileClick)
		{
			CurrentTileClick->OwningGrid->MousePosition = FMath::LinePlaneIntersection(Start, End, CurrentTileClick->OwningGrid->GetActorLocation(), FVector::UpVector);
		}
	}
}

void ACGPlayerPawn::TraceForTile(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if(bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}

	if(HitResult.Actor.IsValid())
	{
		ACGBoardTile* HitBlock = Cast<ACGBoardTile>(HitResult.Actor.Get());
		if(CurrentTileHover != HitBlock)
		{
			if(CurrentTileHover)
			{
				CurrentTileHover->Highlight(false);
			}
			if(HitBlock)
			{
				HitBlock->Highlight(true);
			}
			CurrentTileHover = HitBlock;
		}

		if(PC->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			if(CurrentTileClick == nullptr && HitBlock)
			{
				CurrentTileClick = HitBlock;
				CurrentTileClick->HandleClicked();
				CurrentTileClick->OwningGrid->HandleTileClicked(HitBlock);
			}
		}
		else
		{
			if(CurrentTileClick)
			{
				bool bHovered = CurrentTileClick == HitBlock ? true : false;
				CurrentTileClick->HandleReleased(bHovered);
				CurrentTileClick->OwningGrid->HandleTileReleased(HitBlock);
				CurrentTileClick = nullptr;
			}
		}
	}
	else if(CurrentTileHover)
	{
		CurrentTileHover->Highlight(false);
		CurrentTileHover = nullptr;
	}
	else
	{
		if(!PC->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			if(CurrentTileClick)
			{
				CurrentTileClick->HandleReleased(false);
				CurrentTileClick->OwningGrid->HandleTileReleased(nullptr);
				CurrentTileClick = nullptr;
			}
		}
	}
}
