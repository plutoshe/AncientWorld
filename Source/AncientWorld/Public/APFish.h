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

	void TraceToGround();
	void BounceToRandomDirection();
	float m_BounceThreshold;
	bool m_bOnWater;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
