// Fill out your copyright notice in the Description page of Project Settings.


#include "AncientWorldGameInstance.h"
const int32 DirectionRotationUtility::m_directionOffset[4][2] = { {1,1},{-1,1 } ,{-1,-1 } ,{1,-1} };

UAncientWorldGameInstance::UAncientWorldGameInstance()
{
	for (int i = 0; i < m_buildings.Num(); i++)
	{
		m_buildings[i].UpdateMaxXY();
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