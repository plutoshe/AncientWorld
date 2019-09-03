// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/APResources.h"
#include "AncientWorldCharacter.h"
#include "Public/APPickUP.h"
#include "Engine/World.h"
#include "AncientWorldCharacter.h"
#include "Components/StaticMeshComponent.h"
AAPResources::AAPResources()
{
	m_XYOffset = 1;
}

void AAPResources::BeginPlay()
{
	Super::BeginPlay();
	m_CurrentHealth = m_MaxHealth;

}

void AAPResources::GetDamage(float _dmg)
{
	m_CurrentHealth -= _dmg;
	if (m_CurrentHealth <= 0) {
		m_CurrentHealth = 0;

		SpawnItems();
		Destroy();
	}
}

void AAPResources::SpawnItems()
{
	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn rewards
	for (TSubclassOf<class AAPPickUP> pickUp : m_spawnPickList)
	{
		FVector RndDirection = FMath::VRand();
		RndDirection.Z = 0;
		AAPPickUP* sp = GetWorld()->SpawnActor<AAPPickUP>(pickUp, GetActorLocation()+ RndDirection* m_XYOffset, GetActorRotation(), ActorSpawnParams);


		//sp->GetStaticMesh()->AddImpulse(RndDirection * m_RndImpulse, NAME_None, true);
	}
}

void AAPResources::Interact()
{
	Super::Interact();

	// it is a required tool
	if (true || m_requiredItemID == m_InteratingActor->GetCurrentItem()->ItemID) {
		GetDamage(10);
		//spawnedActor = World->SpawnActor<AMyActor>(AMyActor::StaticClass(), location, rotate, SpawnInfo);


	}
}
