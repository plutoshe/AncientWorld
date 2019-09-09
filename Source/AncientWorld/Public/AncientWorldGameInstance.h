// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AncientWorldGameInstance.generated.h"

static class DirectionRotationUtility
{
public:
	static const int32 m_directionAngle[4];
	static FIntVector GetRealOccupation(FIntVector occupation, int i) {
		switch (i)
		{
		case 0:
			return FIntVector(
				occupation.X,
				occupation.Y,
				occupation.Z);
		case 1:
			return FIntVector(
				-occupation.Y,
				occupation.X,
				occupation.Z);
		case 2:
			return FIntVector(
				occupation.X * -1,
				occupation.Y * -1,
				occupation.Z);

		case 3:
			return FIntVector(
				occupation.Y,
				-occupation.X,
				occupation.Z);
		}
		return FIntVector(
			occupation.X,
			occupation.Y,
			occupation.Z);
	}
	
	static FRotator GetRotationByDirectionID(int directionID)
	{
		return FRotator(0, m_directionAngle[directionID], 0);
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
	FVector GetMaxOffset(int directionID)
	{
		switch (directionID)
		{
		case 0:
			return FVector(0, 0, 0);
		case 1:
			return FVector(m_maxY + 1, 0, 0);
		case 2:
			return FVector(m_maxX + 1, m_maxY + 1, 0);
		case 3:
			return FVector(0, m_maxX + 1, 0);
		}
		return FVector(0, 0, 0);
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
		TArray<FBuildingBlock> m_BuildingEntities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterial* m_MaterialOnBuildSuccess;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterial* m_MaterialOnBuildFailure;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FBuildingStatus> m_BaseBuildingStatus;
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
		//result.X += (DirectionRotationUtility::m_directionOffset[directionID][0] < 0) * m_BaseLayerLength.X;
		//result.Y += (DirectionRotationUtility::m_directionOffset[directionID][1] < 0) * m_BaseLayerLength.Y;
		result.X += m_BuildingEntities[buildingIndex].GetMaxOffset(directionID).X * m_BaseLayerLength.X;
		result.Y += m_BuildingEntities[buildingIndex].GetMaxOffset(directionID).Y * m_BaseLayerLength.Y;
		UE_LOG(LogTemp, Log, TEXT("OFFSET: %s"), *(m_BuildingEntities[buildingIndex].GetMaxOffset(directionID).ToString()));
		result.X += m_PivotIndexOffset.X * m_BaseLayerLength.X;
		result.Y += m_PivotIndexOffset.Y * m_BaseLayerLength.Y;
		return result;
	}

	FVector GetPositoinFromIndex(FVector m_basePoint, FIntVector index, int buildingIndex, int directionID)
	{
		FVector result = m_basePoint + FVector(index) * m_BaseLayerLength;
		result.X += m_PivotIndexOffset.X * m_BaseLayerLength.X;
		result.Y += m_PivotIndexOffset.Y * m_BaseLayerLength.Y;
		// for mouse pivot
		result.X += 0.5 * m_BaseLayerLength.X;
		result.Y += 0.5 * m_BaseLayerLength.Y;
		return result;
	}
};
