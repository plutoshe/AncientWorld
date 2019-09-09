// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AncientWorldGameInstance.generated.h"

/**
 * 
 */

static class DirectionRotationUtility
{
public:
	static const int32 m_directionOffset[4][3];
	static FIntVector GetRealOccupation(FIntVector occupation, int i) {
		return FIntVector(
			occupation.X * m_directionOffset[i][0],
			occupation.Y * m_directionOffset[i][1],
			occupation.Z);
	}
	static FRotator GetRotationByDirectionID(int directionID)
	{
		return FRotator(0, m_directionOffset[directionID][2], 0);
	}
};

USTRUCT(BlueprintType)
struct FBuildingBlock
{
	GENERATED_BODY()
public:
	FBuildingBlock() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMesh* m_mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UMaterial*> m_materials;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FIntVector> m_occupations;
	int m_maxX, m_maxY;
	void UpdateMaxXY()
	{
		m_maxY = 0;
		m_maxX = 0;
		for (int i = 0; i < m_occupations.Num(); i++)
		{
			m_maxX = FMath::Max(m_occupations[i].X, m_maxX);
			m_maxY = FMath::Max(m_occupations[i].Y, m_maxY);
		}
	}
};

USTRUCT(BlueprintType)
struct FBuildingStatus
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_buildingBlockIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FIntVector m_positionIndexForBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_direction;
	FBuildingStatus(int buildingBlockIndex, FIntVector postionIndex, int direction):
		m_buildingBlockIndex(buildingBlockIndex), m_positionIndexForBase(postionIndex), m_direction(direction)
	{}
	FBuildingStatus() {}
};

UCLASS()
class ANCIENTWORLD_API UAncientWorldGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UAncientWorldGameInstance();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FBuildingBlock> m_buildings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterial* m_materialOnBuild;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FBuildingStatus> m_baseStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_BasePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_BaseLayerLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_ModelScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform m_StoredPlayerTransfrom;
	FVector GetModelScale();
	FVector GetBuildingLayerLength();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_PivotIndexOffset;

	FVector GetBuildingPositoinFromIndex(FVector m_basePoint, FIntVector index, int buildingIndex, int directionID)
	{
		FVector result = m_basePoint + FVector(index) * m_BaseLayerLength;
		result.X += (DirectionRotationUtility::m_directionOffset[directionID][0] < 0) * m_BaseLayerLength.X;
		result.Y += (DirectionRotationUtility::m_directionOffset[directionID][1] < 0) * m_BaseLayerLength.Y;
		result.X += m_buildings[buildingIndex].m_maxX * m_BaseLayerLength.X;
		result.Y += m_buildings[buildingIndex].m_maxY * m_BaseLayerLength.Y;
		result.X += m_PivotIndexOffset.X * m_BaseLayerLength.X;
		result.Y += m_PivotIndexOffset.Y * m_BaseLayerLength.Y;
		return result;
	}
};
