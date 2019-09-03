// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "APToolBase.h"
#include "APUnUsableTool.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTWORLD_API AAPUnUsableTool : public AAPToolBase
{
	GENERATED_BODY()
protected:
	AAPUnUsableTool();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* SuperMesh;

public:
	// Call when this tool is being selected in equipment slots
	void OnSelected() override;

	// Call when this tool is being deselected in equipment slots
	void DeSelected() override;

};
