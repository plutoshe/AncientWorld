// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/BuildingBlockActor.h"

// Sets default values
ABuildingBlockActor::ABuildingBlockActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void ABuildingBlockActor::SetIndex(int buildingIndex, FIntVector positionIndex)
{
	m_buildingIndex = buildingIndex;
	m_positionIndex = positionIndex;
}
