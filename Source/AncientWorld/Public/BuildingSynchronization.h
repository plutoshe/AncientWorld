// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingSynchronization.generated.h"

USTRUCT(BlueprintType)
struct FLayerConstructionStatus
{
	GENERATED_BODY()
public:
	static const int LayerMaxX = 2;
	static const int LayerMaxY = 2;
	static void initEmptyLayer(TArray<bool> &m_layerStatus)
	{
		m_layerStatus.Init(false, LayerMaxX * LayerMaxY);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_LayerID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<bool> m_layerStatus;
	int m_availableBlock;
	FLayerConstructionStatus()
	{}
	FLayerConstructionStatus(int layerID)
	{
		m_LayerID = layerID;
		initEmptyLayer(m_layerStatus);
		m_availableBlock = LayerMaxX * LayerMaxY;
	}
	void SetLayerStatus(int x, int y, bool status)
	{
		if (x * LayerMaxY + y < 4 && x * LayerMaxY + y >= 0)
		{
			m_layerStatus[x * LayerMaxY + y] = status;
		}
	}
	bool GetLayerStatus(int x, int y)
	{
		return m_layerStatus[x * LayerMaxY + y];
	}
};

UCLASS()
class ANCIENTWORLD_API ABuildingSynchronization : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingSynchronization();
	void ConfirmBuilding(class ABuildingBlockActor* newBlock);
	FIntVector ReturnSelectedIndexPosition(FVector mousePosition, int direction);
	void InitialBlockByBuildingID(class ABuildingBlockActor* newBlock, int buildingId, bool setMaterial);
	void UpdateBuildingStatus(ABuildingBlockActor* block, bool isAddition);
	UFUNCTION(BlueprintCallable, Category = "BuildingSystem")
		int GetHighestLayer();

	int GetTopIndexZ();
	int GetBottomIndexZ();
	float GetTopZ();
	float GetBottomZ();
	UFUNCTION(BlueprintCallable, Category = "BuilidingSystem")
		FVector GetHorizontalCenter();
	bool BuildingAvailability(ABuildingBlockActor& block);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	class UAncientWorldGameInstance* m_GameStateInstance;
	TArray<class ABuildingBlockActor*> m_BuildingEntities;
	FVector m_basePoint;

	
	TArray<FLayerConstructionStatus> m_UndergroundConstructionStatus;
	TArray<FLayerConstructionStatus> m_HorizontalConstructionStatus;
	

	struct FBuildingBlock* GetCurrentBuildingBlock();
	int GetBuildingBlockCurrentBuildingEntityID();
	void SetBuildingBlockCurrentBuildingEntityID(int id);
	// Building Block Attributes:
	// 1. building entity
	// 2. building index position
	// 3. rotation direction stored in the building block
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_BuildingBlockCurrentBuildingEntityID;
	FIntVector m_BuildingBlockCurrentIndexPosition;
};
