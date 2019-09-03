// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AncientWorldCharacter.generated.h"

UCLASS(Blueprintable)
class AAncientWorldCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAncientWorldCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return CameraComp; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

protected:	
#pragma region PlayerInputFunctions
	void MoveForward(float axis);
	void MoveRight(float axis);
	void RotateCamera90Clockwise();
	void RotateCamera90CounterClockwise();
	void ChangeToBuildingSystem();
	void Jump();
	void Crouch();
	void UnCrouch();
#pragma endregion

	bool m_bRotating;
	FRotator m_DestRotator;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float m_CameraRotateSpeed;

	void PerformCameraRotation(float DeltaSeconds);

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

