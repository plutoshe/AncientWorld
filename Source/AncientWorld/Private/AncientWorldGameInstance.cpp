// Fill out your copyright notice in the Description page of Project Settings.


#include "AncientWorldGameInstance.h"

UAncientWorldGameInstance::UAncientWorldGameInstance()
{
	m_currentSelectBuildingBlockID = 0;

}

FBuildingBlock* UAncientWorldGameInstance::GetCurrentBuildingBlock()
{
	if (m_currentSelectBuildingBlockID < m_buildings.Num())
	{
		return &m_buildings[m_currentSelectBuildingBlockID];
	}

	return nullptr;
}