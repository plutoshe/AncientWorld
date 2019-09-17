// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AncientWorldGameMode.generated.h"

UENUM(BlueprintType)
enum class EModeEnum : uint8
{
	ME_Exploration 	UMETA(DisplayName = "Exploration"),
	ME_Building 	UMETA(DisplayName = "Building")
};

UCLASS(minimalapi)
class AAncientWorldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAncientWorldGameMode();

	class UDataTable* GetItemDB() const { return m_InventoryItemDB; }
	TArray<TSubclassOf<class AAPToolBase>> GetSpawnToolList() const { return m_ToolClassesToSpawn; }



protected:
	UPROPERTY(EditDefaultsOnly, Category = "InventorySystem")
		TArray<TSubclassOf<class AAPToolBase>> m_ToolClassesToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "InventorySystem")
		class UDataTable* m_InventoryItemDB;

#pragma region BuildingSystem
public:
	UFUNCTION(BlueprintCallable, Category = "BuilidingSystem")
		FORCEINLINE bool InBuildingMode() const { return m_inBuildingMode; }

	UFUNCTION(BlueprintCallable, Category = "BuilidingSystem")
		void SwitchToMode(EModeEnum mode);

	UPROPERTY(VisibleAnywhere, Category = "BuilidingSystem")
		EModeEnum m_CurrentMode;

protected:
	bool m_inBuildingMode;

	bool FindBuildingManager();

	UPROPERTY(EditDefaultsOnly, Category = "BuilidingSystem")
		TSubclassOf<class AAncientWorldCharacter> m_venturePawnClass;
	UPROPERTY(EditDefaultsOnly, Category = "BuilidingSystem")
		TSubclassOf<class ABuildingSystemPawn> m_buildingPawnClass;

	class ABuildingSynchronization* m_buildingManager;
	class AAncientWorldCharacter* m_cachedVenturePawn;
	class ABuildingSystemPawn* m_cachedBuildingPawn;

	virtual void BeginPlay() override;

#pragma endregion

};



