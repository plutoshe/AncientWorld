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
		class UWidgetComponent* WidgetComp;

	UFUNCTION()
		void OnPawnEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void OnPawnLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	class AAncientWorldCharacter* m_InteratingActor;
	bool m_bInteracting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* OutlineMesh;



public:


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact();
	virtual void UnInteract();

	void SetWidgetVisibility(bool _show);

	UPROPERTY(EditDefaultsOnly, Category = Gameplay, BlueprintReadOnly)
		bool m_bOneTimeInteract;
	UPROPERTY(EditDefaultsOnly, Category = Gameplay, BlueprintReadWrite)
		bool m_bInteractDisabled;
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
		bool m_bRequireTool;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool m_bHasOutline;
	UFUNCTION(BlueprintImplementableEvent, Category = Gameplay)
		void BPInteract();
	UFUNCTION(BlueprintImplementableEvent, Category = Gameplay)
		void BPUnInteract();
	FORCEINLINE bool GetCanPawnInteract() const { return m_InteratingActor != nullptr; }

};
