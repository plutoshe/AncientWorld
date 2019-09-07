// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/BuildingSynchronization.h"
#include "Engine/Classes/Engine/StaticMeshActor.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Classes/Engine/StaticMesh.h"
#include "AncientWorldGameInstance.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Materials/Material.h"

// Sets default values
ABuildingSynchronization::ABuildingSynchronization()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildingSynchronization::BeginPlay()
{
	Super::BeginPlay();
	m_gameStateInstance = static_cast<UAncientWorldGameInstance*>(UGameplayStatics::GetGameInstance(GetWorld()));
	FVector basePoint = GetActorLocation();

	for (int i = 0; i < m_gameStateInstance->m_baseStatus.Num(); i++)
	{

		AStaticMeshActor* newBlcock = static_cast<AStaticMeshActor*>(GetWorld()->SpawnActor(AStaticMeshActor::StaticClass()));
		newBlcock->SetMobility(EComponentMobility::Movable);
		newBlcock->GetStaticMeshComponent()->SetStaticMesh(m_gameStateInstance->m_buildings[m_gameStateInstance->m_baseStatus[i].m_buildingBlockIndex].m_mesh);
		newBlcock->GetStaticMeshComponent()->SetMaterial(0, m_gameStateInstance->m_buildings[m_gameStateInstance->m_baseStatus[i].m_buildingBlockIndex].m_material);
		newBlcock->SetActorLocation(basePoint + m_gameStateInstance->m_baseStatus[i].m_positionIndexForBase * m_gameStateInstance->m_BaseLayerLength);
	}
}

// Called every frame
void ABuildingSynchronization::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

