// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/BuildingSynchronization.h"
#include "Engine/Classes/Engine/StaticMeshActor.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Classes/Engine/StaticMesh.h"
#include "AncientWorldGameInstance.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Materials/Material.h"
#include "Public/BuildingBlockActor.h"
#include "Public/AncientWorldGameInstance.h"


FBuildingBlock* ABuildingSynchronization::GetCurrentBuildingBlock()
{
	if (m_currentSelectBuildingBlockID < m_buildings.Num())
	{
		return &m_GameStateInstance->m_buildings[m_currentSelectBuildingBlockID];
	}

	return nullptr;
}

int ABuildingSynchronization::GetCurrentBuildingBlockID()
{
	return m_currentSelectBuildingBlockID;
}
// Sets default values
ABuildingSynchronization::ABuildingSynchronization()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABuildingSynchronization::InitialBlockByBuildingID(ABuildingBlockActor* newBlock, int buildingId, bool setMaterial = true)
{
	newBlock->SetActorScale3D(m_GameStateInstance->GetModelScale());
	newBlock->GetStaticMeshComponent()->SetStaticMesh(m_GameStateInstance->m_buildings[buildingId].m_mesh);

	if (setMaterial)
	{
		for (int i = 0; i < m_GameStateInstance->m_buildings[buildingId].m_materials.Num(); i++)
		{
			newBlock->GetStaticMeshComponent()->SetMaterial(i, m_GameStateInstance->m_buildings[buildingId].m_materials[i]);
		}
	}
	

}

// Called when the game starts or when spawned
void ABuildingSynchronization::BeginPlay()
{
	Super::BeginPlay();
	m_GameStateInstance = static_cast<UAncientWorldGameInstance*>(UGameplayStatics::GetGameInstance(GetWorld()));
	m_basePoint = GetActorLocation();
	if (m_GameStateInstance != nullptr && m_GameStateInstance->m_baseStatus.Num() > 0)
	{
		int32 maxZ = m_GameStateInstance->m_baseStatus[0].m_positionIndexForBase.Z;
		int32 minZ = m_GameStateInstance->m_baseStatus[0].m_positionIndexForBase.Z;
		for (int i = 0; i < m_GameStateInstance->m_baseStatus.Num(); i++)
		{
			maxZ = FMath::Max(maxZ, m_GameStateInstance->m_baseStatus[i].m_positionIndexForBase.Z);
			minZ = FMath::Min(minZ, m_GameStateInstance->m_baseStatus[i].m_positionIndexForBase.Z);
		}
		m_UndergroundConstructionStatus.Init(0, 0);
		m_HorizontalConstructionStatus.Init(0, 0);
		for (int i = -1; i >= minZ - 1; i--)
		{
			m_UndergroundConstructionStatus.Add(FLayerConstructionStatus(i));
		}
		for (int i = 0; i <= maxZ + 1; i++)
		{
			m_HorizontalConstructionStatus.Add(FLayerConstructionStatus(i));
		}

		for (int i = 0; i < m_GameStateInstance->m_baseStatus.Num(); i++)
		{
			ABuildingBlockActor* newBlock = static_cast<ABuildingBlockActor*>(GetWorld()->SpawnActor(ABuildingBlockActor::StaticClass()));
			InitialBlockByBuildingID(newBlock, m_GameStateInstance->m_baseStatus[i].m_buildingBlockIndex);
			newBlock->SetIndex(m_GameStateInstance->m_baseStatus[i].m_buildingBlockIndex, m_GameStateInstance->m_baseStatus[i].m_positionIndexForBase, m_GameStateInstance->m_baseStatus[i].m_direction);
			newBlock->UpdateLocation(m_GameStateInstance, m_basePoint);
			newBlock->UpdateRotation();
			UpdateBuildingStatus(newBlock, true);
			m_buildings.Add(newBlock);
		}
	}
	
}



void ABuildingSynchronization::UpdateBuildingStatus(ABuildingBlockActor* block, bool isAddition)
{
	int value = -1;
	if (!isAddition)
	{
		value = 1;
	}

	for (int i = 0; i < m_GameStateInstance->m_buildings[block->m_buildingIndex].m_occupations.Num(); i++)
	{
		FIntVector currentOccupation = block->m_positionIndex + DirectionRotationUtility::GetRealOccupation(m_GameStateInstance->m_buildings[block->m_buildingIndex].m_occupations[i], block->m_directionID);
		if (currentOccupation.Z >= 0)
		{
			for (int j = m_HorizontalConstructionStatus.Num(); j <= currentOccupation.Z; j++)
			{
				m_HorizontalConstructionStatus.Add(FLayerConstructionStatus(j));
			}
			m_HorizontalConstructionStatus[currentOccupation.Z].SetLayerStatus(currentOccupation.X, currentOccupation.Y, isAddition);
			m_HorizontalConstructionStatus[currentOccupation.Z].m_availableBlock += value;
		}
		else
		{
			currentOccupation.Z = -currentOccupation.Z - 1;
			for (int j = m_UndergroundConstructionStatus.Num(); j <= currentOccupation.Z; j++)
			{
				m_UndergroundConstructionStatus.Add(FLayerConstructionStatus(-j - 1));
			}
			m_UndergroundConstructionStatus[currentOccupation.Z].SetLayerStatus(currentOccupation.X, currentOccupation.Y, isAddition);
			m_UndergroundConstructionStatus[currentOccupation.Z].m_availableBlock += value;
		}
	}
}

// Called every frame
void ABuildingSynchronization::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABuildingSynchronization::ConfirmBuilding(ABuildingBlockActor* newBlock)
{
	//ABuildingBlockActor *newBlock = static_cast<ABuildingBlockActor*>(GetWorld()->SpawnActor(ABuildingBlockActor::StaticClass()));
	InitialBlockByBuildingID(newBlock, GetCurrentBuildingBlockID());
	newBlock->SetIndex(GetCurrentBuildingBlockID(), m_select, newBlock->m_directionID);
	newBlock->UpdateLocation(m_GameStateInstance, m_basePoint);
	newBlock->UpdateRotation();
	UpdateBuildingStatus(newBlock, true);
	m_buildings.Add(newBlock);
	m_GameStateInstance->m_baseStatus.Add(FBuildingStatus(GetCurrentBuildingBlockID(), m_select, newBlock->m_directionID));

}

int ABuildingSynchronization::GetTopIndexZ()
{
	if (m_HorizontalConstructionStatus.Num() == 0  || m_HorizontalConstructionStatus[m_HorizontalConstructionStatus.Num() - 1].m_availableBlock == 0)
	{
		m_HorizontalConstructionStatus.Add(FLayerConstructionStatus(m_HorizontalConstructionStatus.Num()));
	}
	return m_HorizontalConstructionStatus.Num() - 1;
}

int ABuildingSynchronization::GetBottomIndexZ()
{
	if (m_UndergroundConstructionStatus.Num() == 0 || m_UndergroundConstructionStatus[m_UndergroundConstructionStatus.Num() - 1].m_availableBlock == 0)
	{
		m_UndergroundConstructionStatus.Add(FLayerConstructionStatus(-m_UndergroundConstructionStatus.Num() - 1));
	}
	return -m_UndergroundConstructionStatus.Num();
}

float ABuildingSynchronization::GetTopZ()
{
	return m_GameStateInstance->GetBuildingPositoinFromIndex(m_basePoint, FIntVector(0, 0, GetTopIndexZ()) , 0, 0).Z;
}

float ABuildingSynchronization::GetBottomZ()
{
	return m_GameStateInstance->GetBuildingPositoinFromIndex(m_basePoint, FIntVector(0, 0, GetBottomIndexZ()), 0, 0).Z;
}

FVector ABuildingSynchronization::ReturnSelectedPosition(FVector mousePosition, int direction)
{
	float minD = 100000000000;
	m_select = FIntVector(0,0,0);
	int topZ = GetTopIndexZ();
	int bottomZ = -GetBottomIndexZ() - 1;
	for (int i = 0; i < FLayerConstructionStatus::LayerMaxX; i++) {
		for (int j = 0; j < FLayerConstructionStatus::LayerMaxY; j++)
		{
			if (!m_HorizontalConstructionStatus[topZ].GetLayerStatus(i, j))
			{

				FVector select_position = m_GameStateInstance->GetBuildingPositoinFromIndex(m_basePoint, FIntVector(i, j, topZ), GetCurrentBuildingBlockID(), direction);
				if (FVector::Distance(select_position, mousePosition) < minD)
				{
					minD = FVector::Distance(select_position, mousePosition);
					m_select = FIntVector(i, j, topZ);
				}
			}
			if (!m_UndergroundConstructionStatus[bottomZ].GetLayerStatus(i, j))
			{

				FVector select_position = m_GameStateInstance->GetBuildingPositoinFromIndex(m_basePoint, FIntVector(i, j, -bottomZ - 1), GetCurrentBuildingBlockID(), direction);
				if (FVector::Distance(select_position, mousePosition) < minD)
				{
					minD = FVector::Distance(select_position, mousePosition);
					m_select = FIntVector(i, j, -bottomZ - 1);
				}
			}
		}
	}
	
	return m_GameStateInstance->GetBuildingPositoinFromIndex(m_basePoint, m_select, GetCurrentBuildingBlockID(), direction);

}


