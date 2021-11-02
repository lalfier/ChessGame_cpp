// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGChessPiece.h"

// Sets default values
ACGChessPiece::ACGChessPiece()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh0"));
	PieceMesh->SetRelativeScale3D(FVector(0.665f, 0.665f, 0.665f));
	PieceMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	PieceMesh->SetCollisionProfileName("Pawn");
	PieceMesh->SetupAttachment(DummyRoot);
}
