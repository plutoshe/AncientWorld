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
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "AncientWorldGameInstance.h"



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
	m_gameStateInstance = static_cast<UAncientWorldGameInstance*>(UGameplayStatics::GetGameInstance(GetWorld()));
	for (int i = 0; i < m_gameStateInstance->m_baseStatus.Num(); i++)
	{
		
		AStaticMeshActor* newBlcock = static_cast<AStaticMeshActor*>(GetWorld()->SpawnActor(AStaticMeshActor::StaticClass()));
		newBlcock->SetMobility(EComponentMobility::Movable);
		newBlcock->GetStaticMeshComponent()->SetStaticMesh(m_gameStateInstance->m_buildings[m_gameStateInstance->m_baseStatus[i].m_buildingBlockIndex].m_mesh);
		newBlcock->GetStaticMeshComponent()->SetMaterial(0, m_gameStateInstance->m_buildings[m_gameStateInstance->m_baseStatus[i].m_buildingBlockIndex].m_material);
		FVector result = m_gameStateInstance->m_BasePoint + m_gameStateInstance->m_baseStatus[i].m_positionIndexForBase * m_gameStateInstance->m_BaseLayerLength;
		UE_LOG(LogTemp, Log, TEXT("%.2f %.2f %.2f %.2f"), m_gameStateInstance->m_baseStatus[i].m_positionIndexForBase.Z, result.X, result.Y, result.Z);
		newBlcock->SetActorLocation(m_gameStateInstance->m_BasePoint + m_gameStateInstance->m_baseStatus[i].m_positionIndexForBase * m_gameStateInstance->m_BaseLayerLength);
	}

	m_MoveCameraDst = this->GetActorLocation();
	m_BuildingSlot.Init(2, 2);
	m_BuildingSlot[0] = 2;
	m_BuildingSlot[1] = 4;
	//m_BuildingSlot.Add(2.0f);
	m_PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Super::BeginPlay();
	
	
}

// Called to bind functionality to input
void ABuildingSystemPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("ChangeToBuildingSystem", IE_Pressed, this, &ABuildingSystemPawn::ChangeToBuildingSystem);
	PlayerInputComponent->BindAction("MoveUp", IE_Pressed, this, &ABuildingSystemPawn::MoveUp);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ABuildingSystemPawn::MoveDown);
	PlayerInputComponent->BindAction("BuildAction", IE_Pressed, this, &ABuildingSystemPawn::BuildAction);
	//PlayerInputComponent->BindAction("BuildComplete", IE_Pressed, this, &ABuildingSystemPawn::BuildComplete);
	PlayerInputComponent->BindAction("BuildCancellation", IE_Pressed, this, &ABuildingSystemPawn::BuildCancellation);


}
void ABuildingSystemPawn::BuildAction()
{
	if (m_BuildingBlock == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("BuildAction"));
		m_BuildingBlock = static_cast<AStaticMeshActor*>(GetWorld()->SpawnActor(AStaticMeshActor::StaticClass()));
		m_BuildingBlock->SetMobility(EComponentMobility::Movable);
		m_BuildingBlock->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
		m_BuildingBlock->GetStaticMeshComponent()->SetStaticMesh(m_gameStateInstance->GetCurrentBuildingBlock()->m_mesh);
		m_BuildingBlock->GetStaticMeshComponent()->SetMaterial(0, m_gameStateInstance->m_materialOnBuild);
	}

}
 
void ABuildingSystemPawn::BuildComplete(UStaticMesh* mesh, UMaterial* mat)
{
	if (m_BuildingBlock)
	{
		UE_LOG(LogTemp, Log, TEXT("BuildComplete"));
		m_BuildingBlock->GetStaticMeshComponent()->SetStaticMesh(mesh);
		m_BuildingBlock->GetStaticMeshComponent()->SetMaterial(0, mat);
		m_BuildingBlock = nullptr;
		if (m_select == 0)
		{
			m_BuildingSlot[0]--;
		}
		else
		{
			m_BuildingSlot[1]++;
		}
	}
}

void ABuildingSystemPawn::BuildCancellation()
{
	if (m_BuildingBlock != nullptr)
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
	if (!(this->m_MoveCameraDst.Z >= 1000 && direction > 0 || this->m_MoveCameraDst.Z < -700 && direction < 0))
	{
		this->m_MoveCameraSrc = this->GetActorLocation();
		this->m_MoveCameraDst = this->m_MoveCameraDst + FVector(0, 0, 200 * direction);
		this->m_MoveTimeSpan = 0.5f + this->m_MoveRemainingTime;
		this->m_MoveRemainingTime = 0.5f + this->m_MoveRemainingTime;
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
		//this->SetActorLocation(FVector(0, 0, 1000));
		this->SetActorLocation(FMath::Lerp(this->m_MoveCameraSrc, this->m_MoveCameraDst, (this->m_MoveTimeSpan - this->m_MoveRemainingTime) / this->m_MoveTimeSpan));
		/*FMath::FInterpTo()
		(
				CameraBoon->TargetArmLength,
				desiredBoonLength,
				deltaTime,
				CameraZoomSpeed);*/
	}
	float mouseX;
	float mouseY;
	int32 viewportX;
	int32 viewportY;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mouseX, mouseY);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewportSize(viewportX, viewportY);
	FVector wp, wd;
	if (m_PlayerController != nullptr)
	{
		m_PlayerController->DeprojectMousePositionToWorld(wp, wd);
		//UE_LOG(LogTemp, Log, TEXT("mouse: (%.2f %.2f %.2f) (%.2f %.2f %.2f )"), wp.X, wp.Y, wp.Z, wd.X, wd.Y, wd.Z);
	}
	
	if (m_BuildingBlock != nullptr)
	{
		int position = -wp.X * wd.Z + wp.Z;
		float minD = -1;
		for (int i = 0; i < m_BuildingSlot.Num(); i++)
		{
			if (minD == -1 || FMath::Abs(m_BuildingSlot[i] * 100 + 50 - position) < minD)
			{
				minD = FMath::Abs(m_BuildingSlot[i] * 100 + 50 - position);
				m_BuildingBlock->SetActorLocation(FVector(0, 100, m_BuildingSlot[i] * 100 + 50));
				m_select = i;
			}
		}
		
	}
}


