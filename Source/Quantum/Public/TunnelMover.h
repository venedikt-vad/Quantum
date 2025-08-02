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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Tunnel mesh components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tunnel")
	UStaticMeshComponent* TunnelMesh1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tunnel")
	UStaticMeshComponent* TunnelMesh2;

	// Movement speed (editable in editor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tunnel")
	float Speed;

	// Length of each tunnel segment (editable in editor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tunnel")
	float MeshLength;

};
