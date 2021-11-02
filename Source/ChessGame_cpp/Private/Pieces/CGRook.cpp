// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/CGRook.h"

// Sets default values
ACGRook::ACGRook()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> ModelMesh;
		FConstructorStatics()
			: ModelMesh(TEXT("/Game/Meshes/Pieces/SM_Rook.SM_Rook"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	// Set static mesh
	PieceMesh->SetStaticMesh(ConstructorStatics.ModelMesh.Get());
}
