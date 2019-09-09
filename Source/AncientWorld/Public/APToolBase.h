// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APToolBase.generated.h"

UCLASS()
class ANCIENTWORLD_API AAPToolBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAPToolBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName m_ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText m_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText m_Describtion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bCanBeUsed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly)
	float m_castTime;
	UPROPERTY(EditDefaultsOnly)
	float m_backSwingTime;

	bool m_bSelected;
	bool m_bCasting;

	FTimerHandle m_CastHandle;
	// Call when cast time is done
	virtual void OnCastFinish();
	// The actual effect to the environment / player or whatever
	virtual void Use();

	UFUNCTION(BlueprintImplementableEvent)
		void BPOnUse();
	FTimerHandle m_BackSwingHandle;
	// Call when back swing time is done
	virtual void OnBackSwingFinish();

	class AAncientWorldCharacter* m_Parent;

	class AAPInteractItemBase* m_InteractingItemBase;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Call when this tool is being selected in equipment slots
	virtual void OnSelected();
	// Call when the interact function call in player controller
	virtual void StartUse(class AAPInteractItemBase* interactBase);

	// Call when this tool is being deselected in equipment slots
	virtual void OnDeSelected();
	virtual void DeSelected();

	virtual void SetOwner(AAncientWorldCharacter* _Owner);

};
