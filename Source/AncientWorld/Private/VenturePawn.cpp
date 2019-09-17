// Fill out your copyright notice in the Description page of Project Settings.


#include "VenturePawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "Public/PlayerControllerBase.h"
#include "Public/InventoryComponent.h"

AVenturePawn::AVenturePawn()
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
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Resources/Materials/M_Cursor_Decal.M_Cursor_Decal'"));
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

	InteractPointComp = CreateDefaultSubobject<USceneComponent>(TEXT("InteractPointComp"));
	InteractPointComp->SetupAttachment(RootComponent);

	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
}

void AVenturePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (APlayerControllerBase* PC = Cast<APlayerControllerBase>(GetController()))
		{
			if (PC) {
				FHitResult TraceHitResult;
				PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
				FVector CursorFV = TraceHitResult.ImpactNormal;
				FRotator CursorR = CursorFV.Rotation();
				CursorToWorld->SetWorldLocation(TraceHitResult.Location);
				CursorToWorld->SetWorldRotation(CursorR);
			}
		}
	}

	PerformCameraRotation(DeltaSeconds);
}

void AVenturePawn::BeginPlay()
{
	Super::BeginPlay();
}

void AVenturePawn::MoveForward(float axis)
{
	FVector dir = CameraComp->GetForwardVector();
	dir.Z = 0;
	AddMovementInput(dir.GetSafeNormal()*axis);
}

void AVenturePawn::VertialAxis(float axis)
{
	MoveForward(axis);
}

void AVenturePawn::MoveRight(float axis)
{
	AddMovementInput(CameraComp->GetRightVector() * axis);
}

void AVenturePawn::HorizontalAxis(float axis)
{
	MoveRight(axis);
}

void AVenturePawn::RotateCamera90Clockwise()
{
	if (!m_bRotating) {
		m_DestRotator = CameraBoom->GetComponentRotation();
		m_DestRotator.Add(0, 90, 0);
		m_bRotating = true;
	}
}

void AVenturePawn::OnQPressed()
{
	RotateCamera90Clockwise();
}

void AVenturePawn::RotateCamera90CounterClockwise()
{
	if (!m_bRotating) {
		m_DestRotator = CameraBoom->GetComponentRotation();
		m_DestRotator.Add(0, -90, 0);
		m_bRotating = true;
	}
}

void AVenturePawn::OnEPressed()
{
	RotateCamera90CounterClockwise();
}

void AVenturePawn::Jump()
{
	Super::Jump();
}

void AVenturePawn::OnSpacePressed()
{
	Jump();
}

void AVenturePawn::NormalAxis(float axis)
{
	BPSwimNormal(axis);
}

void AVenturePawn::PerformCameraRotation(float DeltaSeconds)
{
	if (m_bRotating) {
		CameraBoom->SetWorldRotation(FMath::RInterpTo(CameraBoom->GetComponentRotation(), m_DestRotator, DeltaSeconds, m_CameraRotateSpeed));
		if (CameraBoom->GetComponentRotation().Equals(m_DestRotator, 0.01f)) {
			m_bRotating = false;
			CameraBoom->SetWorldRotation(m_DestRotator);
		}
	}
}

