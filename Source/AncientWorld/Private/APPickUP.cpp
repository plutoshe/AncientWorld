// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/APPickUP.h"
#include "Components/SphereComponent.h"
#include "AncientWorldCharacter.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AAPPickUP::AAPPickUP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OverlapComp = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AAPPickUP::OnPawnEnter);
	
	RootComponent = OverlapComp;

	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMesh"));
	SuperMesh->SetupAttachment(RootComponent);

	m_MoveSpeed = 20.f;
	m_bCanMoveToPlayer = true;
}

void AAPPickUP::OnPawnEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr) {
		AAncientWorldCharacter* characterBase = Cast<AAncientWorldCharacter>(OtherActor);
		if (characterBase) {
			// Move to character
			StartMoveToPlayer(characterBase);
		}
	}
}

// Called when the game starts or when spawned
void AAPPickUP::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPPickUP::StartMoveToPlayer(AAncientWorldCharacter* _InsideCharacter)
{
	m_bMovingToPlayer = true;
	m_InsideCharacter = _InsideCharacter;
	SuperMesh->SetSimulatePhysics(false);
	SuperMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called every frame
void AAPPickUP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bCanMoveToPlayer && m_bMovingToPlayer) {
		FVector dir = m_InsideCharacter->GetActorLocation() - GetActorLocation();
		SetActorLocation(GetActorLocation() + dir * m_MoveSpeed * DeltaTime);

		if (dir.Size() < m_ThresholdToDestroy) {
			m_bMovingToPlayer = false;
			m_InsideCharacter->AddItemToInventory(m_ItemID);
			m_InsideCharacter = nullptr;
			Destroy();
		}

	}

}

