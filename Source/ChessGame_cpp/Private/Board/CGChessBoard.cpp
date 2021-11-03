// Fill out your copyright notice in the Description page of Project Settings.


#include "Board/CGChessBoard.h"
#include "Board/CGBoardTile.h"
#include "Pieces/CGPawn.h"
#include "Pieces/CGRook.h"
#include "Pieces/CGKnight.h"
#include "Pieces/CGBishop.h"
#include "Pieces/CGQueen.h"
#include "Pieces/CGKing.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

// Sets default values
ACGChessBoard::ACGChessBoard()
{
	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> WhiteMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BlackMaterial;
		FConstructorStatics()
			: ModelMesh(TEXT("/Game/Meshes/SM_Board.SM_Board"))
			, WhiteMaterial(TEXT("/Game/Meshes/Pieces/M_White.M_White"))
			, BlackMaterial(TEXT("/Game/Meshes/Pieces/M_Black.M_Black"))
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
	TileUnitSize = 100.f;
	ZOffset = 1.f;
	DragOffset = 100.f;
	DeadPieceScale = 0.3f;
	DeadPieceSpacing = 40.f;

	// Save a pointer to the materials
	TeamMaterials.Add(ConstructorStatics.WhiteMaterial.Get());
	TeamMaterials.Add(ConstructorStatics.BlackMaterial.Get());

	// Populate pieces array
	ChessPiecesToSpawn.Add(ACGPawn::StaticClass());
	ChessPiecesToSpawn.Add(ACGRook::StaticClass());
	ChessPiecesToSpawn.Add(ACGKnight::StaticClass());
	ChessPiecesToSpawn.Add(ACGBishop::StaticClass());
	ChessPiecesToSpawn.Add(ACGQueen::StaticClass());
	ChessPiecesToSpawn.Add(ACGKing::StaticClass());
}

void ACGChessBoard::BeginPlay()
{
	Super::BeginPlay();

	GenerateTiles(GRID_SIZE, TileUnitSize);

	SpawnAllChessPieces();

	PositionAllChessPieces(GRID_SIZE);
}

void ACGChessBoard::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(CurrentPieceDragging)
	{
		MousePosition.Z += DragOffset;
		CurrentPieceDragging->SetPiecePosition(MousePosition);
	}
}

void ACGChessBoard::GenerateTiles(int32 GridSize, float TileSize)
{
	// Number of tiles
	const int32 NumTiles = GridSize * GridSize;

	// Calculate board bounds and Z offset
	Bounds = FVector((GridSize/2) * TileSize - TileSize/2, (GridSize/2) * TileSize - TileSize/2, 0.f) + GetActorLocation();
	Bounds.Z = 0.f;
	ZOffset += GetActorLocation().Z;

	// Loop to spawn each tile
	for(int32 TileIndex = 0; TileIndex < NumTiles; TileIndex++)
	{
		const float XOffset = (TileIndex / GridSize) * TileSize; // Divide by dimension
		const float YOffset = (TileIndex % GridSize) * TileSize; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector TileLocation = FVector(XOffset, YOffset, ZOffset) - Bounds;

		// Spawn a tile
		ACGBoardTile* NewTile = GetWorld()->SpawnActor<ACGBoardTile>(TileLocation, FRotator(0, 0, 0));

		// Tell the tile about its scale, label and owner
		if(NewTile != nullptr)
		{
			NewTile->SetActorScale3D(FVector(TileSize/100, TileSize/100, TileSize/100));
			int32 XIndex = (int32)(XOffset/TileSize);
			int32 YIndex = (int32)(YOffset/TileSize);			
			NewTile->SetActorLabel(FString::Printf(TEXT("X:%d, Y:%d"), XIndex, YIndex));
			NewTile->OwningGrid = this;
			NewTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			ChessTiles[XIndex][YIndex] = NewTile;
		}
	}
}

void ACGChessBoard::SpawnAllChessPieces()
{
	int32 WhiteTeam = 0;
	int32 BlackTeam = 1;

	// White Team
	ChessPiecesOnBoard[0][0] = SpawnChessPiece(ChessPieceType::Rook, WhiteTeam);
	ChessPiecesOnBoard[0][1] = SpawnChessPiece(ChessPieceType::Knight, WhiteTeam);
	ChessPiecesOnBoard[0][2] = SpawnChessPiece(ChessPieceType::Bishop, WhiteTeam);
	ChessPiecesOnBoard[0][3] = SpawnChessPiece(ChessPieceType::Queen, WhiteTeam);
	ChessPiecesOnBoard[0][4] = SpawnChessPiece(ChessPieceType::King, WhiteTeam);
	ChessPiecesOnBoard[0][5] = SpawnChessPiece(ChessPieceType::Bishop, WhiteTeam);
	ChessPiecesOnBoard[0][6] = SpawnChessPiece(ChessPieceType::Knight, WhiteTeam);
	ChessPiecesOnBoard[0][7] = SpawnChessPiece(ChessPieceType::Rook, WhiteTeam);
	for(int32 i = 0; i < GRID_SIZE; i++)
	{
		ChessPiecesOnBoard[1][i] = SpawnChessPiece(ChessPieceType::Pawn, WhiteTeam);
	}

	// Black Team
	ChessPiecesOnBoard[7][0] = SpawnChessPiece(ChessPieceType::Rook, BlackTeam);
	ChessPiecesOnBoard[7][1] = SpawnChessPiece(ChessPieceType::Knight, BlackTeam);
	ChessPiecesOnBoard[7][2] = SpawnChessPiece(ChessPieceType::Bishop, BlackTeam);
	ChessPiecesOnBoard[7][3] = SpawnChessPiece(ChessPieceType::Queen, BlackTeam);
	ChessPiecesOnBoard[7][4] = SpawnChessPiece(ChessPieceType::King, BlackTeam);
	ChessPiecesOnBoard[7][5] = SpawnChessPiece(ChessPieceType::Bishop, BlackTeam);
	ChessPiecesOnBoard[7][6] = SpawnChessPiece(ChessPieceType::Knight, BlackTeam);
	ChessPiecesOnBoard[7][7] = SpawnChessPiece(ChessPieceType::Rook, BlackTeam);
	for(int32 i = 0; i < GRID_SIZE; i++)
	{
		ChessPiecesOnBoard[6][i] = SpawnChessPiece(ChessPieceType::Pawn, BlackTeam);
	}
}

ACGChessPiece* ACGChessBoard::SpawnChessPiece(ChessPieceType Type, int32 Team)
{
	ACGChessPiece* CP = GetWorld()->SpawnActor<ACGChessPiece>(ChessPiecesToSpawn[Type-1], GetActorLocation(), FRotator(0, Team == 0 ? -90 : 90, 0));
	CP->Type = Type;
	CP->Team = Team;
	CP->PieceMesh->SetMaterial(0, TeamMaterials[Team]);
	CP->SetFolderPath("ChessPieces");

	return CP;
}

void ACGChessBoard::PositionAllChessPieces(int32 GridSize)
{
	// Number of tiles
	const int32 NumTiles = GridSize * GridSize;

	// Loop to spawn each tile
	for(int32 TileIndex = 0; TileIndex < NumTiles; TileIndex++)
	{
		const int32 XPos = (TileIndex / GridSize); // Divide by dimension
		const int32 YPos = (TileIndex % GridSize); // Modulo gives remainder

		if(ChessPiecesOnBoard[XPos][YPos] != nullptr)
		{
			PositionChessPiece(XPos, YPos, true);
		}
	}
}

void ACGChessBoard::PositionChessPiece(int32 X, int32 Y, bool bForce /*= false*/)
{
	ChessPiecesOnBoard[X][Y]->CurrentX = X;
	ChessPiecesOnBoard[X][Y]->CurrentY = Y;
	ChessPiecesOnBoard[X][Y]->SetPiecePosition(GetTileCenter(X, Y), bForce);
}

bool ACGChessBoard::MovePieceTo(ACGChessPiece* PieceDragging, int32 XIndex, int32 YIndex)
{
	FIntPoint PreviousIndexPos = FIntPoint(PieceDragging->CurrentX, PieceDragging->CurrentY);

	// Check is there another piece on the target position
	if(ChessPiecesOnBoard[XIndex][YIndex])
	{
		ACGChessPiece* TargetPiece = ChessPiecesOnBoard[XIndex][YIndex];
		// Same team
		if(PieceDragging->Team == TargetPiece->Team)
		{
			return false;
		}

		// Enemy team
		if(TargetPiece->Team == 0)
		{
			WhitePiecesDead.Add(TargetPiece);
			TargetPiece->SetPiecePosition(FVector(-1 * TileUnitSize, 8 * TileUnitSize, ZOffset + 15) - Bounds + (FVector::ForwardVector * DeadPieceSpacing * (WhitePiecesDead.Num() - 1)));
		}
		else
		{
			BlackPiecesDead.Add(TargetPiece);
			TargetPiece->SetPiecePosition(FVector(8 * TileUnitSize, -1 * TileUnitSize, ZOffset + 15) - Bounds + (FVector::BackwardVector * DeadPieceSpacing * (BlackPiecesDead.Num() - 1)));
		}
		TargetPiece->SetPieceScale(FVector::OneVector * DeadPieceScale);
	}

	ChessPiecesOnBoard[XIndex][YIndex] = PieceDragging;
	ChessPiecesOnBoard[PreviousIndexPos.X][PreviousIndexPos.Y] = nullptr;

	PositionChessPiece(XIndex, YIndex);

	return true;
}

FVector ACGChessBoard::GetTileCenter(int32 X, int32 Y)
{
	return FVector(X * TileUnitSize, Y * TileUnitSize, ZOffset) - Bounds;
}

FIntPoint ACGChessBoard::GetTileIndex(int32 GridSize, ACGBoardTile* Tile)
{
	// Number of tiles
	const int32 NumTiles = GridSize * GridSize;

	// Loop to spawn each tile
	for(int32 TileIndex = 0; TileIndex < NumTiles; TileIndex++)
	{
		const int32 XPos = (TileIndex / GridSize); // Divide by dimension
		const int32 YPos = (TileIndex % GridSize); // Modulo gives remainder

		if(ChessTiles[XPos][YPos] == Tile)
		{
			return FIntPoint(XPos, YPos);
		}
	}

	return FIntPoint(-1, -1);
}

void ACGChessBoard::HandleTileClicked(ACGBoardTile* Tile)
{
	FIntPoint TileIndexPos = GetTileIndex(GRID_SIZE, Tile);

	if(ChessPiecesOnBoard[TileIndexPos.X][TileIndexPos.Y])
	{
		// TODO - Is our turn?
		if(true)
		{
			CurrentPieceDragging = ChessPiecesOnBoard[TileIndexPos.X][TileIndexPos.Y];
		}
	}
}

void ACGChessBoard::HandleTileReleased(ACGBoardTile* Tile)
{
	if(CurrentPieceDragging)
	{
		FIntPoint PreviousIndexPos = FIntPoint(CurrentPieceDragging->CurrentX, CurrentPieceDragging->CurrentY);

		if(Tile)
		{
			FIntPoint TileIndexPos = GetTileIndex(GRID_SIZE, Tile);
			bool bValidMove = MovePieceTo(CurrentPieceDragging, TileIndexPos.X, TileIndexPos.Y);
			if(!bValidMove)
			{
				CurrentPieceDragging->SetPiecePosition(GetTileCenter(PreviousIndexPos.X, PreviousIndexPos.Y));				
			}
			CurrentPieceDragging = nullptr;
		}
		else
		{
			CurrentPieceDragging->SetPiecePosition(GetTileCenter(PreviousIndexPos.X, PreviousIndexPos.Y));
			CurrentPieceDragging = nullptr;
		}
	}
}
