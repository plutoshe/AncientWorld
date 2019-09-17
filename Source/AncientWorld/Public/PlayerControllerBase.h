// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

/**
 *
 */
UCLASS()
class ANCIENTWORLD_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerBase();


protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	virtual void OnSpacePressed();
	virtual void OnLeftCtrlPressed();
	virtual void OnQPressed();
	virtual void OnEPressed();
	virtual void OnLeftMouseClick();
	virtual void OnRightMouseClick();
	virtual void OnZPressed();

	virtual void OnNum1Pressed();
	virtual void OnNum2Pressed();
	virtual void OnNum3Pressed();
	virtual void OnNum4Pressed();
	virtual void OnNum5Pressed();
	virtual void OnNum6Pressed();
	virtual void OnNum7Pressed();

	virtual void VertialAxis(float axis);
	virtual void HorizontalAxis(float axis);
	virtual void NormalAxis(float axis);
};
