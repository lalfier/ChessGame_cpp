// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGKnight.h"

// Sets default values
ACGKnight::ACGKnight()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh;
		FConstructorStatics()
			: ModelMesh(TEXT("/Game/Meshes/Pieces/SM_Knight.SM_Knight"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	// Set static mesh
	PieceMesh->SetStaticMesh(ConstructorStatics.ModelMesh.Get());
}
