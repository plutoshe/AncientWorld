// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AncientWorldGameMode.h"
#include "AncientWorldPlayerController.h"
#include "AncientWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAncientWorldGameMode::AAncientWorldGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAncientWorldPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownTemplate/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}