// Fill out your copyright notice in the Description page of Project Settings.

#include "TunnelMover.h"
#include "Kismet/KismetMathLibrary.h"

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

	PositionTunnelMeshes();
}

void ATunnelMover::OnConstruction(const FTransform& Transform) {
	PositionTunnelMeshes();
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
			SpawnSectionObstacles(TunnelMesh1);
		}
		if (X2 <= -MeshLength) {
			TunnelMesh2->SetRelativeLocation(FVector(MaxX + MeshLength, 0.0f, 0.0f));
			MaxX += MeshLength;  // Update max position
			SpawnSectionObstacles(TunnelMesh2);
		}
		if (X3 <= -MeshLength) {
			TunnelMesh3->SetRelativeLocation(FVector(MaxX + MeshLength, 0.0f, 0.0f));
			SpawnSectionObstacles(TunnelMesh3);
		}
	}

	UpdateObstacles(DeltaTime);
	DestroyOffscreenObstacles();
}

void ATunnelMover::PositionTunnelMeshes() {
	if (TunnelMesh1 && TunnelMesh2 && TunnelMesh3) {
		TunnelMesh1->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		TunnelMesh2->SetRelativeLocation(FVector(MeshLength, 0.0f, 0.0f));
		TunnelMesh3->SetRelativeLocation(FVector(2 * MeshLength, 0.0f, 0.0f));
	}
}

void ATunnelMover::SpawnSectionObstacles(UStaticMeshComponent* ResetMesh) {
	if (ObstacleClasses.Num() == 0 || Difficulty < 1) return;

	// Calculate number of obstacles based on difficulty
	int32 ObstacleCount = FMath::Clamp(Difficulty, 1, 5);

	// Get reset mesh's world position
	const FVector SectionStart = ResetMesh->GetComponentLocation();
	const FVector SectionEnd = SectionStart + FVector(MeshLength, 0, 0);

	// Calculate possible spawn positions
	TArray<FVector> SpawnPositions;
	float CurrentX = SectionStart.X + MinObstacleSpacing;

	while (CurrentX < SectionEnd.X - MinObstacleSpacing) {
		SpawnPositions.Add(FVector(CurrentX, 0, 0));
		CurrentX += MinObstacleSpacing;

		// Randomize next position
		CurrentX += FMath::RandRange(0.2f, 0.8f) * MinObstacleSpacing;
	}

	// Randomize positions and select based on obstacle count
	SpawnPositions.Sort([](const FVector& A, const FVector& B) {
		return UKismetMathLibrary::RandomBool();
		});

	// Spawn obstacles
	int32 SpawnedCount = 0;
	for (int32 i = 0; i < FMath::Min(ObstacleCount, SpawnPositions.Num()) && SpawnedCount < ObstacleCount; i++) {
		// Random obstacle selection
		const int32 ObstacleIndex = FMath::RandRange(0, ObstacleClasses.Num() - 1);
		TSubclassOf<AActor> SelectedObstacle = ObstacleClasses[ObstacleIndex];

		if (!SelectedObstacle) continue;

		// Calculate spawn position
		FVector SpawnPos = SpawnPositions[i];
		//SpawnPos.Y = FMath::RandRange(-TunnelWidth / 2, TunnelWidth / 2);

		// Spawn with random rotation
		FRotator SpawnRot(0, 0, FMath::RandRange(0.0f, 360.0f));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* NewObstacle = GetWorld()->SpawnActor<AActor>(
			SelectedObstacle,
			SpawnPos,
			SpawnRot,
			SpawnParams
		);

		if (NewObstacle) {
			SpawnedCount++;
			// Attach obstacle to the tunnel section so it moves with it
			//NewObstacle->AttachToComponent(ResetMesh, FAttachmentTransformRules::KeepWorldTransform);
			SpawnedObstacles.Add(NewObstacle);
		}

		//Track spawned obstacles in array
	}
}

void ATunnelMover::UpdateObstacles(float DeltaTime) {
	// Move all obstacles with the tunnel
	const FVector Movement = FVector(-Speed * DeltaTime, 0.0f, 0.0f);

	for (AActor* Obstacle : SpawnedObstacles) {
		if (IsValid(Obstacle)) {
			Obstacle->AddActorWorldOffset(Movement, true);
		}
	}
}

void ATunnelMover::DestroyOffscreenObstacles() {
	// Destroy obstacles that have moved off-screen
	for (int32 i = SpawnedObstacles.Num() - 1; i >= 0; i--) {
		AActor* Obstacle = SpawnedObstacles[i];

		if (!IsValid(Obstacle)) {
			SpawnedObstacles.RemoveAt(i);
			continue;
		}

		// Check if obstacle is behind the player
		if (Obstacle->GetActorLocation().X < -1000.0f) {
			Obstacle->Destroy();
			SpawnedObstacles.RemoveAt(i);
		}
	}
}