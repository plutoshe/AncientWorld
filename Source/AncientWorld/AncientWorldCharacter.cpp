// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AncientWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"

AAncientWorldCharacter::AAncientWorldCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;

	// Create a camera...
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComp->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownTemplate/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	m_CameraRotateSpeed = 30.f;

}

void AAncientWorldCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}

	PerformCameraRotation(DeltaSeconds);
}
#pragma region PlayerInputFunctions
void AAncientWorldCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AAncientWorldCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAncientWorldCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAncientWorldCharacter::Jump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AAncientWorldCharacter::Crouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AAncientWorldCharacter::UnCrouch);
	PlayerInputComponent->BindAction("RotateCameraC", IE_Pressed, this, &AAncientWorldCharacter::RotateCamera90Clockwise);
	PlayerInputComponent->BindAction("RotateCameraCC", IE_Pressed, this, &AAncientWorldCharacter::RotateCamera90CounterClockwise);
}

void AAncientWorldCharacter::MoveForward(float axis)
{
	FVector dir = CameraComp->GetForwardVector();
	dir.Z = 0;
	AddMovementInput(dir.GetSafeNormal()*axis);
}

void AAncientWorldCharacter::MoveRight(float axis)
{
	AddMovementInput(CameraComp->GetRightVector() * axis);

}

void AAncientWorldCharacter::RotateCamera90Clockwise()
{
	if (!m_bRotating) {
		m_DestRotator = CameraBoom->GetComponentRotation();
		m_DestRotator.Add(0, 90, 0);
		m_bRotating = true;
	}
}

void AAncientWorldCharacter::RotateCamera90CounterClockwise()
{
	if (!m_bRotating) {
		m_DestRotator = CameraBoom->GetComponentRotation();
		m_DestRotator.Add(0, -90, 0);
		m_bRotating = true;
	}
}

void AAncientWorldCharacter::Jump()
{
	Super::Jump();
}

void AAncientWorldCharacter::Crouch()
{
	Super::Crouch();
}

void AAncientWorldCharacter::UnCrouch()
{
	Super::UnCrouch();
}



#pragma endregion

void AAncientWorldCharacter::PerformCameraRotation(float DeltaSeconds)
{
	if (m_bRotating) {
		CameraBoom->SetWorldRotation(FMath::RInterpTo(CameraBoom->GetComponentRotation(), m_DestRotator, DeltaSeconds, m_CameraRotateSpeed));
		if (CameraBoom->GetComponentRotation().Equals(m_DestRotator, 0.01f)) {
			m_bRotating = false;
			CameraBoom->SetWorldRotation(m_DestRotator);
		}
	}
}

