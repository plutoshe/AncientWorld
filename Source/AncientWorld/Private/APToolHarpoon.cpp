// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/APToolHarpoon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Public/APInteractItemBase.h"

AAPToolHarpoon::AAPToolHarpoon()
{
	SuperMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Super Mesh"));
	SuperMesh->SetupAttachment(SceneComp);

}

void AAPToolHarpoon::Use()
{
	Super::Use();
	if (m_InteractingItemBase)
		m_InteractingItemBase->Interact();
}

void AAPToolHarpoon::OnSelected()
{
	Super::OnSelected();

	SuperMesh->SetVisibility(true);

}

void AAPToolHarpoon::StartUse(AAPInteractItemBase* _InteractingItemBase)
{
	Super::StartUse(_InteractingItemBase);
	m_Startplaying = true;
	m_StopPlaying = false;
}

void AAPToolHarpoon::OnBackSwingFinish()
{
	Super::OnBackSwingFinish();

	m_Startplaying = false;
	m_StopPlaying = true;
}

void AAPToolHarpoon::DeSelected()
{
	Super::DeSelected();

	SuperMesh->SetVisibility(false);
	SuperMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}