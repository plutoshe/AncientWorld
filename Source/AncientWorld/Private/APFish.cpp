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


	m_ImpulsePower = 250;
	m_BounceThreshold = 5;
	m_SwingAngle = 90;
	m_bOnWater = false;
	m_TurnRate = 50;
	m_SwingTailWaterSpeed = 10;
	m_SwingTailGroundSpeed = 45;
	m_RandomSphereRangeMin = 300;
	m_RandomSphereRangeMax = 500;
}

// Called when the game starts or when spawned
void AAPFish::BeginPlay()
{
	Super::BeginPlay();

}
#pragma region GroundMovement

void AAPFish::TraceToGround()
{
	FCollisionQueryParams CollisionQueryParam;
	CollisionQueryParam.AddIgnoredActor(this);
	CollisionQueryParam.AddIgnoredComponent(SuperMesh);
	CollisionQueryParam.AddIgnoredComponent(SphereComp);

	FHitResult outHit;
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() - FVector::UpVector*1000.f;

	if (GetWorld()->LineTraceSingleByChannel(outHit, Start,
		End, ECC_WorldStatic, CollisionQueryParam)) {
		//	DrawDebugLine(GetWorld(), Start, outHit.ImpactPoint, FColor::Red, true, 0.1f);
		//	UE_LOG(LogTemp, Log, TEXT("Dist: %f"), outHit.Distance)
		if (outHit.Distance <= m_BounceThreshold + SphereComp->GetScaledSphereRadius()) {
			BounceToRandomDirection();
		}

	}

}

void AAPFish::RotateBodyYaw(float t)
{
	startTime += t;
	FRotator newRot = SuperMesh->RelativeRotation;
	newRot.Yaw = m_SwingAngle * FMath::Sin((m_bOnWater?m_SwingTailWaterSpeed: m_SwingTailGroundSpeed)*startTime);
	SuperMesh->SetRelativeRotation(newRot);
}

void AAPFish::BounceToRandomDirection()
{
	FVector RNd = FMath::VRand();

	FVector dir = FVector::UpVector;
	dir.X = FMath::RandRange(-0.1f, 0.1f);
	dir.Y = FMath::RandRange(-0.1f, 0.1f);
	FVector impulse = dir * RNd * m_ImpulsePower;
	SuperMesh->AddImpulse((impulse + FVector::UpVector * m_ImpulsePower) * 100, NAME_None);
}
#pragma endregion

#pragma region WaterMovement

void AAPFish::TraceToObstcle()
{
	FCollisionQueryParams CollisionQueryParam;
	CollisionQueryParam.AddIgnoredActor(this);
	CollisionQueryParam.AddIgnoredComponent(SuperMesh);
	CollisionQueryParam.AddIgnoredComponent(SphereComp);

	FHitResult outHit;
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() + GetActorForwardVector()*(SphereComp->GetScaledSphereRadius() +10);
//	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f);

	if (GetWorld()->LineTraceSingleByChannel(outHit, Start,
		End, ECC_WorldStatic, CollisionQueryParam)) {
			
		MoveTo3DLocation(outHit.ImpactPoint + outHit.ImpactNormal * 30.f);
	}
}

void AAPFish::UpdateFishMovement(float t)
{
	FVector Dir = m_Destination - GetActorLocation();
	SphereComp->AddRelativeLocation(Dir.GetSafeNormal()*m_MoveSpeed*t);
	SetActorRotation(FMath::Lerp(GetActorRotation(), Dir.Rotation(), m_TurnRate*t));
	if (Dir.Size() < 10) {
		OnArriveDestination();
	}
}

void AAPFish::MoveTo3DLocation(FVector _newLoc)
{
	//DrawDebugLine(GetWorld(), GetActorLocation(), _newLoc, FColor::Yellow, false, 2.f);
	m_Destination = _newLoc;
	m_bMoving = true;
}

FVector AAPFish::RandomLocationInSphere(FVector _center, float _radius)
{
	//DrawDebugSphere(GetWorld(), _center, _radius, 12, FColor::Blue, false, 2.f);

	FVector rdPoint = _center + _radius * FMath::VRand().GetSafeNormal();//FMath::FRand() * FRotator(FMath::FRandRange(-90, 90), FMath::FRandRange(0, 360), 0).Vector().GetSafeNormal();
	return rdPoint;
}

void AAPFish::OnArriveDestination()
{
	MoveTo3DLocation(RandomLocationInSphere(GetActorLocation(), FMath::RandRange(m_RandomSphereRangeMin, m_RandomSphereRangeMax)));
}
#pragma endregion
// Called every frame
void AAPFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_bOnWater)
		TraceToGround();
	else {
		TraceToObstcle();
		UpdateFishMovement(DeltaTime);
	}
	if(!m_bStuned)
	RotateBodyYaw(DeltaTime);
}

void AAPFish::InOutWater(bool _inWater)
{
	m_bOnWater = _inWater;

	if (m_bOnWater) {
		OnArriveDestination();
		SphereComp->SetSimulatePhysics(false);
		m_SwingAngle = 15;
	}
	else {
		SphereComp->SetSimulatePhysics(true);
		m_bMoving = false;
		m_SwingAngle = 90;
	}
}

void AAPFish::GetStuned()
{
	m_bStuned = true;
	SuperMesh->SetRelativeRotation(FRotator::ZeroRotator);
}

