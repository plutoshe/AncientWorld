// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "APInteractItemBase.h"
#include "APResources.generated.h"

/**
 *  Base class for some resources
 */
struct FInventoryItem;
UCLASS()
class ANCIENTWORLD_API AAPResources : public AAPInteractItemBase
{
	GENERATED_BODY()

		AAPResources();
protected:

	UPROPERTY(EditAnywhere, Category = "Utils")
	TArray<TSubclassOf<class AAPPickUP>> m_spawnPickList;

	UPROPERTY(EditDefaultsOnly, Category = "Utils")
	FName m_requiredItemID;

	UPROPERTY(EditDefaultsOnly, Category = "Utils")
	float m_MaxHealth;
	float m_CurrentHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Utils")
		float m_XYOffset;

	virtual void BeginPlay() override;

public:
	void GetDamage(float _dmg);
	void SpawnItems();

	virtual void Interact() override;
};
