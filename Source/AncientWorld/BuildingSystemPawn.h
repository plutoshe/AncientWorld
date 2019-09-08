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

	UFUNCTION(BlueprintCallable, Category = "MyCategory")
		void BuildCancellation();
	
	void ChangeToBuildingSystem();
	void MoveUp();
	void MoveDown();
	void MoveForBuilding(int direction);
	void BuildAction();
	void BuildComplete();
	
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

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		class ABuildingSynchronization* m_buildingsystem;

private:
	class AStaticMeshActor* m_BuildingBlock;
	class UAncientWorldGameInstance* m_gameStateInstance;
	APlayerController* m_PlayerController;

	FVector m_MoveCameraDst;
	FVector m_MoveCameraSrc;

	float m_MoveTimeSpan;
	float m_MoveRemainingTime;	
	float m_LayerLength;
	int m_select;
};

