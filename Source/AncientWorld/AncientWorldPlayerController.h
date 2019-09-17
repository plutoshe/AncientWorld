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

public:

};


