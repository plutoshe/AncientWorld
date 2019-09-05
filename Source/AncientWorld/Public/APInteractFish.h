// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "APInteractItemBase.h"
#include "APInteractFish.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTWORLD_API AAPInteractFish : public AAPInteractItemBase
{
	GENERATED_BODY()
		AAPInteractFish();
protected:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* SuperMesh;
	UPROPERTY(EditAnywhere, Category = "Utils")
		TArray<TSubclassOf<class AAPPickUP>> m_spawnPickList;

	UPROPERTY(EditDefaultsOnly, Category = "Utils")
		FName m_requiredItemID;

	UPROPERTY(EditDefaultsOnly, Category = "Utils")
		float m_MaxHealth;
	float m_CurrentHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Utils")
		float m_XYOffset;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void RotateBodyYaw(float t);
	float startTime;
	bool m_bStuned;
#pragma region GroundMovement
	void TraceToGround();
	void BounceToRandomDirection();
	UPROPERTY(EditAnywhere, Category = FishGroundMovement)
		float m_ImpulsePower;
	UPROPERTY(EditAnywhere, Category = FishGroundMovement)
		float m_SwingTailGroundSpeed;
	UPROPERTY(EditAnywhere, Category = FishGroundMovement)
		float m_BounceThreshold;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FishGroundMovement)
		bool m_bOnWater;
protected:
	float m_SwingAngle;
#pragma endregion
#pragma region WaterMovement

	FVector m_Destination;
	UPROPERTY(EditAnywhere, Category = FishWaterMovement)
		float m_RandomSphereRangeMin;
	UPROPERTY(EditAnywhere, Category = FishWaterMovement)
		float m_RandomSphereRangeMax;
	bool m_bMoving;
	UPROPERTY(EditAnywhere, Category = FishWaterMovement)
		float m_MoveSpeed;
	UPROPERTY(EditAnywhere, Category = FishWaterMovement)
		float m_SwingTailWaterSpeed;
	UPROPERTY(EditAnywhere, Category = FishWaterMovement)
		float m_TurnRate;
	void TraceToObstcle();
	void UpdateFishMovement(float t);
	void MoveTo3DLocation(FVector _newLoc);
	FVector RandomLocationInSphere(FVector _center, float _radius);
public:
	void OnArriveDestination();
#pragma endregion


public:
	void GetDamage(float _dmg);
	void SpawnItems();

	virtual void Interact() override;

	UFUNCTION(BlueprintCallable)
		void InOutWater(bool _inWater);
	void GetStuned();
};
