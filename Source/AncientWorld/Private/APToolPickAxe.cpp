// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/APToolPickAxe.h"
#include "Components/SkeletalMeshComponent.h"
#include "Public/APInteractItemBase.h"
AAPToolPickAxe::AAPToolPickAxe()
{
	SuperMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Super Mesh"));
	SuperMesh->SetupAttachment(SceneComp);

}

void AAPToolPickAxe::Use()
{
	Super::Use();
	if (m_InteractingItemBase)
		m_InteractingItemBase->Interact();

}

void AAPToolPickAxe::OnSelected()
{
	Super::OnSelected();

	SuperMesh->SetVisibility(true);
	
}

void AAPToolPickAxe::StartUse(AAPInteractItemBase* _InteractingItemBase)
{
	Super::StartUse(_InteractingItemBase);
	m_Startplaying = true;
	m_StopPlaying = false;
}

void AAPToolPickAxe::OnBackSwingFinish()
{
	Super::OnBackSwingFinish();

	m_Startplaying = false;
	m_StopPlaying = true;
}

void AAPToolPickAxe::DeSelected()
{
	Super::DeSelected();

	SuperMesh->SetVisibility(false);
	SuperMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

