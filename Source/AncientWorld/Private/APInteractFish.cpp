// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/APInteractFish.h"
#include "AncientWorldCharacter.h"
#include "Public/APPickUP.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

AAPInteractFish::AAPInteractFish()
{
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SuperMesh->SetupAttachment(CapComp);

	m_XYOffset = 20;

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

void AAPInteractFish::BeginPlay()
{
	Super::BeginPlay();
	m_CurrentHealth = m_MaxHealth;

}

void AAPInteractFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!m_bOnWater)
		TraceToGround();
	else {
		TraceToObstcle();
		UpdateFishMovement(DeltaTime);
	}
	if (!m_bStuned)
		RotateBodyYaw(DeltaTime);
}

void AAPInteractFish::GetDamage(float _dmg)
{
	m_CurrentHealth -= _dmg;
	if (m_CurrentHealth <= 0) {
		m_CurrentHealth = 0;

		SpawnItems();
		Destroy();
	}
}

void AAPInteractFish::SpawnItems()
{
	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn rewards
	for (TSubclassOf<class AAPPickUP> pickUp : m_spawnPickList)
	{
		FVector RndDirection = FMath::VRand();
		RndDirection.Z = 0;
		AAPPickUP* sp = GetWorld()->SpawnActor<AAPPickUP>(pickUp, CapComp->GetComponentLocation() + RndDirection * m_XYOffset, GetActorRotation(), ActorSpawnParams);

		//sp->GetStaticMesh()->AddImpulse(RndDirection * m_RndImpulse, NAME_None, true);
	}
}

void AAPInteractFish::Interact()
{
	Super::Interact();
	// it is a required tool
	if (m_requiredItemID.IsEqual(m_InteratingActor->GetCurrentItem()->ItemID)) {
		GetDamage(10);

	}
}

#pragma region GroundMovement

void AAPInteractFish::TraceToGround()
{
	FCollisionQueryParams CollisionQueryParam;
	CollisionQueryParam.AddIgnoredActor(this);
	CollisionQueryParam.AddIgnoredComponent(SuperMesh);
	CollisionQueryParam.AddIgnoredComponent(CapComp);

	FHitResult outHit;
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() - FVector::UpVector * 1000.f;

	if (GetWorld()->LineTraceSingleByChannel(outHit, Start,
		End, ECC_WorldStatic, CollisionQueryParam)) {
		//	DrawDebugLine(GetWorld(), Start, outHit.ImpactPoint, FColor::Red, true, 0.1f);
		//	UE_LOG(LogTemp, Log, TEXT("Dist: %f"), outHit.Distance)
		if (outHit.Distance <= m_BounceThreshold + CapComp->GetScaledCapsuleHalfHeight()) {
			BounceToRandomDirection();
		}

	}

}

void AAPInteractFish::RotateBodyYaw(float t)
{
	startTime += t;
	FRotator newRot = SuperMesh->RelativeRotation;
	newRot.Yaw = m_SwingAngle * FMath::Sin((m_bOnWater ? m_SwingTailWaterSpeed : m_SwingTailGroundSpeed) * startTime);
	SuperMesh->SetRelativeRotation(newRot);
}

void AAPInteractFish::BounceToRandomDirection()
{
	FVector RNd = FMath::VRand();

	FVector dir = FVector::UpVector;
	dir.X = FMath::RandRange(-0.1f, 0.1f);
	dir.Y = FMath::RandRange(-0.1f, 0.1f);
	FVector impulse = dir * RNd * m_ImpulsePower;
	CapComp->AddImpulse((impulse + FVector::UpVector * m_ImpulsePower) * 100, NAME_None);
}
#pragma endregion

#pragma region WaterMovement

void AAPInteractFish::TraceToObstcle()
{
	FCollisionQueryParams CollisionQueryParam;
	CollisionQueryParam.AddIgnoredActor(this);
	CollisionQueryParam.AddIgnoredComponent(SuperMesh);
	CollisionQueryParam.AddIgnoredComponent(CapComp);

	FHitResult outHit;
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() + GetActorForwardVector() * (CapComp->GetScaledCapsuleRadius() + 10);
	//	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f);

	if (GetWorld()->LineTraceSingleByChannel(outHit, Start,
		End, ECC_WorldStatic, CollisionQueryParam)) {

		MoveTo3DLocation(outHit.ImpactPoint + outHit.ImpactNormal * 30.f);
	}
}

void AAPInteractFish::UpdateFishMovement(float t)
{
	FVector Dir = m_Destination - GetActorLocation();
	CapComp->AddRelativeLocation(Dir.GetSafeNormal() * m_MoveSpeed * t);
	SetActorRotation(FMath::Lerp(GetActorRotation(), Dir.Rotation(), m_TurnRate * t));
	if (Dir.Size() < 10) {
		OnArriveDestination();
	}
}

void AAPInteractFish::MoveTo3DLocation(FVector _newLoc)
{
	DrawDebugLine(GetWorld(), GetActorLocation(), _newLoc, FColor::Yellow, false, 2.f);
	m_Destination = _newLoc;
	m_bMoving = true;
}

FVector AAPInteractFish::RandomLocationInSphere(FVector _center, float _radius)
{
	//DrawDebugSphere(GetWorld(), _center, _radius, 12, FColor::Blue, false, 2.f);

	FVector rdPoint = _center + _radius * FMath::VRand().GetSafeNormal();//FMath::FRand() * FRotator(FMath::FRandRange(-90, 90), FMath::FRandRange(0, 360), 0).Vector().GetSafeNormal();
	return rdPoint;
}

void AAPInteractFish::OnArriveDestination()
{
	MoveTo3DLocation(RandomLocationInSphere(GetActorLocation(), FMath::RandRange(m_RandomSphereRangeMin, m_RandomSphereRangeMax)));
}
#pragma endregion

void AAPInteractFish::InOutWater(bool _inWater)
{
	m_bOnWater = _inWater;

	if (m_bOnWater) {
		OnArriveDestination();
		CapComp->SetSimulatePhysics(false);
		m_SwingAngle = 15;
	}
	else {
		CapComp->SetSimulatePhysics(true);
		m_bMoving = false;
		m_SwingAngle = 90;
	}
}

void AAPInteractFish::GetStuned()
{
	m_bStuned = true;
	SuperMesh->SetRelativeRotation(FRotator::ZeroRotator);
}
