// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "AncientWorldCharacter.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase {

	GENERATED_BODY()

public:
	FInventoryItem() {
		Name = FText::FromString("Item");
		Action = FText::FromString("Use");
		Describtion = FText::FromString("Add Description");
		Value = 10;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AAPPickUP> ItemPickUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32  Value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Describtion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeUsed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanStack;

	bool operator==(const FInventoryItem& other) const {
		return ItemID == other.ItemID;
	}
};

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

	FORCEINLINE  FVector GetInteractPointLocation() { return InteractPointComp->GetComponentLocation(); }
	FORCEINLINE  FRotator GetInteractPointRotation() { return InteractPointComp->GetComponentRotation(); }


protected:
	virtual void BeginPlay() override;
#pragma region PlayerInputFunctions
	void MoveForward(float axis);
	void MoveRight(float axis);
	void RotateCamera90Clockwise();
	void RotateCamera90CounterClockwise();
	void Jump();
	void Crouch();
	void UnCrouch();
#pragma endregion

	bool m_bRotating;
	FRotator m_DestRotator;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
		float m_CameraRotateSpeed;

	void PerformCameraRotation(float DeltaSeconds);

#pragma region Inventory

protected:
	UPROPERTY(VisibleAnywhere, Category = "Utils")
		TArray<FInventoryItem> Inventory;

	FInventoryItem* m_currentItem;
	class AAPToolBase* m_usingTool;

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

#pragma endregion


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

