// Fill out your copyright notice in the Description page of Project Settings.


#include "Board/CGChessBoard.h"
#include "Board/CGBoardTile.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

// Sets default values
ACGChessBoard::ACGChessBoard()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh;
		FConstructorStatics()
			: ModelMesh(TEXT("/Game/Meshes/SM_Board.SM_Board"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardMesh0"));
	BoardMesh->SetStaticMesh(ConstructorStatics.ModelMesh.Get());
	BoardMesh->SetRelativeScale3D(FVector(0.665f, 0.665f, 0.665f));
	BoardMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	BoardMesh->SetupAttachment(DummyRoot);

	// Set defaults
	Size = 8;
	TileSize = 100.f;
	ZOffset = 1.f;
}

void ACGChessBoard::BeginPlay()
{
	Super::BeginPlay();

	// Number of tiles
	const int32 NumTiles = Size * Size;

	// Calculate board bounds and Z offset
	Bounds = FVector((Size / 2) * TileSize - TileSize/2, (Size / 2) * TileSize - TileSize/2, 0.f) + GetActorLocation();
	Bounds.Z = 0.f;
	ZOffset += GetActorLocation().Z;

	// Loop to spawn each tile
	for(int32 TileIndex = 0; TileIndex < NumTiles; TileIndex++)
	{
		const float XOffset = (TileIndex / Size) * TileSize; // Divide by dimension
		const float YOffset = (TileIndex % Size) * TileSize; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector TileLocation = FVector(XOffset, YOffset, ZOffset) - Bounds;

		// Spawn a tile
		ACGBoardTile* NewTile = GetWorld()->SpawnActor<ACGBoardTile>(TileLocation, FRotator(0, 0, 0));

		// Tell the tile about its scale, label and owner
		if(NewTile != nullptr)
		{
			NewTile->SetActorScale3D(FVector(TileSize/100, TileSize/100, TileSize/100));
			NewTile->SetActorLabel(FString::Printf(TEXT("X:%d, Y:%d"), (int)(XOffset/TileSize), (int)(YOffset/TileSize)));
			NewTile->OwningGrid = this;
			NewTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}
