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


}

