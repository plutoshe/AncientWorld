// Fill out your copyright notice in the Description page of Project Settings.


#include "AncientWorldGameInstance.h"
const int32 DirectionRotationUtility::m_directionAngle[4] = { 0, 90, 180, 270 };

UAncientWorldGameInstance::UAncientWorldGameInstance()
{
	for (int i = 0; i < m_BuildingEntities.Num(); i++)
	{
		m_BuildingEntities[i].UpdateMaxXY();
	}
}


FVector UAncientWorldGameInstance::GetModelScale()
{
	return m_ModelScale;
}

FVector UAncientWorldGameInstance::GetBuildingLayerLength()
{
	return m_BaseLayerLength;
}