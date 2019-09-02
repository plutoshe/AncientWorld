// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BuildingSystemPawn.generated.h"

UCLASS()
class ANCIENTWORLD_API ABuildingSystemPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	void ChangeToBuildingSystem();
	void MoveUp();
	void MoveDown();
	void MoveForBuilding(int direction);
	ABuildingSystemPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoon;
private:
	FVector MoveCameraDst;
	FVector MoveCameraSrc;
	float MoveTimeSpan;
	float MoveSpendTime;
};
