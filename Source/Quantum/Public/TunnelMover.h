// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TunnelMover.generated.h"

UCLASS()
class QUANTUM_API ATunnelMover : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATunnelMover();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Tunnel mesh components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tunnel")
	UStaticMeshComponent* TunnelMesh1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tunnel")
	UStaticMeshComponent* TunnelMesh2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tunnel")
	UStaticMeshComponent* TunnelMesh3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tunnel")
	TArray<UStaticMesh*> TunnelMeshesVariants;

	// Movement speed (editable in editor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tunnel")
	float Speed;

	// Obstacle spawning parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacles")
	TArray<TSubclassOf<AActor>> ObstacleClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacles")
	int32 Difficulty = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacles")
	float TunnelWidth = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacles")
	float MinObstacleSpacing = 200.f;

	// Length of each tunnel segment (editable in editor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tunnel")
	float MeshLength;

private:

	void SetRandomMesh(UStaticMeshComponent* MeshComp);
	void PositionTunnelMeshes();
	void SpawnSectionObstacles(UStaticMeshComponent* ResetMesh);
	void UpdateObstacles(float DeltaTime);
	void DestroyOffscreenObstacles();

	TArray<AActor*> SpawnedObstacles;
};
