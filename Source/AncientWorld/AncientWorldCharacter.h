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

	FORCEINLINE class UInventoryComponent* GetInventoryComponent() { return InventoryComp; }


	FORCEINLINE  FVector GetInteractPointLocation() { return InteractPointComp->GetComponentLocation(); }
	FORCEINLINE  FRotator GetInteractPointRotation() { return InteractPointComp->GetComponentRotation(); }

	void InteractWithTool(class AAPInteractItemBase* interactBase);
protected:
	virtual void BeginPlay() override;
#pragma region PlayerInputFunctions
	void MoveForward(float axis);
	void MoveRight(float axis);
	void RotateCamera90Clockwise();
	void RotateCamera90CounterClockwise();
	void ChangeToBuildingSystem();
	void Jump();
	void Crouch();
	void UnCrouch();
	void OnMouseClick();

#pragma endregion

	bool m_bRotating;
	FRotator m_DestRotator;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
		float m_CameraRotateSpeed;

	void PerformCameraRotation(float DeltaSeconds);

	void SetNewMoveDestination(const FHitResult& outHit);
	// Navigation component
	class UPathFollowingComponent* m_PFollowComp;
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
		void MoveTo(AController* i_Controller, const FVector& GoalLocation);
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
		void CancelMoveToLocation();

	class UPathFollowingComponent* InitNavigationControl(AController& Controller);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* InventoryComp;

	class AAPToolBase* m_usingTool;

	/*

	#pragma region Inventory

	protected:


		UPROPERTY(VisibleAnywhere, Category = "Utils")
			TArray<FInventoryItem> Inventory;



		void SetSelectingItem(FInventoryItem* _item);
		void ClearItem();

		TMap<FName, class AAPToolBase*> m_spawnedToolList;
		// Spawn all useful tools and disable them.
		void SpawnUsefulTools();
	public:
		UFUNCTION(BlueprintCallable, Category = "Utils")
			void AddItemToInventory(FName itemID);
		FInventoryItem* GetCurrentItem() const { return m_currentItem; }

		void SwitchToItem(int slotID);
		void InteractWithTool(class AAPInteractItemBase* interactBase);

		UFUNCTION(BlueprintCallable)
		void RemoveItemFromInventory(FName itemID, int _amount);
		UFUNCTION(BlueprintCallable)
		void ThrowItem(const FInventoryItem& _spawnItem);
		UFUNCTION(BlueprintImplementableEvent)
			void OnAddNewItem(FInventoryItem _newItem);
		UFUNCTION(BlueprintImplementableEvent)
			void OnAddExistingItem(FName _name);

	#pragma endregion

	*/

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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};

