// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "APInteractItemBase.h"
#include "APInteractResource.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTWORLD_API AAPInteractResource : public AAPInteractItemBase
{
	GENERATED_BODY()
	
protected:
	float m_MaxHealth;
	float m_CurrentHealth;

public:
	void GetDamage(float _dmg);
	void SpawnItems();
};
