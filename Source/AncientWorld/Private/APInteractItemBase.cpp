// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/APInteractItemBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "AncientWorldCharacter.h"
#include "Components/WidgetComponent.h"
// Sets default values
AAPInteractItemBase::AAPInteractItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapComp"));
	RootComponent = CapComp;
	CapComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	OverlapComp = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComp"));
	OverlapComp->SetupAttachment(CapComp);
	OverlapComp->SetSphereRadius(120.f);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AAPInteractItemBase::OnPawnEnter);
	OverlapComp->OnComponentEndOverlap.AddDynamic(this, &AAPInteractItemBase::OnPawnLeft);


	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(CapComp);

}

// Called when the game starts or when spawned
void AAPInteractItemBase::BeginPlay()
{
	Super::BeginPlay();
	SetWidgetVisibility(false);
}

void AAPInteractItemBase::OnPawnEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr) {
		AAncientWorldCharacter* characterBase = Cast<AAncientWorldCharacter>(OtherActor);
		if (characterBase) {
			m_InteratingActor = characterBase;

			SetWidgetVisibility(true);
		}
	}
}

void AAPInteractItemBase::OnPawnLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr) {
		AAncientWorldCharacter* characterBase = Cast<AAncientWorldCharacter>(OtherActor);
		if (characterBase) {
			if (m_InteratingActor == characterBase) {
				SetWidgetVisibility(false);
				UnInteract();
				m_InteratingActor = nullptr;
			}
		}
	}
}

// Called every frame
void AAPInteractItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPInteractItemBase::Interact()
{
	if (!m_InteratingActor) return;
	m_bInteracting = true;
	BPInteract();
}

void AAPInteractItemBase::UnInteract()
{
	if(!m_bInteracting) return;
	m_bInteracting = false;
	BPUnInteract();
}

void AAPInteractItemBase::SetWidgetVisibility(bool _show)
{
	WidgetComp->SetVisibility(_show);
}

