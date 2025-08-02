// Fill out your copyright notice in the Description page of Project Settings.

#include "TunnelMover.h"

// Sets default values
ATunnelMover::ATunnelMover() {
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create and attach tunnel meshes
	TunnelMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TunnelMesh1"));
	TunnelMesh1->SetupAttachment(RootComponent);

	TunnelMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TunnelMesh2"));
	TunnelMesh2->SetupAttachment(RootComponent);

	// Initialize default values
	Speed = 200.0f;
	MeshLength = 1000.0f;
}

// Called when the game starts or when spawned
void ATunnelMover::BeginPlay() {
	Super::BeginPlay();

	// Set initial positions
	if (TunnelMesh1 && TunnelMesh2) {
		TunnelMesh1->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		TunnelMesh2->SetRelativeLocation(FVector(MeshLength, 0.0f, 0.0f));
	}
}

// Called every frame
void ATunnelMover::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Move both meshes
	const float Movement = Speed * DeltaTime;
	const FVector MovementVector = FVector(-Movement, 0.0f, 0.0f);

	if (TunnelMesh1) {
		TunnelMesh1->AddRelativeLocation(MovementVector);
	}
	if (TunnelMesh2) {
		TunnelMesh2->AddRelativeLocation(MovementVector);
	}

	// Check and reset positions
	if (TunnelMesh1 && TunnelMesh2) {
		const float X1 = TunnelMesh1->GetRelativeLocation().X;
		const float X2 = TunnelMesh2->GetRelativeLocation().X;

		if (X1 <= -MeshLength) {
			TunnelMesh1->SetRelativeLocation(FVector(X2 + MeshLength, 0.0f, 0.0f));
		}
		if (X2 <= -MeshLength) {
			TunnelMesh2->SetRelativeLocation(FVector(X1 + MeshLength, 0.0f, 0.0f));
		}
	}
}