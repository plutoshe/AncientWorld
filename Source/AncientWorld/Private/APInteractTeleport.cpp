// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/APInteractTeleport.h"
#include "Components/StaticMeshComponent.h"
#include "AncientWorldCharacter.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ConstructorHelpers.h"

AAPInteractTeleport::AAPInteractTeleport()
{
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Super Mesh"));
	SuperMesh->SetupAttachment(RootComponent);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));

	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DoorMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	DoorMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DoorMesh->SetupAttachment(RootComponent);

	DoorMesh->OnComponentEndOverlap.AddDynamic(this, &AAPInteractTeleport::OnPawnLeftDoor);

	m_TeleportCDTime = 1.5f;
	m_bCanTeleport = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> DisableMaterial(TEXT("MaterialInstanceConstant'/Game/Resources/Geometry/Meshes/Environment/Teleport/m_Teleport_Disable.m_Teleport_Disable'"));
	if (DisableMaterial.Succeeded())
	{
		m_Dynamic_M_disabledMaterial = DisableMaterial.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> EnableMaterial(TEXT("MaterialInstanceConstant'/Game/Resources/Geometry/Meshes/Environment/Teleport/m_Teleport_Enable.m_Teleport_Enable'"));
	if (EnableMaterial.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Succeed Loading TP"));
		m_Dynamic_M_enabledMaterial = EnableMaterial.Object;

	}
}


void AAPInteractTeleport::SetTeleportable(bool _canTeleport)
{
	m_bCanTeleport = _canTeleport;
	DoorMesh->SetMaterial(0, _canTeleport?m_Dynamic_M_enabledMaterial: m_Dynamic_M_disabledMaterial);
}

void AAPInteractTeleport::EnterTeleportCD()
{
	SetTeleportable(false);
	GetWorldTimerManager().SetTimer(m_TeleportCDHandle, this, &AAPInteractTeleport::OnCDFinish, m_TeleportCDTime, false);
}

void AAPInteractTeleport::BeginPlay()
{
	Super::BeginPlay();
	SetTeleportable(false);
}

void AAPInteractTeleport::OnPawnLeftDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (m_bCanTeleport && OtherActor != nullptr && OtherActor != this && OtherComp != nullptr) {
		AAncientWorldCharacter* _leftChar = Cast<AAncientWorldCharacter>(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("Character left"));
		if (_leftChar && m_linkingTeleportor)
		{
			m_EnteringCharacter = _leftChar;
			TeleportToLinkingTeleportor();
		}
	}
}

void AAPInteractTeleport::TeleportToLinkingTeleportor()
{
	m_linkingTeleportor->EnterTeleportCD();
	FVector diff = m_EnteringCharacter->GetActorLocation() - GetActorLocation();
	m_EnteringCharacter->SetActorLocation(m_linkingTeleportor->GetActorLocation() + diff);
	m_EnteringCharacter = nullptr;
}
