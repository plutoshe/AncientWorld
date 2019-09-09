// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/APToolBase.h"
#include "Components/MeshComponent.h"
#include "TimerManager.h"
#include "Components/SceneComponent.h"
#include "AncientWorldCharacter.h"
#include "Public/APInteractItemBase.h"

// Sets default values
AAPToolBase::AAPToolBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;
}

// Called when the game starts or when spawned
void AAPToolBase::BeginPlay()
{
	Super::BeginPlay();

}



// Called every frame
void AAPToolBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bSelected) {
		SetActorLocation(m_Parent->GetInteractPointLocation());
		SetActorRotation(m_Parent->GetInteractPointRotation());
	}

}

void AAPToolBase::OnSelected()
{
	if (m_bSelected) return;
	m_bSelected = true;
	SetActorTickEnabled(true);
}

void AAPToolBase::StartUse(AAPInteractItemBase* interactBase)
{
	// if it is using, or selecting or can not be used
	if (m_bCasting || !m_bSelected || !m_bCanBeUsed) return;

	m_InteractingItemBase = interactBase;
	m_bCasting = true;
	UE_LOG(LogTemp, Log, TEXT("Start Use Once"));

	// when time is up, use it
	GetWorldTimerManager().SetTimer(m_CastHandle, this, &AAPToolBase::OnCastFinish, m_castTime, false);

}

void AAPToolBase::OnCastFinish()
{
	GetWorldTimerManager().ClearTimer(m_CastHandle);
	// if the player switch to another tool, can not use it
	if (m_bSelected)
		Use();

}

void AAPToolBase::Use()
{
	GetWorldTimerManager().SetTimer(m_BackSwingHandle, this, &AAPToolBase::OnBackSwingFinish, m_backSwingTime, false);
	BPOnUse();
}

void AAPToolBase::OnBackSwingFinish()
{
	m_InteractingItemBase = nullptr;
	m_bCasting = false;
	UE_LOG(LogTemp, Log, TEXT("Finish use once"));
	GetWorldTimerManager().ClearTimer(m_BackSwingHandle);
	// finish one interact
}

void AAPToolBase::OnDeSelected()
{
	if (!m_bSelected) return;
	DeSelected();

}

void AAPToolBase::DeSelected()
{
	m_bSelected = false;
	SetActorTickEnabled(false);
	UE_LOG(LogTemp, Log, TEXT("%s is deselecting"), *m_Name.ToString());
}

void AAPToolBase::SetOwner(AAncientWorldCharacter* _Owner)
{
	m_Parent = _Owner;
}

