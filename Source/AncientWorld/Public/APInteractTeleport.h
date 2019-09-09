// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "APInteractItemBase.h"
#include "APInteractTeleport.generated.h"

/**
 *
 */
UCLASS()
class ANCIENTWORLD_API AAPInteractTeleport : public AAPInteractItemBase
{
	GENERATED_BODY()
public:
	AAPInteractTeleport();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* SuperMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Teleporter, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, Category = Teleporter)
		int m_TeleportorID;
	UPROPERTY(EditAnywhere, Category = Teleporter)
		AAPInteractTeleport* m_linkingTeleportor;
	UFUNCTION(BlueprintCallable, Category = Teleporter)
	void SetTeleportable(bool _canTeleport);
	void EnterTeleportCD();
protected:

	virtual void BeginPlay() override;

	FTimerHandle m_TeleportCDHandle;
	float m_TeleportCDTime;
	void OnCDFinish() { SetTeleportable(true); }

	bool m_bCanTeleport;
	  

	class UMaterialInstance* m_Dynamic_M_disabledMaterial;
	class UMaterialInstance* m_Dynamic_M_enabledMaterial;

	UFUNCTION()
		void OnPawnLeftDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	class AAncientWorldCharacter* m_EnteringCharacter;
	void TeleportToLinkingTeleportor();
};
