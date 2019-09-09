// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Classes/Engine/StaticMeshActor.h"
#include "BuildingBlockActor.generated.h"

UCLASS()
class ANCIENTWORLD_API ABuildingBlockActor : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingBlockActor();
	void SetIndex(int buildingIndex, FIntVector positionIndex, int directionID);
	int m_buildingIndex;
	FIntVector m_positionIndex;
	int m_directionID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateLocation(class UAncientWorldGameInstance* gameinstance, FVector m_basePoint);
	void UpdateRotation();
	void SetDirectionID(int id);
	void UpdateDirectionIDOffset(int offset);
};
