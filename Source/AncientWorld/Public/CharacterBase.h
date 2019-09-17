// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class ANCIENTWORLD_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void OnSpacePressed() {};
	virtual void OnLeftCtrlPressed() {};
	virtual void OnQPressed() {};
	virtual void OnEPressed() {};
	virtual void OnLeftMouseClick() {};
	virtual void OnRightMouseClick() {};
	virtual void OnZPressed() {};

	virtual void OnNum1Pressed() {};
	virtual void OnNum2Pressed() {};
	virtual void OnNum3Pressed() {};
	virtual void OnNum4Pressed() {};
	virtual void OnNum5Pressed() {};
	virtual void OnNum6Pressed() {};
	virtual void OnNum7Pressed() {};

	virtual void VertialAxis(float axis) {};
	virtual void HorizontalAxis(float axis) {};
	virtual void NormalAxis(float axis) {};

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
