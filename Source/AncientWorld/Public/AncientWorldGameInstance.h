// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AncientWorldGameInstance.generated.h"

/**
 * 
 */

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
	FBuildingStatus(int buildingBlockIndex, FIntVector postionIndex):
		m_buildingBlockIndex(buildingBlockIndex), m_positionIndexForBase(postionIndex)
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
		int m_currentSelectBuildingBlockID;
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
	FBuildingBlock* GetCurrentBuildingBlock();
	FVector GetModelScale();
	FVector GetBuildingLayerLength();
	int GetCurrentBuildingBlockID();
};
