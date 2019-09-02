// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystemPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Engine/Classes/GameFramework/Pawn.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "RenderCore/Public/RenderCore.h"

// Sets default values
ABuildingSystemPawn::ABuildingSystemPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComp->SetupAttachment(CameraBoon, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CameraBoon = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoon->SetupAttachment(RootComponent);
	CameraBoon->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoon->TargetArmLength = 800.f;
	CameraBoon->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoon->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoon->bInheritPitch = false;
	CameraBoon->bInheritRoll = false;
	CameraBoon->bInheritYaw = false;
	
	MoveSpendTime = 0;
	MoveTimeSpan = 0;
}

// Called when the game starts or when spawned
void ABuildingSystemPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void ABuildingSystemPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("ChangeToBuildingSystem", IE_Pressed, this, &ABuildingSystemPawn::ChangeToBuildingSystem);
	PlayerInputComponent->BindAction("MoveUp", IE_Pressed, this, &ABuildingSystemPawn::MoveUp);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ABuildingSystemPawn::MoveDown);

}

void ABuildingSystemPawn::ChangeToBuildingSystem()
{
	UE_LOG(LogTemp, Log, TEXT("hahaha"));
	
}

void ABuildingSystemPawn::MoveForBuilding(int direction)
{
	this->MoveCameraSrc = this->GetActorLocation();
	this->MoveCameraDst = this->GetActorLocation() + FVector(0, 0, 200 * direction);
	this->MoveTimeSpan = 1.0;
	this->MoveSpendTime = 0.0;
}

void ABuildingSystemPawn::MoveUp()
{
	//UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString::FromInt(axis));
	//this->SetActorLocation(FVector(0, 0, 1000));
	MoveForBuilding(1);
	//this->MoveCameraStartTime = FTimer::GetCurrentTime();
	//BecomeViewTarget(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//PlayerController->SetViewTarget
	//this->AddMovementInput(FVector(0, 0, 100));
	//CameraBoon->AddLocalOffset(FVector(0, -1 * axis, 0));
	//CameraBoon->ApplyWorldOffset()
}

void ABuildingSystemPawn::MoveDown()
{
	//UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString::FromInt(axis));
	//this->SetActorLocation(FVector(0, 0, 1000));
	MoveForBuilding(-1);
	//this->MoveCameraStartTime = FTimer::GetCurrentTime();
	//BecomeViewTarget(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//PlayerController->SetViewTarget
	//this->AddMovementInput(FVector(0, 0, 100));
	//CameraBoon->AddLocalOffset(FVector(0, -1 * axis, 0));
	//CameraBoon->ApplyWorldOffset()
}


void ABuildingSystemPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->MoveSpendTime < this->MoveTimeSpan)
	{
		this->MoveSpendTime += DeltaTime;
		this->SetActorLocation(FVector(0, 0, 1000));
		this->SetActorLocation(FMath::Lerp(this->MoveCameraSrc, this->MoveCameraDst, this->MoveSpendTime / this->MoveTimeSpan));
		/*FMath::FInterpTo()
		(
				CameraBoon->TargetArmLength, 
				desiredBoonLength, 
				deltaTime, 
				CameraZoomSpeed);*/
	}
	
}


