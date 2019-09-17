// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"
#include "Public/CharacterBase.h"
APlayerControllerBase::APlayerControllerBase()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Bind Action
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerControllerBase::OnSpacePressed);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerControllerBase::OnLeftCtrlPressed);
	InputComponent->BindAction("RotateCameraC", IE_Pressed, this, &APlayerControllerBase::OnQPressed);
	InputComponent->BindAction("RotateCameraCC", IE_Pressed, this, &APlayerControllerBase::OnEPressed);
	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerControllerBase::OnLeftMouseClick);
	InputComponent->BindAction("BuildCancellation", IE_Pressed, this, &APlayerControllerBase::OnRightMouseClick);
	InputComponent->BindAction("BlendToBuilding", IE_Pressed, this, &APlayerControllerBase::OnZPressed);

	InputComponent->BindAction("Item1", IE_Pressed, this, &APlayerControllerBase::OnNum1Pressed);
	InputComponent->BindAction("Item2", IE_Pressed, this, &APlayerControllerBase::OnNum2Pressed);
	InputComponent->BindAction("Item3", IE_Pressed, this, &APlayerControllerBase::OnNum3Pressed);
	InputComponent->BindAction("Item4", IE_Pressed, this, &APlayerControllerBase::OnNum4Pressed);
	InputComponent->BindAction("Item5", IE_Pressed, this, &APlayerControllerBase::OnNum5Pressed);
	InputComponent->BindAction("Item6", IE_Pressed, this, &APlayerControllerBase::OnNum6Pressed);
	InputComponent->BindAction("Item7", IE_Pressed, this, &APlayerControllerBase::OnNum7Pressed);

	// Bind Axis
	InputComponent->BindAxis("MoveForward", this, &APlayerControllerBase::VertialAxis);
	InputComponent->BindAxis("MoveRight", this, &APlayerControllerBase::HorizontalAxis);
	InputComponent->BindAxis("SwimUpDown", this, &APlayerControllerBase::NormalAxis);

}

void APlayerControllerBase::OnSpacePressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnSpacePressed();
	}
}

void APlayerControllerBase::OnLeftCtrlPressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnLeftCtrlPressed();
	}
}

void APlayerControllerBase::OnQPressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnQPressed();
	}
}

void APlayerControllerBase::OnEPressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnEPressed();
	}
}

void APlayerControllerBase::OnLeftMouseClick()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnLeftMouseClick();
	}
}

void APlayerControllerBase::OnRightMouseClick()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnRightMouseClick();
	}
}

void APlayerControllerBase::OnZPressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnZPressed();
	}
}

void APlayerControllerBase::OnNum1Pressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnNum1Pressed();
	}
}

void APlayerControllerBase::OnNum2Pressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnNum2Pressed();
	}
}

void APlayerControllerBase::OnNum3Pressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnNum3Pressed();
	}
}

void APlayerControllerBase::OnNum4Pressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnNum4Pressed();
	}
}

void APlayerControllerBase::OnNum5Pressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnNum5Pressed();
	}
}

void APlayerControllerBase::OnNum6Pressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnNum6Pressed();
	}
}

void APlayerControllerBase::OnNum7Pressed()
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->OnNum7Pressed();
	}
}

void APlayerControllerBase::VertialAxis(float axis)
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->VertialAxis(axis);
	}
}

void APlayerControllerBase::HorizontalAxis(float axis)
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->HorizontalAxis(axis);
	}
}

void APlayerControllerBase::NormalAxis(float axis)
{
	ACharacterBase* base = Cast<ACharacterBase>(GetPawn());
	if (base) {
		base->NormalAxis(axis);
	}
}
