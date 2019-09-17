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

	UFUNCTION(BlueprintCallable, Category = "BuilidingSystem")
		void BuildCancellation();
	UFUNCTION(BlueprintCallable, Category = "BuilidingSystem")
		void ChangeCurrentBuildingBlockEntityID(int id);
	
	UFUNCTION(BlueprintCallable, Category = "BuilidingSystem")
	void BuildAction();
	void BuildComplete();
	UFUNCTION(BlueprintCallable, Category = "BuilidingSystem")
	void MoveUp();
	UFUNCTION(BlueprintCallable, Category = "BuilidingSystem")
	void MoveDown();
	void MoveForBuilding(int direction);
	void RotateBuildingCamera(float axis);
	void RoatetForward();
	void RotateBackword();
	void UpdateCamera();
	void UpdateCurrentBuildingBlock();

	UFUNCTION(BlueprintCallable, Category = "BuilidingSystem")
	void ChangeToBuilding(int idx);

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

#pragma region BuildingDebug
	void ChangeToBuilding1();
	void ChangeToBuilding2();
	void ChangeToBuilding3();
	void ChangeToBuilding4();
#pragma endregion





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

