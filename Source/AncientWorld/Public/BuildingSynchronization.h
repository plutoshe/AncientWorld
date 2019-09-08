// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingSynchronization.generated.h"

UCLASS()
class ANCIENTWORLD_API ABuildingSynchronization : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingSynchronization();
	void ConfirmBuilding();
	FVector ReturnSelectedPosition(FVector mousePosition);
	FVector GetBuildingPositoinFromIndex(FVector index);
	FVector GetCurrentSelectLocation();
	void InitialBlock(class AStaticMeshActor* newBlock, int buildingId, bool setMaterial);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	class UAncientWorldGameInstance* m_gameStateInstance;
	TArray<class AStaticMeshActor*> m_buildings;
	TArray<FVector> m_BuildingSlot;
	FVector basePoint;

	int m_select;
};
