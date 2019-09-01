// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APInteractItemBase.generated.h"

UCLASS()
class ANCIENTWORLD_API AAPInteractItemBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAPInteractItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CapComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* OverlapComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* SuperMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* WidgetComp;

	UFUNCTION()
		void OnPawnEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void OnPawnLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	class AAncientWorldCharacter* m_InteratingActor;
	bool m_bInteracting;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() { if (!m_InteratingActor) return; m_bInteracting = true; BPInteract(); };
	virtual void UnInteract() { if (!m_bInteracting) return; m_bInteracting = false; BPUnInteract(); };

	void SetWidgetVisibility(bool _show);

	UFUNCTION(BlueprintImplementableEvent, Category = Gameplay)
		void BPInteract();
	UFUNCTION(BlueprintImplementableEvent, Category = Gameplay)
		void BPUnInteract();

	FORCEINLINE bool GetCanPawnInteract() const { return m_InteratingActor != nullptr; }

};
