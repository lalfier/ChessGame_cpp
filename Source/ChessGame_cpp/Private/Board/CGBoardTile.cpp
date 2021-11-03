// Fill out your copyright notice in the Description page of Project Settings.


#include "Board/CGBoardTile.h"
#include "Board/CGChessBoard.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

// Sets default values
ACGBoardTile::ACGBoardTile()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> GreenMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> YellowMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Meshes/SM_Plane.SM_Plane"))
			, BaseMaterial(TEXT("/Game/Meshes/M_Base.M_Base"))
			, GreenMaterial(TEXT("/Game/Meshes/MI_Green.MI_Green"))
			, RedMaterial(TEXT("/Game/Meshes/MI_Red.MI_Red"))
			, YellowMaterial(TEXT("/Game/Meshes/MI_Yellow.MI_Yellow"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh0"));
	TileMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	TileMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	TileMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	TileMesh->SetMaterial(0, ConstructorStatics.BaseMaterial.Get());
	TileMesh->SetupAttachment(DummyRoot);

	// Save a pointer to the materials
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	GreenMaterial = ConstructorStatics.GreenMaterial.Get();
	RedMaterial = ConstructorStatics.RedMaterial.Get();
	YellowMaterial = ConstructorStatics.YellowMaterial.Get();
}

void ACGBoardTile::HandleClicked()
{
	// Check we are not already active
	if(!bIsActive)
	{
		bIsActive = true;

		// Change material
		TileMesh->SetMaterial(0, RedMaterial);
	}
}

void ACGBoardTile::HandleReleased(bool bIsHovered)
{
	// Check we are already active
	if(bIsActive)
	{
		bIsActive = false;

		// Change material
		if(bIsHovered)
		{
			TileMesh->SetMaterial(0, YellowMaterial);
		}
		else
		{
			TileMesh->SetMaterial(0, BaseMaterial);
		}		
	}
}

void ACGBoardTile::HoverHighlight(bool bOn)
{
	// Do not highlight if the tile has already been activated.
	if(bIsActive)
	{
		return;
	}

	// Change material
	if(bOn)
	{
		TileMesh->SetMaterial(0, YellowMaterial);
	}
	else
	{
		TileMesh->SetMaterial(0, BaseMaterial);
	}
}

void ACGBoardTile::AvailableHighlight(bool bOn)
{
	// Change material
	if(bOn)
	{
		TileMesh->SetMaterial(0, GreenMaterial);
	}
	else
	{
		TileMesh->SetMaterial(0, BaseMaterial);
	}
}
