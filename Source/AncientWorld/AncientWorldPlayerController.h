// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AncientWorldPlayerController.generated.h"

UCLASS()
class AAncientWorldPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAncientWorldPlayerController();

protected:

	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void OnMouseClick();

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FHitResult& outHit);

	// Navigation component
	class UPathFollowingComponent* m_PFollowComp;

public:

	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	void MoveToLocation(AController* Controller, const FVector& GoalLocation);
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
		void CancelMoveToLocation();

	class UPathFollowingComponent* InitNavigationControl(AController& Controller);
};


