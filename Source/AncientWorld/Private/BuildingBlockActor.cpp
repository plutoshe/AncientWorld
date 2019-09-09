// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/BuildingBlockActor.h"
#include "Public/AncientWorldGameInstance.h"
// Sets default values
ABuildingBlockActor::ABuildingBlockActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

// Called when the game starts or when spawned
void ABuildingBlockActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingBlockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingBlockActor::SetIndex(int buildingIndex, FIntVector positionIndex, int directionID)
{
	m_buildingIndex = buildingIndex;
	m_positionIndex = positionIndex;
	m_directionID = directionID;
}

void ABuildingBlockActor::UpdateLocation(class UAncientWorldGameInstance* gameinstance, FVector m_basePoint)
{
	SetActorLocation(gameinstance->GetBuildingPositoinFromIndex(m_basePoint, m_positionIndex, m_buildingIndex, m_directionID));
	
}

void ABuildingBlockActor::UpdateRotation()
{
	SetActorRotation(DirectionRotationUtility::GetRotationByDirectionID(m_directionID));
}

void ABuildingBlockActor::UpdateDirectionIDOffset(int offset)
{
	m_directionID += offset;
	if (m_directionID < 0)
		m_directionID += 4;
	m_directionID %= 4;
	UpdateRotation();
}

void ABuildingBlockActor::SetDirectionID(int id)
{
	m_directionID = id;
	UpdateRotation();
}

