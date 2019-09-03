// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AncientWorldGameMode.generated.h"

UCLASS(minimalapi)
class AAncientWorldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAncientWorldGameMode();

	class UDataTable* GetItemDB() const { return m_InventoryItemDB; }
	TArray<TSubclassOf<class AAPToolBase>> GetSpawnToolList() const { return m_ToolClassesToSpawn; }

protected:
	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<class AAPToolBase>> m_ToolClassesToSpawn;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* m_InventoryItemDB;
};



