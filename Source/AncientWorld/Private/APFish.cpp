// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/APFish.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
// Sets default values
AAPFish::AAPFish()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMesh"));
	SuperMesh->SetupAttachment(RootComponent);


	m_BounceThreshold = 5;
	m_bOnWater = false;
}

// Called when the game starts or when spawned
void AAPFish::BeginPlay()
{
	Super::BeginPlay();

}

void AAPFish::TraceToGround()
{
	FCollisionQueryParams CollisionQueryParam;
	CollisionQueryParam.AddIgnoredActor(this);
	FHitResult outHit;
	FVector Start = GetActorLocation() - GetActorUpVector() * SphereComp->GetScaledSphereRadius();
	FVector End = GetActorLocation() - GetActorUpVector()*1000.f;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 5.f);
	if (GetWorld()->LineTraceSingleByChannel(outHit, Start,
		End, ECC_WorldStatic, CollisionQueryParam)) {
		if (outHit.Actor != nullptr) {
			UE_LOG(LogTemp, Log, TEXT("Name: %s"), *outHit.Actor->GetName());

			if (outHit.Distance <= m_BounceThreshold) {
				BounceToRandomDirection();
			}
		}
	}

}

void AAPFish::BounceToRandomDirection()
{
	FVector RNd = FMath::VRand();

	FVector dir = RNd * 30+GetActorUpVector() * SphereComp->GetScaledSphereRadius();
	FVector impulse = dir * 200.f;
	SuperMesh->AddImpulseAtLocation(impulse, GetActorLocation() - GetActorUpVector() * SphereComp->GetScaledSphereRadius(), NAME_None);
}

// Called every frame
void AAPFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!m_bOnWater)
	TraceToGround();

}

