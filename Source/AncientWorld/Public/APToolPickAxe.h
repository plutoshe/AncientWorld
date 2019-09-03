// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "APToolBase.h"
#include "APToolPickAxe.generated.h"

/**
 *
 */
UCLASS()
class ANCIENTWORLD_API AAPToolPickAxe : public AAPToolBase
{
	GENERATED_BODY()

		AAPToolPickAxe();

protected:

	void Use() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* SuperMesh;

	/*
		UPROPERTY(EditDefaultsOnly)
			class UAnimSequence* Anim;*/
	bool m_Startplaying;
	bool m_StopPlaying;

public:
	// Call when this tool is being selected in equipment slots
	void OnSelected() override;
	// Call when the interact function call in player controller
	void StartUse(class AAPInteractItemBase* m_InteractingItemBase) override;
	void OnBackSwingFinish() override;
	// Call when this tool is being deselected in equipment slots
	void DeSelected() override;


	UFUNCTION(BlueprintCallable)
		bool GetStartPlay() { return m_Startplaying; }
	UFUNCTION(BlueprintCallable)
		bool GetStopPlay() { return m_StopPlaying; }

};
