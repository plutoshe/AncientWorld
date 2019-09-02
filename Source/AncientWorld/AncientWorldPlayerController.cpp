// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AncientWorldPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "AncientWorldCharacter.h"
#include "Engine/World.h"
#include "Public/APInteractItemBase.h"

#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"

AAncientWorldPlayerController::AAncientWorldPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AAncientWorldPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AAncientWorldPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);


}

void AAncientWorldPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &AAncientWorldPlayerController::OnMouseClick);
	InputComponent->BindAction("Item1", IE_Pressed, this, &AAncientWorldPlayerController::SwitchToItem1);
	InputComponent->BindAction("Item2", IE_Pressed, this, &AAncientWorldPlayerController::SwitchToItem2);
	InputComponent->BindAction("Item3", IE_Pressed, this, &AAncientWorldPlayerController::SwitchToItem3);
	InputComponent->BindAction("Item4", IE_Pressed, this, &AAncientWorldPlayerController::SwitchToItem4);

}


void AAncientWorldPlayerController::OnMouseClick()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		SetNewMoveDestination(Hit);
	}

}

void AAncientWorldPlayerController::SetNewMoveDestination(const FHitResult& outHit)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		AAPInteractItemBase* interactBase = Cast<AAPInteractItemBase>(outHit.Actor);
		if (interactBase) {
			float const Distance = FVector::Dist(outHit.ImpactPoint, MyPawn->GetActorLocation());

			if ((Distance > 120.0f))
			{
				if (!interactBase->GetCanPawnInteract())
					MoveToLocation(this, outHit.ImpactPoint);
				//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, outHit.ImpactPoint);
			}

			if (interactBase->GetCanPawnInteract()) {
				interactBase->Interact();
			}
		}


	}
}


UPathFollowingComponent* AAncientWorldPlayerController::InitNavigationControl(AController& Controller)
{

	AAIController* AsAIController = Cast<AAIController>(&Controller);
	UPathFollowingComponent* PathFollowingComp = nullptr;

	if (AsAIController)
	{
		PathFollowingComp = AsAIController->GetPathFollowingComponent();
	}
	else
	{
		PathFollowingComp = Controller.FindComponentByClass<UPathFollowingComponent>();
		if (PathFollowingComp == nullptr)
		{
			PathFollowingComp = NewObject<UPathFollowingComponent>(&Controller);
			PathFollowingComp->RegisterComponentWithWorld(Controller.GetWorld());
			PathFollowingComp->Initialize();
		}
	}

	return PathFollowingComp;

}

void AAncientWorldPlayerController::MoveToLocation(AController* Controller, const FVector& GoalLocation)
{
	UNavigationSystemV1* NavSys = Controller ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(Controller->GetWorld()) : nullptr;
	if (NavSys == nullptr || Controller == nullptr || Controller->GetPawn() == nullptr)
	{
		UE_LOG(LogNavigation, Warning, TEXT("UNavigationSystemV1::SimpleMoveToActor called for NavSys:%s Controller:%s controlling Pawn:%s (if any of these is None then there's your problem"),
			*GetNameSafe(NavSys), *GetNameSafe(Controller), Controller ? *GetNameSafe(Controller->GetPawn()) : TEXT("NULL"));
		return;
	}

	m_PFollowComp = InitNavigationControl(*Controller);
	UE_LOG(LogTemp, Log, TEXT("init: %d"), m_PFollowComp->bIsActive );

	if (m_PFollowComp == nullptr)
	{
		return;
	}

	if (!m_PFollowComp->IsPathFollowingAllowed())
	{
		return;
	}

	const bool bAlreadyAtGoal = m_PFollowComp->HasReached(GoalLocation, EPathFollowingReachMode::OverlapAgent);

	// script source, keep only one move request at time
	if (m_PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		m_PFollowComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::AnyRequest, bAlreadyAtGoal ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
	}

	// script source, keep only one move request at time
	if (m_PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		m_PFollowComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
	}

	if (bAlreadyAtGoal)
	{
		m_PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
	}
	else
	{
		const ANavigationData* NavData = NavSys->GetNavDataForProps(Controller->GetNavAgentPropertiesRef());
		if (NavData)
		{
			FPathFindingQuery Query(Controller, *NavData, Controller->GetNavAgentLocation(), GoalLocation);
			FPathFindingResult Result = NavSys->FindPathSync(Query);
			if (Result.IsSuccessful())
			{
				m_PFollowComp->RequestMove(FAIMoveRequest(GoalLocation), Result.Path);
			}
			else if (m_PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
			{
				m_PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
			}
		}
	}
}

void AAncientWorldPlayerController::CancelMoveToLocation()
{
	if (m_PFollowComp && m_PFollowComp->GetStatus() == EPathFollowingStatus::Moving)
		m_PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Aborted);
}

void AAncientWorldPlayerController::SwitchToItem1()
{
	AAncientWorldCharacter* myCharacter = Cast<AAncientWorldCharacter>(GetPawn());
	if (myCharacter) {
		myCharacter->SwitchToItem(0);
	}
}

void AAncientWorldPlayerController::SwitchToItem2()
{
	AAncientWorldCharacter* myCharacter = Cast<AAncientWorldCharacter>(GetPawn());
	if (myCharacter) {
		myCharacter->SwitchToItem(1);

	}
}

void AAncientWorldPlayerController::SwitchToItem3()
{
	AAncientWorldCharacter* myCharacter = Cast<AAncientWorldCharacter>(GetPawn());
	if (myCharacter) {
		myCharacter->SwitchToItem(2);
	}
}

void AAncientWorldPlayerController::SwitchToItem4()
{
	AAncientWorldCharacter* myCharacter = Cast<AAncientWorldCharacter>(GetPawn());
	if (myCharacter) {
		myCharacter->SwitchToItem(3);
	}
}
