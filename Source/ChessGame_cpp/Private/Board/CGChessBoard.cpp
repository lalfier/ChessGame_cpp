// Fill out your copyright notice in the Description page of Project Settings.


#include "Board/CGChessBoard.h"
#include "Board/CGBoardTile.h"
#include "Pieces/CGPawn.h"
#include "Pieces/CGRook.h"
#include "Pieces/CGKnight.h"
#include "Pieces/CGBishop.h"
#include "Pieces/CGQueen.h"
#include "Pieces/CGKing.h"
#include "Save/CGSaveGame.h"
#include "Kismet/GameplayStatics.h"
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

	// Set save game name
	SaveGameSlotName = "CG_SaveGame";
}

void ACGChessBoard::BeginPlay()
{
	Super::BeginPlay();

	GS = GetWorld()->GetGameState<ACGGameState>();

	// Prepare all for chess game start
	GenerateTiles(GRID_SIZE, TileUnitSize);
	SpawnAllChessPieces();
	PositionAllChessPieces(GRID_SIZE);
}

void ACGChessBoard::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Move dragging chess piece to mouse position
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
#if WITH_EDITOR
			NewTile->SetActorLabel(FString::Printf(TEXT("X:%d, Y:%d"), XIndex, YIndex));
#endif
			NewTile->SetOwningGrid(this);
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
	// Spawn chess piece, rotate it and set material depending on team
	ACGChessPiece* CP = GetWorld()->SpawnActor<ACGChessPiece>(ChessPiecesToSpawn[Type-1], GetActorLocation(), FRotator(0, Team == 0 ? -90 : 90, 0));
	CP->Type = Type;
	CP->Team = Team;
	CP->GetPieceMesh()->SetMaterial(0, TeamMaterials[Team]);
#if WITH_EDITOR
	CP->SetFolderPath("ChessPieces");
#endif

	return CP;
}

void ACGChessBoard::PositionAllChessPieces(int32 GridSize)
{
	// Number of tiles
	const int32 NumTiles = GridSize * GridSize;

	// Loop trough tiles and position spawned chess pieces
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

bool ACGChessBoard::MovePieceTo(ACGChessPiece* PieceDragging, int32 XIndex, int32 YIndex, bool bForce /*= false*/)
{
	// If there is no valid moves return
	if(!ContainsValidMove(AvailableMoves, XIndex, YIndex))
	{
		return false;
	}

	FIntPoint PreviousIndexPos = FIntPoint(PieceDragging->CurrentX, PieceDragging->CurrentY);

	// Check is there another piece on the target position
	if(ChessPiecesOnBoard[XIndex][YIndex])
	{
		ACGChessPiece* TargetPiece = ChessPiecesOnBoard[XIndex][YIndex];
		// If same team can not move there, return
		if(PieceDragging->Team == TargetPiece->Team)
		{
			return false;
		}

		// If enemy team, eat
		EatChessPiece(TargetPiece, bForce);

		SetHistoryRow(TargetPiece, "Is Eaten", FIntPoint(XIndex, YIndex));
	}

	ChessPiecesOnBoard[XIndex][YIndex] = PieceDragging;
	ChessPiecesOnBoard[PreviousIndexPos.X][PreviousIndexPos.Y] = nullptr;

	PositionChessPiece(XIndex, YIndex, bForce);
	SetHistoryRow(PieceDragging, "Moved", PreviousIndexPos);

	// Add move to list and process special moves (like En Passant, ...) if any
	MoveList.Add(TArray<FIntPoint>{PreviousIndexPos, FIntPoint(XIndex, YIndex)});
	ProcessSpecialMove(bForce);

	GS->SetIsWhiteTurn(!GS->GetIsWhiteTurn());

	// If next player is in checkmate or stalemate game is over
	ChessGameOver GameOverCondition = GameOverConditionMet();
	if(GameOverCondition == ChessGameOver::Checkmate)
	{
		GS->Checkmate(PieceDragging->Team);
	}
	else if(GameOverCondition == ChessGameOver::Stalemate)
	{
		GS->Stalemate();
	}

	return true;
}

void ACGChessBoard::EatChessPiece(ACGChessPiece* TargetPiece, bool bForce /*= false*/)
{
	// Move eaten pieces off board and scale it down
	if(TargetPiece->Team == 0)
	{
		WhitePiecesDead.Add(TargetPiece);
		TargetPiece->SetPiecePosition(FVector(-1 * TileUnitSize, 8 * TileUnitSize, ZOffset + 15) - Bounds + (FVector::ForwardVector * DeadPieceSpacing * (WhitePiecesDead.Num() - 1)), bForce);
	}
	else
	{
		BlackPiecesDead.Add(TargetPiece);
		TargetPiece->SetPiecePosition(FVector(8 * TileUnitSize, -1 * TileUnitSize, ZOffset + 15) - Bounds + (FVector::BackwardVector * DeadPieceSpacing * (BlackPiecesDead.Num() - 1)), bForce);
	}
	TargetPiece->SetPieceScale(FVector::OneVector * DeadPieceScale, bForce);
}

bool ACGChessBoard::ContainsValidMove(TArray<FIntPoint>& Moves, int32 XIndex, int32 YIndex)
{
	for(int32 i = 0; i < Moves.Num(); i++)
	{
		if(Moves[i].X == XIndex && Moves[i].Y == YIndex)
		{
			return true;
		}
	}

	return false;
}

void ACGChessBoard::ProcessSpecialMove(bool bForce /*= false*/)
{
	// Process all additional actions (movements) for each special move
	TArray<FIntPoint> NewMove = MoveList[MoveList.Num() - 1];

	if(SpecialMove == ChessSpecialMove::EnPassant)
	{
		ACGChessPiece* MovingPawn = ChessPiecesOnBoard[NewMove[1].X][NewMove[1].Y];
		TArray<FIntPoint> TargetPawnMove = MoveList[MoveList.Num() - 2];
		ACGChessPiece* TargetPawn = ChessPiecesOnBoard[TargetPawnMove[1].X][TargetPawnMove[1].Y];

		// Eat pawn behind moved pawn
		if(MovingPawn->CurrentY == TargetPawn->CurrentY)
		{
			if((MovingPawn->CurrentX == TargetPawn->CurrentX - 1) || (MovingPawn->CurrentX == TargetPawn->CurrentX + 1))
			{
				EatChessPiece(TargetPawn, bForce);
				ChessPiecesOnBoard[TargetPawn->CurrentX][TargetPawn->CurrentY] = nullptr;

				SetHistoryRow(TargetPawn, "Is Eaten", FIntPoint(TargetPawnMove[1].X, TargetPawnMove[1].Y));
				SetHistoryRow(MovingPawn, "En Passant", FIntPoint(NewMove[1].X, NewMove[1].Y));
			}
		}
	}

	if(SpecialMove == ChessSpecialMove::Promotion)
	{
		ACGChessPiece* MovingPawn = ChessPiecesOnBoard[NewMove[1].X][NewMove[1].Y];

		// Swap Pawn with Queen
		if((MovingPawn->Team == 0 && NewMove[1].X == 7) || (MovingPawn->Team == 1 && NewMove[1].X == 0))
		{
			ACGChessPiece* NewQueen = SpawnChessPiece(ChessPieceType::Queen, MovingPawn->Team);
			NewQueen->SetActorLocation(MovingPawn->GetActorLocation());
			ChessPiecesOnBoard[NewMove[1].X][NewMove[1].Y]->Destroy();
			ChessPiecesOnBoard[NewMove[1].X][NewMove[1].Y] = NewQueen;
			PositionChessPiece(NewMove[1].X, NewMove[1].Y, bForce);

			SetHistoryRow(MovingPawn, "Queening", FIntPoint(NewMove[1].X, NewMove[1].Y));
		}
	}

	if(SpecialMove == ChessSpecialMove::Castling)
	{
		ACGChessPiece* MovingKing = ChessPiecesOnBoard[NewMove[1].X][NewMove[1].Y];		
		int32 StartingX = (NewMove[1].X == 0) ? 0 : 7;
		ACGChessPiece* TargetRook = nullptr;

		// Move Left Rook
		if(NewMove[1].Y == 2 && (StartingX == 0 || StartingX == 7))
		{			
			TargetRook = ChessPiecesOnBoard[StartingX][0];
			ChessPiecesOnBoard[StartingX][3] = TargetRook;
			PositionChessPiece(StartingX, 3, bForce);
			ChessPiecesOnBoard[StartingX][0] = nullptr;
			SetHistoryRow(TargetRook, "Moved", FIntPoint(StartingX, 0));
		}

		// Move Right Rook
		if(NewMove[1].Y == 6 && (StartingX == 0 || StartingX == 7))
		{
			TargetRook = ChessPiecesOnBoard[StartingX][7];
			ChessPiecesOnBoard[StartingX][5] = TargetRook;
			PositionChessPiece(StartingX, 5, bForce);
			ChessPiecesOnBoard[StartingX][7] = nullptr;
			SetHistoryRow(TargetRook, "Moved", FIntPoint(StartingX, 7));
		}

		if(TargetRook)
		{			
			SetHistoryRow(MovingKing, "Castling", FIntPoint(NewMove[1].X, NewMove[1].Y));
		}
	}
}

void ACGChessBoard::PreventCheckmate()
{
	ACGChessPiece* TargetKing = nullptr;

	// Find king from same team as dragged pieces
	const int32 NumTiles = GRID_SIZE * GRID_SIZE;
	for(int32 TileIndex = 0; TileIndex < NumTiles; TileIndex++)
	{
		const int32 XPos = (TileIndex / GRID_SIZE); // Divide by dimension
		const int32 YPos = (TileIndex % GRID_SIZE); // Modulo gives remainder

		if(ChessPiecesOnBoard[XPos][YPos])
		{
			if(ChessPiecesOnBoard[XPos][YPos]->Type == ChessPieceType::King)
			{
				if(ChessPiecesOnBoard[XPos][YPos]->Team == CurrentPieceDragging->Team)
				{
					TargetKing = ChessPiecesOnBoard[XPos][YPos];
				}
			}
		}
	}

	// Delete all moves that are putting King in Checkmate position
	SimulateMoveForPieceDragging(CurrentPieceDragging, MoveList, AvailableMoves, TargetKing);
}

void ACGChessBoard::SimulateMoveForPieceDragging(ACGChessPiece* Dragging, TArray<TArray<FIntPoint>>& AllMoves, TArray<FIntPoint>& AllowableMoves, ACGChessPiece* King)
{
	// Save current values
	int32 SavedX = Dragging->CurrentX;
	int32 SavedY = Dragging->CurrentY;
	TArray<FIntPoint> MovesToRemove;

	// Simulate moves to see if king is in Checkmate
	for(int32 i = 0; i < AllowableMoves.Num(); i++)
	{
		int32 SimX = AllowableMoves[i].X;
		int32 SimY = AllowableMoves[i].Y;

		FIntPoint SimKingPosition = FIntPoint(King->CurrentX, King->CurrentY);
		// Are we dragging King
		if(Dragging->Type == ChessPieceType::King)
		{
			SimKingPosition = FIntPoint(SimX, SimY);
		}

		// Copy Board and add enemy pieces that can attack, so we can use it for simulation
		ACGChessPiece* SimulationBoard[GRID_SIZE][GRID_SIZE];
		TArray<ACGChessPiece*> SimAttackPiece;
		const int32 NumTiles = GRID_SIZE * GRID_SIZE;
		for(int32 TileIndex = 0; TileIndex < NumTiles; TileIndex++)
		{
			const int32 XPos = (TileIndex / GRID_SIZE); // Divide by dimension
			const int32 YPos = (TileIndex % GRID_SIZE); // Modulo gives remainder

			SimulationBoard[XPos][YPos] = ChessPiecesOnBoard[XPos][YPos];
			if(SimulationBoard[XPos][YPos])
			{
				if(SimulationBoard[XPos][YPos]->Team != Dragging->Team)
				{
					// Do not add attack if enemy piece is eaten during simulation
					if(!(SimulationBoard[XPos][YPos]->CurrentX == SimX && SimulationBoard[XPos][YPos]->CurrentY == SimY))
					{
						SimAttackPiece.Add(SimulationBoard[XPos][YPos]);
					}
				}
			}
		}

		// Check for Special Move En-Passant
		if(Dragging->Type == ChessPieceType::Pawn)
		{
			TArray<FIntPoint> SimAvailableMoves = Dragging->GetAvailableMoves(SimulationBoard, GRID_SIZE);
			ChessSpecialMove SimSpecialMoves = Dragging->GetSpecialMoves(SimulationBoard, AllMoves, SimAvailableMoves);
			if(SimSpecialMoves == ChessSpecialMove::EnPassant)
			{
				TArray<FIntPoint> TargetPawnPosition = AllMoves[AllMoves.Num() - 1];
				ACGChessPiece* EnemyPawn = SimulationBoard[TargetPawnPosition[1].X][TargetPawnPosition[1].Y];
				if(EnemyPawn->CurrentY == SimY)
				{
					// Remove that pawn if simulated move is En-Passant
					SimAttackPiece.Remove(EnemyPawn);
					SimulationBoard[TargetPawnPosition[1].X][TargetPawnPosition[1].Y] = nullptr;
				}
			}
		}

		// Simulate move
		SimulationBoard[SavedX][SavedY] = nullptr;
		Dragging->CurrentX = SimX;
		Dragging->CurrentY = SimY;
		SimulationBoard[SimX][SimY] = Dragging;

		// Simulate all attacking moves
		TArray<FIntPoint> SimAttackingMoves;
		for(int32 am = 0; am < SimAttackPiece.Num(); am++)
		{
			TArray<FIntPoint> PieceMoves = SimAttackPiece[am]->GetAvailableMoves(SimulationBoard, GRID_SIZE);
			for(int32 pm = 0; pm < PieceMoves.Num(); pm++)
			{
				SimAttackingMoves.Add(PieceMoves[pm]);
			}
		}

		// Check is there Checkmate on King
		if(ContainsValidMove(SimAttackingMoves, SimKingPosition.X, SimKingPosition.Y))
		{
			MovesToRemove.Add(AllowableMoves[i]);
		}

		// Restore saved values
		Dragging->CurrentX = SavedX;
		Dragging->CurrentY = SavedY;
	}

	// If Checkmate position, remove it from Available Move list
	for(int32 i = 0; i < MovesToRemove.Num(); i++)
	{
		AllowableMoves.Remove(MovesToRemove[i]);
	}
}

ChessGameOver ACGChessBoard::GameOverConditionMet()
{
	TArray<FIntPoint> LastMove = MoveList[MoveList.Num() - 1];
	int32 TargetTeam = (ChessPiecesOnBoard[LastMove[1].X][LastMove[1].Y]->Team == 0) ? 1 : 0;

	// Make list of attacking and defending pieces on board, and defending king
	TArray<ACGChessPiece*> AttackingPieces;
	TArray<ACGChessPiece*> DefendingPieces;
	ACGChessPiece* TargetKing = nullptr;
	const int32 NumTiles = GRID_SIZE * GRID_SIZE;
	for(int32 TileIndex = 0; TileIndex < NumTiles; TileIndex++)
	{
		const int32 XPos = (TileIndex / GRID_SIZE); // Divide by dimension
		const int32 YPos = (TileIndex % GRID_SIZE); // Modulo gives remainder

		if(ChessPiecesOnBoard[XPos][YPos])
		{
			if(ChessPiecesOnBoard[XPos][YPos]->Team == TargetTeam)
			{
				DefendingPieces.Add(ChessPiecesOnBoard[XPos][YPos]);
				if(ChessPiecesOnBoard[XPos][YPos]->Type == ChessPieceType::King)
				{
					TargetKing = ChessPiecesOnBoard[XPos][YPos];
				}
			}
			else
			{
				AttackingPieces.Add(ChessPiecesOnBoard[XPos][YPos]);
			}
		}
	}

	// Is King under attack
	TArray<FIntPoint> CurrentAvailableMoves;
	for(int32 i = 0; i < AttackingPieces.Num(); i++)
	{
		TArray<FIntPoint> PieceMoves = AttackingPieces[i]->GetAvailableMoves(ChessPiecesOnBoard, GRID_SIZE);
		for(int32 pm = 0; pm < PieceMoves.Num(); pm++)
		{
			CurrentAvailableMoves.Add(PieceMoves[pm]);
		}
	}
	// Is Check
	if(ContainsValidMove(CurrentAvailableMoves, TargetKing->CurrentX, TargetKing->CurrentY))
	{
		// Is there available moves
		for(int32 i = 0; i < DefendingPieces.Num(); i++)
		{
			TArray<FIntPoint> DefendingMoves = DefendingPieces[i]->GetAvailableMoves(ChessPiecesOnBoard, GRID_SIZE);
			SimulateMoveForPieceDragging(DefendingPieces[i], MoveList, DefendingMoves, TargetKing);
			// Not Checkmate
			if(DefendingMoves.Num() != 0)
			{
				return ChessGameOver::NotOver;
			}
		}

		// Checkmate
		return ChessGameOver::Checkmate;
	}
	else
	{
		// Is there available moves
		for(int32 i = 0; i < DefendingPieces.Num(); i++)
		{
			TArray<FIntPoint> DefendingMoves = DefendingPieces[i]->GetAvailableMoves(ChessPiecesOnBoard, GRID_SIZE);
			SimulateMoveForPieceDragging(DefendingPieces[i], MoveList, DefendingMoves, TargetKing);
			// Not Stalemate
			if(DefendingMoves.Num() != 0)
			{
				return ChessGameOver::NotOver;
			}
		}

		// Stalemate
		return ChessGameOver::Stalemate;
	}

	return ChessGameOver::NotOver;
}

void ACGChessBoard::SetHistoryRow(ACGChessPiece* Piece, FString Action, FIntPoint PrevPosition)
{
	// Populate struct
	FCGHistoryStruct HistoryRow;
	HistoryRow.Team = Piece->Team;
	HistoryRow.PieceType = Piece->Type;
	HistoryRow.GridLocation = FIntPoint(Piece->CurrentX, Piece->CurrentY);
	HistoryRow.ActionStr = Action;
	HistoryRow.PrevLocation = PrevPosition;
	// Add it to list and send it to UI
	GameHistory.Add(HistoryRow);
	GS->DisplayHistory(HistoryRow);
}

FVector ACGChessBoard::GetTileCenter(int32 X, int32 Y)
{
	return FVector(X * TileUnitSize, Y * TileUnitSize, ZOffset) - Bounds;
}

FIntPoint ACGChessBoard::GetTileIndex(int32 GridSize, ACGBoardTile* Tile)
{
	// Number of tiles
	const int32 NumTiles = GridSize * GridSize;

	// Loop trough tiles and find X and Y index of a tile
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

void ACGChessBoard::HighlightAvailableTiles()
{
	for(int i = 0; i < AvailableMoves.Num(); i++)
	{
		ChessTiles[AvailableMoves[i].X][AvailableMoves[i].Y]->AvailableHighlight(true);
	}
}

void ACGChessBoard::RemoveHighlightTiles()
{
	for(int i = 0; i < AvailableMoves.Num(); i++)
	{
		ChessTiles[AvailableMoves[i].X][AvailableMoves[i].Y]->AvailableHighlight(false);
	}
	AvailableMoves.Empty();
}

void ACGChessBoard::ResetBoard()
{
	// Reset Fields	
	CurrentPieceDragging = nullptr;
	AvailableMoves.Empty();
	GameHistory.Empty();
	MoveList.Empty();

	// Remove pieces on board
	const int32 NumTiles = GRID_SIZE * GRID_SIZE;
	for(int32 TileIndex = 0; TileIndex < NumTiles; TileIndex++)
	{
		const int32 XPos = (TileIndex / GRID_SIZE); // Divide by dimension
		const int32 YPos = (TileIndex % GRID_SIZE); // Modulo gives remainder

		if(ChessPiecesOnBoard[XPos][YPos] != nullptr)
		{
			ChessPiecesOnBoard[XPos][YPos]->Destroy();
			ChessPiecesOnBoard[XPos][YPos] = nullptr;
		}
	}

	// Remove dead pieces
	for(int32 i = 0; i < WhitePiecesDead.Num(); i++)
	{
		WhitePiecesDead[i]->Destroy();
	}
	WhitePiecesDead.Empty();
	for(int32 i = 0; i < BlackPiecesDead.Num(); i++)
	{
		BlackPiecesDead[i]->Destroy();
	}
	BlackPiecesDead.Empty();

	// Set pieces to start position
	SpawnAllChessPieces();
	PositionAllChessPieces(GRID_SIZE);
	GS->SetIsWhiteTurn(true);
}

int32 ACGChessBoard::UndoMove()
{
	int32 UndoMoves = 0;

	if(GameHistory.Num() > 0)
	{
		// Get last history entry
		FCGHistoryStruct LastHistory = GameHistory[GameHistory.Num() - 1];

		if(LastHistory.ActionStr.Equals("Moved"))
		{
			ACGChessPiece* MovedPiece = ChessPiecesOnBoard[LastHistory.GridLocation.X][LastHistory.GridLocation.Y];
			ChessPiecesOnBoard[LastHistory.GridLocation.X][LastHistory.GridLocation.Y] = nullptr;

			if(GameHistory.Num() > 1)
			{
				// Check is entry before last history entry is piece eaten
				FCGHistoryStruct EatenHistory = GameHistory[GameHistory.Num() - 2];
				if(EatenHistory.ActionStr.Equals("Is Eaten"))
				{
					// Restore dead piece
					ACGChessPiece* EatenPiece;
					if(EatenHistory.Team == 0)
					{
						EatenPiece = WhitePiecesDead[WhitePiecesDead.Num() - 1];
						WhitePiecesDead.Remove(EatenPiece);
					}
					else
					{
						EatenPiece = BlackPiecesDead[BlackPiecesDead.Num() - 1];
						BlackPiecesDead.Remove(EatenPiece);
					}
					ChessPiecesOnBoard[EatenHistory.PrevLocation.X][EatenHistory.PrevLocation.Y] = EatenPiece;
					EatenPiece->SetPieceScale(FVector::OneVector * EatenPiece->GetDefaultScale());
					PositionChessPiece(EatenHistory.PrevLocation.X, EatenHistory.PrevLocation.Y);

					// Remove eaten history entry and add to return int for UI to know
					GameHistory.RemoveAt(GameHistory.Num() - 2);
					UndoMoves++;
				}
			}

			// Finish moving piece from last history entry
			ChessPiecesOnBoard[LastHistory.PrevLocation.X][LastHistory.PrevLocation.Y] = MovedPiece;
			PositionChessPiece(LastHistory.PrevLocation.X, LastHistory.PrevLocation.Y);

			// Remove last history entry, remove last player move, set enemy turn and add to return int for UI to know
			GameHistory.RemoveAt(GameHistory.Num() - 1);
			MoveList.RemoveAt(MoveList.Num() - 1);
			GS->SetIsWhiteTurn(!GS->GetIsWhiteTurn());
			UndoMoves++;
		}
		else
		{
			// Check is last history entry is one of special moves
			if(LastHistory.ActionStr.Equals("En Passant"))
			{
				// Remove last history entry and add to return int for UI to know
				GameHistory.RemoveAt(GameHistory.Num() - 1);
				UndoMoves++;

				// Set new last history and restore dead pawn
				LastHistory = GameHistory[GameHistory.Num() - 1];
				ACGChessPiece* EatenPiece;
				if(LastHistory.Team == 0)
				{
					EatenPiece = WhitePiecesDead[WhitePiecesDead.Num() - 1];
					WhitePiecesDead.Remove(EatenPiece);
				}
				else
				{
					EatenPiece = BlackPiecesDead[BlackPiecesDead.Num() - 1];
					BlackPiecesDead.Remove(EatenPiece);
				}
				ChessPiecesOnBoard[LastHistory.PrevLocation.X][LastHistory.PrevLocation.Y] = EatenPiece;
				EatenPiece->SetPieceScale(FVector::OneVector * EatenPiece->GetDefaultScale());
				PositionChessPiece(LastHistory.PrevLocation.X, LastHistory.PrevLocation.Y);

				// Remove eaten history entry and add to return int for UI to know
				GameHistory.RemoveAt(GameHistory.Num() - 1);
				UndoMoves++;

				// Set new last history and move pawn
				LastHistory = GameHistory[GameHistory.Num() - 1];
				ACGChessPiece* MovedPiece = ChessPiecesOnBoard[LastHistory.GridLocation.X][LastHistory.GridLocation.Y];
				ChessPiecesOnBoard[LastHistory.GridLocation.X][LastHistory.GridLocation.Y] = nullptr;
				ChessPiecesOnBoard[LastHistory.PrevLocation.X][LastHistory.PrevLocation.Y] = MovedPiece;
				PositionChessPiece(LastHistory.PrevLocation.X, LastHistory.PrevLocation.Y);

				// Remove last history entry, remove last player move, set enemy turn and add to return int for UI to know
				GameHistory.RemoveAt(GameHistory.Num() - 1);
				MoveList.RemoveAt(MoveList.Num() - 1);
				GS->SetIsWhiteTurn(!GS->GetIsWhiteTurn());
				UndoMoves++;
			}
			else if(LastHistory.ActionStr.Equals("Queening"))
			{
				// Swap queen with pawn
				ACGChessPiece* NewPawn = SpawnChessPiece(ChessPieceType::Pawn, LastHistory.Team);
				NewPawn->SetActorLocation(ChessPiecesOnBoard[LastHistory.GridLocation.X][LastHistory.GridLocation.Y]->GetActorLocation());
				ChessPiecesOnBoard[LastHistory.GridLocation.X][LastHistory.GridLocation.Y]->Destroy();
				ChessPiecesOnBoard[LastHistory.GridLocation.X][LastHistory.GridLocation.Y] = NewPawn;
				PositionChessPiece(LastHistory.GridLocation.X, LastHistory.GridLocation.Y);

				// Remove eaten history entry and add to return int for UI to know
				GameHistory.RemoveAt(GameHistory.Num() - 1);
				UndoMoves++;

				// Set new last history and move pawn
				LastHistory = GameHistory[GameHistory.Num() - 1];
				ChessPiecesOnBoard[LastHistory.GridLocation.X][LastHistory.GridLocation.Y] = nullptr;
				ChessPiecesOnBoard[LastHistory.PrevLocation.X][LastHistory.PrevLocation.Y] = NewPawn;
				PositionChessPiece(LastHistory.PrevLocation.X, LastHistory.PrevLocation.Y);

				// Remove last history entry, remove last player move, set enemy turn and add to return int for UI to know
				GameHistory.RemoveAt(GameHistory.Num() - 1);
				MoveList.RemoveAt(MoveList.Num() - 1);
				GS->SetIsWhiteTurn(!GS->GetIsWhiteTurn());
				UndoMoves++;
			}
			else if(LastHistory.ActionStr.Equals("Castling"))
			{
				// Remove last history entry and add to return int for UI to know
				GameHistory.RemoveAt(GameHistory.Num() - 1);
				UndoMoves++;

				// Move rook and king to previous locations
				for(int32 i = 0; i < 2; i++)
				{
					// Set new last history
					LastHistory = GameHistory[GameHistory.Num() - 1];
					// Move piece
					ACGChessPiece* MovedPiece = ChessPiecesOnBoard[LastHistory.GridLocation.X][LastHistory.GridLocation.Y];
					ChessPiecesOnBoard[LastHistory.GridLocation.X][LastHistory.GridLocation.Y] = nullptr;
					ChessPiecesOnBoard[LastHistory.PrevLocation.X][LastHistory.PrevLocation.Y] = MovedPiece;
					PositionChessPiece(LastHistory.PrevLocation.X, LastHistory.PrevLocation.Y);

					// Remove eaten history entry and add to return int for UI to know
					GameHistory.RemoveAt(GameHistory.Num() - 1);
					UndoMoves++;
				}

				// Remove last player move and set enemy turn
				MoveList.RemoveAt(MoveList.Num() - 1);
				GS->SetIsWhiteTurn(!GS->GetIsWhiteTurn());
			}
		}
	}

	return UndoMoves;
}

void ACGChessBoard::LoadGame()
{
	USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0);
	SaveGameObject = Cast<UCGSaveGame>(LoadedGame);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Trying to load a saved game."));

	if(SaveGameObject)
	{
		// Reset history HUD, board and Load variables
		GS->ResetHistoryHUD();
		ResetBoard();
		for(int32 i = 0; i < SaveGameObject->SavedMoveList.Num(); i++)
		{
			FCGSaveStruct SaveStruct = SaveGameObject->SavedMoveList[i];
			AvailableMoves = ChessPiecesOnBoard[SaveStruct.Moves[0].X][SaveStruct.Moves[0].Y]->GetAvailableMoves(ChessPiecesOnBoard, GRID_SIZE);
			SpecialMove = ChessPiecesOnBoard[SaveStruct.Moves[0].X][SaveStruct.Moves[0].Y]->GetSpecialMoves(ChessPiecesOnBoard, MoveList, AvailableMoves);
			MovePieceTo(ChessPiecesOnBoard[SaveStruct.Moves[0].X][SaveStruct.Moves[0].Y], SaveStruct.Moves[1].X, SaveStruct.Moves[1].Y, true);
		}
		AvailableMoves.Empty();

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Saved game found. Loaded."));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("No saved games found."));
	}
}

void ACGChessBoard::SaveGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Saving game..."));
	SaveGameObject = Cast<UCGSaveGame>(UGameplayStatics::CreateSaveGameObject(UCGSaveGame::StaticClass()));

	// Save variables
	for(int32 i = 0; i < MoveList.Num(); i++)
	{
		FCGSaveStruct SaveStruct;
		TArray<FIntPoint> MovesPlayed = MoveList[i];
		for(int32 j = 0; j < MovesPlayed.Num(); j++)
		{
			SaveStruct.Moves.Add(MovesPlayed[j]);
		}
		SaveGameObject->SavedMoveList.Add(SaveStruct);
	}

	const bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);
	if(IsSaved)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Game saved."));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Cannot save the game."));
	}
}

bool ACGChessBoard::ContainsValidMove(ACGBoardTile* Tile)
{
	FIntPoint TileIndex = GetTileIndex(GRID_SIZE, Tile);

	for(int32 i = 0; i < AvailableMoves.Num(); i++)
	{
		if(AvailableMoves[i].X == TileIndex.X && AvailableMoves[i].Y == TileIndex.Y)
		{
			return true;
		}
	}

	return false;
}

void ACGChessBoard::HandleTileClicked(ACGBoardTile* Tile)
{
	FIntPoint TileIndexPos = GetTileIndex(GRID_SIZE, Tile);

	if(ChessPiecesOnBoard[TileIndexPos.X][TileIndexPos.Y])
	{
		// Is White turn or Black turn
		if((ChessPiecesOnBoard[TileIndexPos.X][TileIndexPos.Y]->Team == 0 && GS->GetIsWhiteTurn())
			|| (ChessPiecesOnBoard[TileIndexPos.X][TileIndexPos.Y]->Team == 1 && !GS->GetIsWhiteTurn()))
		{
			CurrentPieceDragging = ChessPiecesOnBoard[TileIndexPos.X][TileIndexPos.Y];

			// Get list of available basic moves
			AvailableMoves = CurrentPieceDragging->GetAvailableMoves(ChessPiecesOnBoard, GRID_SIZE);
			// Get list of special moves
			SpecialMove = CurrentPieceDragging->GetSpecialMoves(ChessPiecesOnBoard, MoveList, AvailableMoves);
			// Prevent Checkmate move
			PreventCheckmate();

			// Highlight tiles
			HighlightAvailableTiles();
		}
	}
}

void ACGChessBoard::HandleTileReleased(ACGBoardTile* Tile)
{
	if(CurrentPieceDragging)
	{
		FIntPoint PreviousIndexPos = FIntPoint(CurrentPieceDragging->CurrentX, CurrentPieceDragging->CurrentY);		

		// Is dragged piece is released over tile
		if(Tile)
		{
			FIntPoint TileIndexPos = GetTileIndex(GRID_SIZE, Tile);
			bool bValidMove = MovePieceTo(CurrentPieceDragging, TileIndexPos.X, TileIndexPos.Y);
			if(!bValidMove)
			{
				// If move is not valid return dragged piece to previous position
				CurrentPieceDragging->SetPiecePosition(GetTileCenter(PreviousIndexPos.X, PreviousIndexPos.Y));				
			}
			CurrentPieceDragging = nullptr;
		}
		else
		{
			// Return dragged piece to previous position
			CurrentPieceDragging->SetPiecePosition(GetTileCenter(PreviousIndexPos.X, PreviousIndexPos.Y));
			CurrentPieceDragging = nullptr;
		}

		RemoveHighlightTiles();
	}
}
