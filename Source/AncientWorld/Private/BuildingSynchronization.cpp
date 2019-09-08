// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/BuildingSynchronization.h"
#include "Engine/Classes/Engine/StaticMeshActor.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Classes/Engine/StaticMesh.h"
#include "AncientWorldGameInstance.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Materials/Material.h"

// Sets default values
ABuildingSynchronization::ABuildingSynchronization()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
FVector ABuildingSynchronization::GetCurrentSelectLocation()
{
	return GetBuildingPositoinFromIndex(m_BuildingSlot[m_select]);
}

FVector ABuildingSynchronization::GetBuildingPositoinFromIndex(FVector index)
{
	return basePoint + index * m_gameStateInstance->m_BaseLayerLength;
}

void ABuildingSynchronization::InitialBlock(AStaticMeshActor* newBlock, int buildingId, bool setMaterial = true)
{
	newBlock->SetMobility(EComponentMobility::Movable);
	newBlock->SetActorScale3D(m_gameStateInstance->GetModelScale());

	newBlock->GetStaticMeshComponent()->SetStaticMesh(m_gameStateInstance->m_buildings[buildingId].m_mesh);
	if (setMaterial)
	{
		for (int i = 0; i < m_gameStateInstance->m_buildings[buildingId].m_materials.Num(); i++)
		{
			newBlock->GetStaticMeshComponent()->SetMaterial(i, m_gameStateInstance->m_buildings[buildingId].m_materials[i]);
		}
	}
	

}

// Called when the game starts or when spawned
void ABuildingSynchronization::BeginPlay()
{
	Super::BeginPlay();
	m_gameStateInstance = static_cast<UAncientWorldGameInstance*>(UGameplayStatics::GetGameInstance(GetWorld()));
	basePoint = GetActorLocation();
	m_BuildingSlot.Init(FVector(0, 0, -1000), 2);
	m_BuildingSlot[0] = FVector(0, 0, -1000);
	m_BuildingSlot[1] = FVector(0, 0, 1000);
	for (int i = 0; i < m_gameStateInstance->m_baseStatus.Num(); i++)
	{
		AStaticMeshActor* newBlock = static_cast<AStaticMeshActor*>(GetWorld()->SpawnActor(AStaticMeshActor::StaticClass()));
		InitialBlock(newBlock, m_gameStateInstance->m_baseStatus[i].m_buildingBlockIndex);
		newBlock->SetActorLocation(GetBuildingPositoinFromIndex(m_gameStateInstance->m_baseStatus[i].m_positionIndexForBase));
		m_buildings.Add(newBlock);
		m_BuildingSlot[0] = FVector(0, 0, FMath::Max(m_BuildingSlot[0].Z, m_gameStateInstance->m_baseStatus[i].m_positionIndexForBase.Z));
		m_BuildingSlot[1] = FVector(0, 0, FMath::Min(m_BuildingSlot[1].Z, m_gameStateInstance->m_baseStatus[i].m_positionIndexForBase.Z));
	}
	m_BuildingSlot[0].Z += 1;
	m_BuildingSlot[1].Z -= 1;
}

// Called every frame
void ABuildingSynchronization::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABuildingSynchronization::ConfirmBuilding()
{

	AStaticMeshActor* newBlock = static_cast<AStaticMeshActor*>(GetWorld()->SpawnActor(AStaticMeshActor::StaticClass()));
	InitialBlock(newBlock, m_gameStateInstance->GetCurrentBuildingBlockID());
	m_buildings.Add(newBlock);
	m_gameStateInstance->m_baseStatus.Add(FBuildingStatus(m_gameStateInstance->GetCurrentBuildingBlockID(), m_BuildingSlot[m_select]));
	switch (m_select)
	{
	case 0: m_BuildingSlot[0].Z += 1; break;
	case 1: m_BuildingSlot[1].Z -= 1; break;
	}
}

FVector ABuildingSynchronization::ReturnSelectedPosition(FVector mousePosition)
{
	float minD = -1;
	m_select = -1;
	for (int i = 0; i < m_BuildingSlot.Num(); i++)
	{
		FVector select_position = GetBuildingPositoinFromIndex(m_BuildingSlot[i]);
		if (m_select == -1 || FVector::Distance(select_position, mousePosition) < minD)
		{
			minD = FVector::Distance(select_position, mousePosition);
			m_select = i;
		}
	}
	
	return GetBuildingPositoinFromIndex(m_BuildingSlot[m_select]);

}


