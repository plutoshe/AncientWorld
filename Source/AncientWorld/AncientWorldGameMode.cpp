// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AncientWorldGameMode.h"
#include "AncientWorldPlayerController.h"
#include "AncientWorldCharacter.h"
#include "BuildingSystemPawn.h"
#include "BuildingSynchronization.h"	
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "EngineUtils.h"

AAncientWorldGameMode::AAncientWorldGameMode()
{
	// use our custom PlayerController class

	m_inBuildingMode = false;
}

void AAncientWorldGameMode::SwitchToMode(EModeEnum mode)
{
	switch (mode)
	{
	case EModeEnum::ME_Exploration:
		if (mode != m_CurrentMode) {
			m_CurrentMode = mode;
			// Switch to exploration mode

			if (m_cachedVenturePawn == nullptr) {
				m_cachedVenturePawn = GetWorld()->SpawnActor<AAncientWorldCharacter>(m_venturePawnClass);
			}

			APlayerController* controller = GetWorld()->GetFirstPlayerController();
			controller->UnPossess();
			controller->Possess(m_cachedVenturePawn);
			UE_LOG(LogTemp, Warning, TEXT("Go to Exploration Mode"));
		}
		break;
	case EModeEnum::ME_Building:
		if (mode != m_CurrentMode) {
			m_CurrentMode = mode;
			// Switch to building mode
			
			// if it is a nullptr
			if (m_cachedBuildingPawn == nullptr) {
				m_cachedBuildingPawn = GetWorld()->SpawnActor<ABuildingSystemPawn>(m_buildingPawnClass);
			}

			APlayerController* controller = GetWorld()->GetFirstPlayerController();
			controller->UnPossess();
			controller->Possess(m_cachedBuildingPawn);
			m_cachedBuildingPawn->m_BuildingSystem = m_buildingManager;

			m_cachedBuildingPawn->SetActorLocation(m_buildingManager->GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Go to Building Mode"));
		}
		break;
	default:
		break;
	}
}

bool AAncientWorldGameMode::FindBuildingManager()
{
	if (m_buildingManager == nullptr) {
		for (TActorIterator<ABuildingSynchronization> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ABuildingSynchronization* buildingMan = Cast<ABuildingSynchronization>(*ActorItr);
			if (buildingMan) {
				m_buildingManager = buildingMan;
				return true;
			}
		}
		return false;
	}
	else return true;
}

void AAncientWorldGameMode::BeginPlay()
{
	Super::BeginPlay();

	m_cachedVenturePawn = Cast<AAncientWorldCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!FindBuildingManager()) UE_LOG(LogTemp, Warning, TEXT("No Building Manager Found"));
}
