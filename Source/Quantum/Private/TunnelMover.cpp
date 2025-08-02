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
	TunnelMesh1->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 90, 0)));

	TunnelMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TunnelMesh2"));
	TunnelMesh2->SetupAttachment(RootComponent);
	TunnelMesh2->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 90, 0)));

	TunnelMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TunnelMesh3"));
	TunnelMesh3->SetupAttachment(RootComponent);
	TunnelMesh3->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 90, 0)));


	// Initialize default values
	Speed = 200.0f;
	MeshLength = 1000.0f;
}

// Called when the game starts or when spawned
void ATunnelMover::BeginPlay() {
	Super::BeginPlay();

	// Set initial positions
	if (TunnelMesh1 && TunnelMesh2 && TunnelMesh3) {
		TunnelMesh1->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		TunnelMesh2->SetRelativeLocation(FVector(MeshLength, 0.0f, 0.0f));
		TunnelMesh3->SetRelativeLocation(FVector(2 * MeshLength, 0.0f, 0.0f));
	}
}

void ATunnelMover::OnConstruction(const FTransform& Transform) {
	if (TunnelMesh1 && TunnelMesh2 && TunnelMesh3) {
		TunnelMesh1->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		TunnelMesh2->SetRelativeLocation(FVector(MeshLength, 0.0f, 0.0f));
		TunnelMesh3->SetRelativeLocation(FVector(2 * MeshLength, 0.0f, 0.0f));
	}
}

// Called every frame
void ATunnelMover::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Move both meshes
	const float Movement = Speed * DeltaTime;
	const FVector MovementVector = FVector(-Movement, 0.0f, 0.0f);

	if (TunnelMesh1) TunnelMesh1->AddRelativeLocation(MovementVector);
	if (TunnelMesh2) TunnelMesh2->AddRelativeLocation(MovementVector);
	if (TunnelMesh3) TunnelMesh3->AddRelativeLocation(MovementVector);

	// Check and reset positions
	if (TunnelMesh1 && TunnelMesh2 && TunnelMesh3) {
		// Get current positions
		const float X1 = TunnelMesh1->GetRelativeLocation().X;
		const float X2 = TunnelMesh2->GetRelativeLocation().X;
		const float X3 = TunnelMesh3->GetRelativeLocation().X;

		// Find the rightmost position
		float MaxX = FMath::Max3(X1, X2, X3);

		// Reset any mesh that's moved too far left
		if (X1 <= -MeshLength) {
			TunnelMesh1->SetRelativeLocation(FVector(MaxX + MeshLength, 0.0f, 0.0f));
			MaxX += MeshLength;  // Update max position
		}
		if (X2 <= -MeshLength) {
			TunnelMesh2->SetRelativeLocation(FVector(MaxX + MeshLength, 0.0f, 0.0f));
			MaxX += MeshLength;  // Update max position
		}
		if (X3 <= -MeshLength) {
			TunnelMesh3->SetRelativeLocation(FVector(MaxX + MeshLength, 0.0f, 0.0f));
		}
	}
}