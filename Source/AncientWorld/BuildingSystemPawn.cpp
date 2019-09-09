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
	lookPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LookPoint"));
	lookPoint->SetupAttachment(RootComponent);

	CameraBoon = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoon->SetupAttachment(lookPoint);
	//CameraBoon->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	//CameraBoon->TargetArmLength = 800.f;
	//CameraBoon->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoon->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoon->bInheritPitch = false;
	CameraBoon->bInheritRoll = false;
	CameraBoon->bInheritYaw = false;
	m_MoveRemainingTime = 0;
	m_MoveTimeSpan = 0;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComp->SetupAttachment(CameraBoon, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void ABuildingSystemPawn::BeginPlay()
{
	Super::BeginPlay();
	m_gameStateInstance = static_cast<UAncientWorldGameInstance*>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	m_PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	m_cameraRotationInitialPoint = CameraBoon->RelativeLocation;
	m_intialPoint = GetActorLocation();
	m_cameraAngle = 0.f;
	m_isIntialCamera = false;


}

void ABuildingSystemPawn::MoveBuildingCamera(float axis)
{
	if (this->m_MoveRemainingTime <= 0 && m_buildingsystem != nullptr)
	{
		m_cameraAngle += axis;
		FRotator rot(0, m_cameraAngle, 0);
		//auto result = 
		CameraBoon->RelativeLocation = rot.RotateVector(m_cameraRotationInitialPoint);
		//SetActorLocation(FVector(result.X, result.Y, GetActorLocation().Z));
		CameraBoon->RelativeRotation = UKismetMathLibrary::FindLookAtRotation(CameraBoon->RelativeLocation, FVector(0,0,0));
	}
}

// Called to bind functionality to input
void ABuildingSystemPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("ChangeToBuildingSystem", IE_Pressed, this, &ABuildingSystemPawn::ChangeToBuildingSystem);
	PlayerInputComponent->BindAction("MoveUp", IE_Pressed, this, &ABuildingSystemPawn::MoveUp);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ABuildingSystemPawn::MoveDown);
	PlayerInputComponent->BindAction("RoatetForward", IE_Pressed, this, &ABuildingSystemPawn::RoatetForward);
	PlayerInputComponent->BindAction("RotateBackword", IE_Pressed, this, &ABuildingSystemPawn::RotateBackword);
	PlayerInputComponent->BindAction("BuildAction", IE_Pressed, this, &ABuildingSystemPawn::BuildAction);
	PlayerInputComponent->BindAction("BuildComplete", IE_Pressed, this, &ABuildingSystemPawn::BuildComplete);
	PlayerInputComponent->BindAction("BuildCancellation", IE_Pressed, this, &ABuildingSystemPawn::BuildCancellation);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABuildingSystemPawn::MoveBuildingCamera);
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
	if (m_buildingsystem != nullptr)
	{
		m_CurrentBuildingBlock = static_cast<ABuildingBlockActor*>(GetWorld()->SpawnActor(ABuildingBlockActor::StaticClass()));
		m_buildingsystem->InitialBlockByBuildingID(m_CurrentBuildingBlock, m_buildingsystem->GetCurrentBuildingBlockID(), false);
		// for current build block
		m_CurrentBuildingBlock->SetDirectionID(0);
		m_CurrentBuildingBlock->GetStaticMeshComponent()->SetMaterial(0, m_gameStateInstance->m_materialOnBuild);
	}

}
 
void ABuildingSystemPawn::BuildComplete()
{
	UE_LOG(LogTemp, Log, TEXT("BuildComplete"));
	if (m_buildingsystem != nullptr && m_CurrentBuildingBlock != nullptr)
	{
		m_buildingsystem->ConfirmBuilding(m_CurrentBuildingBlock);
		m_CurrentBuildingBlock = nullptr;
	}
}

void ABuildingSystemPawn::BuildCancellation()
{
	if (m_buildingsystem != nullptr && m_CurrentBuildingBlock != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("BuildCancellation"));
		m_CurrentBuildingBlock->K2_DestroyActor();
		m_CurrentBuildingBlock = nullptr;
	}
}
void ABuildingSystemPawn::ChangeToBuildingSystem()
{
	UE_LOG(LogTemp, Log, TEXT("hahaha"));
	
}

void ABuildingSystemPawn::MoveForBuilding(int direction)
{
	this->m_MoveCameraSrcZ = this->GetActorLocation().Z;
	this->m_MoveTimeSpan = 0.5f;
	this->m_MoveRemainingTime = 0.5f;
	if (direction > 0)
	{
		this->m_MoveCameraDstZ = m_buildingsystem->GetTopZ();
	}
	else
	{
		this->m_MoveCameraDstZ = m_buildingsystem->GetBottomZ();
	}
}

void ABuildingSystemPawn::MoveUp()
{
	if (this->m_MoveRemainingTime <= 0)
	{
		MoveForBuilding(1);
	}
}

void ABuildingSystemPawn::MoveDown()
{
	if (this->m_MoveRemainingTime <= 0)
	{
		MoveForBuilding(-1);
	}
}

void ABuildingSystemPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_isIntialCamera = true;
	if (!m_isIntialCamera)
	{
		m_isIntialCamera = true;
		m_MoveCameraDstZ = m_buildingsystem->GetTopZ();
		FVector currentLocation = this->GetActorLocation();
		this->SetActorLocation(FVector(currentLocation.X, currentLocation.Y, this->m_MoveCameraSrcZ));
	}
	if (this->m_MoveRemainingTime > 0)
	{
		this->m_MoveRemainingTime = FMath::Max(this->m_MoveRemainingTime - DeltaTime, 0.0f);
		FVector currentLocation = this->GetActorLocation();
		//this->SetActorLocation(FVector(currentLocation.X, currentLocation.Y, FMath::Lerp(this->m_MoveCameraSrcZ, this->m_MoveCameraDstZ, (this->m_MoveTimeSpan - this->m_MoveRemainingTime) / this->m_MoveTimeSpan)));
		this->SetActorLocation(FVector(currentLocation.X, currentLocation.Y, FMath::Lerp(this->m_MoveCameraSrcZ, this->m_MoveCameraDstZ, (this->m_MoveTimeSpan - this->m_MoveRemainingTime) / this->m_MoveTimeSpan)));
	}

	FVector wp, wd;
	if (m_PlayerController != nullptr)
	{
		m_PlayerController->DeprojectMousePositionToWorld(wp, wd);
		
	}
	
	if (m_buildingsystem != nullptr && m_CurrentBuildingBlock != nullptr)
	{
		DrawDebugLine(GetWorld(), wp, (GetActorLocation().Z - wp.Z) / wd.Z * wd + wp, FColor::Red, true);
		UE_LOG(LogTemp, Log, TEXT("Is mouse pressed %s, %s, %s"), (*((GetActorLocation().Z - wp.Z) / wd.Z * wd + wp).ToString()), *(wp.ToString()), *(wd.ToString()));
		m_CurrentBuildingBlock->SetActorLocation(m_buildingsystem->ReturnSelectedPosition((GetActorLocation().Z - wp.Z) / wd.Z * wd + wp, m_CurrentBuildingBlock->m_directionID));
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


