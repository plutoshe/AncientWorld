// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APPickUP.generated.h"

UCLASS()
class ANCIENTWORLD_API AAPPickUP : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAPPickUP();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName m_ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText m_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText m_Describtion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bCanBeUsed;
protected:

	// Overlap Function for going to player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* OverlapComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* SuperMesh;

	UFUNCTION()
		void OnPawnEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bCanMoveToPlayer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float m_MoveSpeed;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool m_bMovingToPlayer;
	UPROPERTY(EditDefaultsOnly)
	float m_ThresholdToDestroy;
	class AAncientWorldCharacter* m_InsideCharacter;

	UFUNCTION(BlueprintCallable, Category = "Utils")
		void StartMoveToPlayer(AAncientWorldCharacter* m_InsideCharacter);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
