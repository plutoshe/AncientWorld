// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "VenturePawn.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTWORLD_API AVenturePawn : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AVenturePawn();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return CameraComp; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	FORCEINLINE class UInventoryComponent* GetInventoryComponent() { return InventoryComp; }


	FORCEINLINE  FVector GetInteractPointLocation() { return InteractPointComp->GetComponentLocation(); }
	FORCEINLINE  FRotator GetInteractPointRotation() { return InteractPointComp->GetComponentRotation(); }

protected:
	virtual void BeginPlay() override;
#pragma region PlayerInputFunctions
	void MoveForward(float axis);
	void VertialAxis(float axis) override;
	void MoveRight(float axis);
	void HorizontalAxis(float axis) override;
	void RotateCamera90Clockwise();
	void OnQPressed() override;
	void RotateCamera90CounterClockwise();
	void OnEPressed() override;
	void Jump();
	void OnSpacePressed() override;
	UFUNCTION(BlueprintImplementableEvent, Category = Swim)
	void BPSwimNormal(float axis);
	void NormalAxis(float axis);
#pragma endregion

	bool m_bRotating;
	FRotator m_DestRotator;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
		float m_CameraRotateSpeed;

	void PerformCameraRotation(float DeltaSeconds);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* InventoryComp;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComp;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* InteractPointComp;
};
