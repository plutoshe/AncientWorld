// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APFish.generated.h"

UCLASS()
class ANCIENTWORLD_API AAPFish : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAPFish();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* SuperMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereComp;

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
	UPROPERTY(VisibleAnywhere, Category = FishGroundMovement)
		bool m_bOnWater;

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void InOutWater(bool _inWater);
	void GetStuned();
};
