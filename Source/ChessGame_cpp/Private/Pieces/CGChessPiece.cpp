// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGChessPiece.h"

ACGChessPiece::ACGChessPiece()
{
	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh0"));
	PieceMesh->SetRelativeScale3D(FVector(0.665f, 0.665f, 0.665f));
	DefaultScale = PieceMesh->GetRelativeScale3D();
	DesiredScale = DefaultScale;
	PieceMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	PieceMesh->SetCollisionProfileName("Pawn");
	PieceMesh->SetupAttachment(DummyRoot);
}

void ACGChessPiece::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Interpolate piece position and scale
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), DesiredPosition, DeltaSeconds, 10));
	PieceMesh->SetRelativeScale3D(FMath::VInterpTo(PieceMesh->GetRelativeScale3D(), DesiredScale, DeltaSeconds, 10));
}

void ACGChessPiece::SetPiecePosition(FVector Position, bool bForce /*= false*/)
{
	DesiredPosition = Position;
	if(bForce)
	{
		SetActorLocation(DesiredPosition);
	}
}

void ACGChessPiece::SetPieceScale(FVector Scale, bool bForce /*= false*/)
{
	DesiredScale = Scale;
	if(bForce)
	{
		PieceMesh->SetRelativeScale3D(Scale);
	}
}

TArray<FIntPoint> ACGChessPiece::GetAvailableMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], int32 GridSize)
{
	// Override in derived classes.
	TArray<FIntPoint> NoMoves;
	return NoMoves;
}

ChessSpecialMove ACGChessPiece::GetSpecialMoves(ACGChessPiece* PiecesOnBoard[GRID_SIZE][GRID_SIZE], TArray<TArray<FIntPoint>>& MoveList, TArray<FIntPoint>& AvailableMoves)
{
	// Override in derived classes.
	return ChessSpecialMove::Basic;
}
