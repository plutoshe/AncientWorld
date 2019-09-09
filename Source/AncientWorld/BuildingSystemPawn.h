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
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "MyCategory")
		void BuildCancellation();
	
	void BuildAction();
	void BuildComplete();
	void MoveUp();
	void MoveDown();
	void MoveForBuilding(int direction);
	void RotateBuildingCamera(float axis);
	void RoatetForward();
	void RotateBackword();
	void UpdateCamera();
	void UpdateCurrentBuildingBlock();

	ABuildingSystemPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopCameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* BottomCameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* TopCameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* BottomCameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Actor, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* OriginPoint;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		class ABuildingSynchronization* m_BuildingSystem;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		float m_InputSensitivity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	class ABuildingBlockActor* m_CurrentBuildingBlock;
	class UAncientWorldGameInstance* m_GameStateInstance;
	class APlayerController* m_PlayerController;

	FVector m_CameraRotationInitialTopPoint, m_CameraRotationInitialBottomPoint;

	float m_CameraAngle;
	float m_LastMouseX;
	float m_LayerLength;
	float m_MoveCameraDstZ;
	
	bool m_IsRotateCamera;
	bool m_isIntialCamera;
	bool m_IsTopCamera;
};

