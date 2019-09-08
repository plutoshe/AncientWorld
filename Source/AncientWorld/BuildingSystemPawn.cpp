// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystemPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Engine/Classes/GameFramework/Pawn.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "RenderCore/Public/RenderCore.h"
#include "Engine/Public/UnrealEngine.h"
#include "Engine/Classes/Engine/Engine.h"
#include "Engine/Classes/Engine/StaticMeshActor.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Classes/Engine/StaticMesh.h"
#include "AncientWorldGameInstance.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/Classes/Materials/Material.h"
#include "Public/BuildingSynchronization.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"

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
	m_MoveRemainingTime = 0;
	m_MoveTimeSpan = 0;

}

// Called when the game starts or when spawned
void ABuildingSystemPawn::BeginPlay()
{
	Super::BeginPlay();
	m_gameStateInstance = static_cast<UAncientWorldGameInstance*>(UGameplayStatics::GetGameInstance(GetWorld()));
	m_MoveCameraDstZ = this->GetActorLocation().Z;
	m_PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	m_cameraRotationInitialPoint = GetActorLocation();
	m_cameraAngle = 0.f;
	
}

void ABuildingSystemPawn::MoveBuildingCamera(float axis)
{
	if (this->m_MoveRemainingTime <= 0 && m_buildingsystem != nullptr)
	{
		m_cameraAngle += axis;
		FRotator rot(0, m_cameraAngle, 0);
		auto result = m_buildingsystem->GetActorLocation() + rot.RotateVector(m_cameraRotationInitialPoint - m_buildingsystem->GetActorLocation());
		SetActorLocation(FVector(result.X, result.Y, GetActorLocation().Z));
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(result, m_buildingsystem->GetActorLocation()));
	}
}

// Called to bind functionality to input
void ABuildingSystemPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("ChangeToBuildingSystem", IE_Pressed, this, &ABuildingSystemPawn::ChangeToBuildingSystem);
	PlayerInputComponent->BindAction("MoveUp", IE_Pressed, this, &ABuildingSystemPawn::MoveUp);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ABuildingSystemPawn::MoveDown);
	PlayerInputComponent->BindAction("BuildAction", IE_Pressed, this, &ABuildingSystemPawn::BuildAction);
	PlayerInputComponent->BindAction("BuildComplete", IE_Pressed, this, &ABuildingSystemPawn::BuildComplete);
	PlayerInputComponent->BindAction("BuildCancellation", IE_Pressed, this, &ABuildingSystemPawn::BuildCancellation);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABuildingSystemPawn::MoveBuildingCamera);
}
void ABuildingSystemPawn::BuildAction()
{
	if (m_buildingsystem != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("BuildAction"));
		m_buildingsystem->InitialBlock(m_BuildingBlock, m_gameStateInstance->GetCurrentBuildingBlockID(), false);
		m_BuildingBlock->GetStaticMeshComponent()->SetMaterial(0, m_gameStateInstance->m_materialOnBuild);
	}

}
 
void ABuildingSystemPawn::BuildComplete()
{
	UE_LOG(LogTemp, Log, TEXT("BuildComplete"));
	if (m_buildingsystem != nullptr && m_BuildingBlock != nullptr)
	{
		m_buildingsystem->ConfirmBuilding();
		m_BuildingBlock->K2_DestroyActor();
		m_BuildingBlock = nullptr;
	}
}

void ABuildingSystemPawn::BuildCancellation()
{
	if (m_buildingsystem != nullptr && m_BuildingBlock != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("BuildCancellation"));
		m_BuildingBlock->K2_DestroyActor();
		m_BuildingBlock = nullptr;
	}
}
void ABuildingSystemPawn::ChangeToBuildingSystem()
{
	UE_LOG(LogTemp, Log, TEXT("hahaha"));
	
}

void ABuildingSystemPawn::MoveForBuilding(int direction)
{
	if (this->m_MoveRemainingTime <= 0) {
		if (!(this->m_MoveCameraDstZ >= 2000 && direction > 0 || this->m_MoveCameraDstZ < -1000 && direction < 0))
		{
			this->m_MoveCameraSrcZ = this->GetActorLocation().Z;
			this->m_MoveCameraDstZ = this->m_MoveCameraSrcZ + m_gameStateInstance->GetBuildingLayerLength().Z * 3 * direction;
			this->m_MoveTimeSpan = 0.5f + this->m_MoveRemainingTime;
			this->m_MoveRemainingTime = 0.5f + this->m_MoveRemainingTime;
		}
	}
}

void ABuildingSystemPawn::MoveUp()
{
	MoveForBuilding(1);
}

void ABuildingSystemPawn::MoveDown()
{
	MoveForBuilding(-1);
}

void ABuildingSystemPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->m_MoveRemainingTime > 0)
	{
		this->m_MoveRemainingTime = FMath::Max(this->m_MoveRemainingTime - DeltaTime, 0.0f);
		FVector currentLocation = this->GetActorLocation();
		this->SetActorLocation(FVector(currentLocation.X, currentLocation.Y, FMath::Lerp(this->m_MoveCameraSrcZ, this->m_MoveCameraDstZ, (this->m_MoveTimeSpan - this->m_MoveRemainingTime) / this->m_MoveTimeSpan)));
	}

	FVector wp, wd;
	if (m_PlayerController != nullptr)
	{
		m_PlayerController->DeprojectMousePositionToWorld(wp, wd);
	}
	
	if (m_buildingsystem != nullptr && m_BuildingBlock != nullptr)
	{
		int position = -wp.X * wd.Z + wp.Z;
		float minD = -1;
		m_BuildingBlock->SetActorLocation(m_buildingsystem->ReturnSelectedPosition(FVector(0, 0, position)));
	}



	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Log, TEXT("Is mouse pressed"));
		float mouseX, mouseY;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mouseX, mouseY);
		
		if (m_IsMoveCamera)
		{
			MoveBuildingCamera((mouseX - m_lastMouseX) * m_inputSensitivity);
		}
		m_lastMouseX = mouseX;
		m_IsMoveCamera = true;
	}
	else
	{
		m_IsMoveCamera = false;
	}
	
}


