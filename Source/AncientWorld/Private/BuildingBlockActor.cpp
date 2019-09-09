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

void ABuildingBlockActor::SetIndex(int buildingEntityIndex, FIntVector indexPosition, int directionID)
{
	m_BuildingEntityId = buildingEntityIndex;
	m_IndexPosition = indexPosition;
	m_DirectionID = directionID;
}

void ABuildingBlockActor::UpdateLocation(class UAncientWorldGameInstance* gameinstance, FVector m_basePoint)
{
	SetActorLocation(gameinstance->GetBuildingPositoinFromIndex(m_basePoint, m_IndexPosition, m_BuildingEntityId, m_DirectionID));
	
}

void ABuildingBlockActor::UpdateRotation()
{
	SetActorRotation(DirectionRotationUtility::GetRotationByDirectionID(m_DirectionID));
}

void ABuildingBlockActor::UpdateDirectionIDOffset(int offset)
{
	m_DirectionID += offset;
	if (m_DirectionID < 0)
		m_DirectionID += 4;
	m_DirectionID %= 4;
	UpdateRotation();
}

void ABuildingBlockActor::SetDirectionID(int directionID)
{
	m_DirectionID = directionID;
	UpdateRotation();
}

void ABuildingBlockActor::SetBuildingEntityIndex(int buildingEntityIndex)
{
	m_BuildingEntityId = buildingEntityIndex;
}

void ABuildingBlockActor::SetIndexPosition(FIntVector indexPosition)
{
	m_IndexPosition = indexPosition;
}


