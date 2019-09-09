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
#include "Public/BuildingBlockActor.h"
#include "Engine/Public/DrawDebugHelpers.h"
// Sets default values
ABuildingSystemPawn::ABuildingSystemPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	

	PrimaryActorTick.bCanEverTick = true;
	OriginPoint = CreateDefaultSubobject<USceneComponent>(TEXT("OriginPoint"));
	OriginPoint->SetupAttachment(RootComponent);

	TopCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("TopCameraBoon"));
	TopCameraBoom->SetupAttachment(OriginPoint);
	TopCameraBoom->bDoCollisionTest = false;
	TopCameraBoom->bInheritPitch = false;
	TopCameraBoom->bInheritRoll = false;
	TopCameraBoom->bInheritYaw = false;

	BottomCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("BottomCameraBoon"));
	BottomCameraBoom->SetupAttachment(OriginPoint);
	BottomCameraBoom->bDoCollisionTest = false;
	BottomCameraBoom->bInheritPitch = false;
	BottomCameraBoom->bInheritRoll = false;
	BottomCameraBoom->bInheritYaw = false;

	TopCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TopCameraComp"));
	TopCameraComp->SetupAttachment(TopCameraBoom, USpringArmComponent::SocketName);
	TopCameraComp->bUsePawnControlRotation = false;

	BottomCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("BottomCameraComp"));
	BottomCameraComp->SetupAttachment(BottomCameraBoom, USpringArmComponent::SocketName);
	BottomCameraComp->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ABuildingSystemPawn::BeginPlay()
{
	Super::BeginPlay();

	TopCameraComp->SetActive(true);
	BottomCameraComp->SetActive(false);

	m_GameStateInstance = static_cast<UAncientWorldGameInstance*>(UGameplayStatics::GetGameInstance(GetWorld()));
	m_CameraRotationInitialTopPoint = TopCameraBoom->RelativeLocation;
	m_CameraRotationInitialBottomPoint = BottomCameraBoom->RelativeLocation;

	m_PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	m_CameraAngle = 0.f;
	m_isIntialCamera = false;

}

void ABuildingSystemPawn::RotateBuildingCamera(float axis)
{
	if (m_BuildingSystem != nullptr)
	{
		m_CameraAngle += axis;
		FRotator rot(0, m_CameraAngle, 0);
		FHitResult tmp;
		TopCameraBoom->K2_SetRelativeLocation(rot.RotateVector(m_CameraRotationInitialTopPoint), false, tmp, true);
		TopCameraBoom->RelativeRotation = UKismetMathLibrary::FindLookAtRotation(TopCameraBoom->RelativeLocation, FVector(0,0,0));

		BottomCameraBoom->K2_SetRelativeLocation(rot.RotateVector(m_CameraRotationInitialBottomPoint), false, tmp, true);
		BottomCameraBoom->RelativeRotation = UKismetMathLibrary::FindLookAtRotation(TopCameraBoom->RelativeLocation, FVector(0, 0, 0));

	}
}

// Called to bind functionality to input
void ABuildingSystemPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("MoveUp", IE_Pressed, this, &ABuildingSystemPawn::MoveUp);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ABuildingSystemPawn::MoveDown);
	PlayerInputComponent->BindAction("RoatetForward", IE_Pressed, this, &ABuildingSystemPawn::RoatetForward);
	PlayerInputComponent->BindAction("RotateBackword", IE_Pressed, this, &ABuildingSystemPawn::RotateBackword);
	PlayerInputComponent->BindAction("BuildAction", IE_Pressed, this, &ABuildingSystemPawn::BuildAction);
	PlayerInputComponent->BindAction("BuildComplete", IE_Pressed, this, &ABuildingSystemPawn::BuildComplete);
	PlayerInputComponent->BindAction("BuildCancellation", IE_Pressed, this, &ABuildingSystemPawn::BuildCancellation);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABuildingSystemPawn::RotateBuildingCamera);
}

void ABuildingSystemPawn::RoatetForward()
{
	if (m_CurrentBuildingBlock != nullptr)
	{
		m_CurrentBuildingBlock->UpdateDirectionIDOffset(1);
	}
}

void ABuildingSystemPawn::RotateBackword()
{
	if (m_CurrentBuildingBlock != nullptr)
	{
		m_CurrentBuildingBlock->UpdateDirectionIDOffset(-1);
	}
}


void ABuildingSystemPawn::BuildAction()
{
	if (m_BuildingSystem != nullptr)
	{
		m_CurrentBuildingBlock = static_cast<ABuildingBlockActor*>(GetWorld()->SpawnActor(ABuildingBlockActor::StaticClass()));
		m_BuildingSystem->InitialBlockByBuildingID(m_CurrentBuildingBlock, m_BuildingSystem->GetBuildingBlockCurrentBuildingEntityID(), false);
		// for current build block
		m_CurrentBuildingBlock->SetBuildingEntityIndex(m_BuildingSystem->GetBuildingBlockCurrentBuildingEntityID());
		m_CurrentBuildingBlock->SetDirectionID(0);
	}

}
 
void ABuildingSystemPawn::BuildComplete()
{
	UE_LOG(LogTemp, Log, TEXT("BuildComplete"));
	if (m_BuildingSystem != nullptr && m_CurrentBuildingBlock != nullptr && m_BuildingSystem->BuildingAvailability(*m_CurrentBuildingBlock))
	{
		m_BuildingSystem->ConfirmBuilding(m_CurrentBuildingBlock);
		m_CurrentBuildingBlock = nullptr;
	}
}

void ABuildingSystemPawn::BuildCancellation()
{
	if (m_BuildingSystem != nullptr && m_CurrentBuildingBlock != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("BuildCancellation"));
		m_CurrentBuildingBlock->K2_DestroyActor();
		m_CurrentBuildingBlock = nullptr;
	}
}

void ABuildingSystemPawn::MoveForBuilding(int direction)
{
	if (direction > 0)
	{
		this->m_MoveCameraDstZ = m_BuildingSystem->GetTopZ();
		m_IsTopCamera = true;
	}
	else
	{
		this->m_MoveCameraDstZ = m_BuildingSystem->GetBottomZ();
		m_IsTopCamera = false;
		
	}
	TopCameraComp->SetActive(m_IsTopCamera);
	BottomCameraComp->SetActive(!m_IsTopCamera);
	FVector currentLocation = this->GetActorLocation();
	this->SetActorLocation(FVector(currentLocation.X, currentLocation.Y, this->m_MoveCameraDstZ));
}

void ABuildingSystemPawn::MoveUp()
{
	MoveForBuilding(1);
}

void ABuildingSystemPawn::MoveDown()
{
	MoveForBuilding(-1);
}

void ABuildingSystemPawn::UpdateCurrentBuildingBlock()
{
	//DrawDebugLine(GetWorld(), TopCameraComp->relative, GetActorLocation(), FColor::Green, true);
	if (m_BuildingSystem != nullptr && m_CurrentBuildingBlock != nullptr && m_PlayerController != nullptr)
	{
		FVector mouseStartPosition, mouseDirection;
		m_PlayerController->DeprojectMousePositionToWorld(mouseStartPosition, mouseDirection);
		// update location
		m_CurrentBuildingBlock->SetIndexPosition(
			m_BuildingSystem->ReturnSelectedIndexPosition(
			(GetActorLocation().Z + -mouseStartPosition.Z) / mouseDirection.Z * mouseDirection + mouseStartPosition,
				m_CurrentBuildingBlock->m_DirectionID));
		m_CurrentBuildingBlock->SetActorLocation(
			m_GameStateInstance->GetBuildingPositoinFromIndex(
				m_BuildingSystem->m_basePoint, m_CurrentBuildingBlock->m_IndexPosition,
				m_CurrentBuildingBlock->m_BuildingEntityId, m_CurrentBuildingBlock->m_DirectionID)
		);
		// update material(success/failure)
		m_BuildingSystem->BuildingAvailability(*m_CurrentBuildingBlock);
	}
}

void ABuildingSystemPawn::UpdateCamera()
{
	// Rotate Camera by Mouse X axis
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::LeftMouseButton))
	{
		float mouseX, mouseY;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mouseX, mouseY);

		if (m_IsRotateCamera)
		{
			RotateBuildingCamera((mouseX - m_LastMouseX) * m_InputSensitivity);
		}
		m_LastMouseX = mouseX;
		m_IsRotateCamera = true;
	}
	else
	{
		m_IsRotateCamera = false;
	}
}

void ABuildingSystemPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_isIntialCamera = true;
	if (!m_isIntialCamera)
	{
		m_isIntialCamera = true;
		MoveUp();
	}
	UpdateCurrentBuildingBlock();
	UpdateCamera();
}



