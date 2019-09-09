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
	virtual void Tick(float DeltaTime) override;

	ABuildingBlockActor();
	void SetIndex(int buildingEntityIndex, FIntVector indexPosition, int directionID);
	void SetBuildingEntityIndex(int buildingEntityIndex);
	void SetIndexPosition(FIntVector indexPosition);
	void SetDirectionID(int directionID);
	void UpdateLocation(class UAncientWorldGameInstance* gameinstance, FVector m_basePoint);
	void UpdateRotation();
	void UpdateDirectionIDOffset(int offset);

	FIntVector m_IndexPosition;
	int m_BuildingEntityId;
	int m_DirectionID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
