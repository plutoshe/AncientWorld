// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AncientWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "AncientWorldPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "AncientWorldGameMode.h"
#include "Components/SceneComponent.h"
#include "Public/APToolBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "BuildingSystemPawn.h"
#include "Public/APPickUP.h"
#include "Public/APInteractItemBase.h"

#include "Public/InventoryComponent.h"

#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"

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
void AAncientWorldCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Spawn tools when activates
	//SpawnUsefulTools();
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
	PlayerInputComponent->BindAction("ChangeToBuildingSystem", IE_Pressed, this, &AAncientWorldCharacter::ChangeToBuildingSystem);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AAncientWorldCharacter::OnMouseClick);

}

void AAncientWorldCharacter::ChangeToBuildingSystem()
{
	UE_LOG(LogTemp, Log, TEXT("Change Level to Building System"));
	UGameplayStatics::OpenLevel(GetWorld(), "BuildingSystem");

	AController* controller = GetController();
	controller->UnPossess();
	FVector NewLocation = GetActorLocation() + FVector(0.f, 0.f, 300.f);
	ABuildingSystemPawn* character = GetWorld()->SpawnActor<ABuildingSystemPawn>(ABuildingSystemPawn::StaticClass(), NewLocation, FRotator::ZeroRotator);
	controller->Possess(character);
}


void AAncientWorldCharacter::MoveForward(float axis)
{
	if ((axis > 0.1f || axis < -0.1f)) CancelMoveToLocation();
	FVector dir = CameraComp->GetForwardVector();
	dir.Z = 0;
	AddMovementInput(dir.GetSafeNormal()*axis);
}

void AAncientWorldCharacter::MoveRight(float axis)
{
	if ((axis > 0.1f || axis < -0.1f)) CancelMoveToLocation();

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
/*
#pragma region InventorySystem


void AAncientWorldCharacter::SpawnUsefulTools()
{
	AAncientWorldGameMode* GM = Cast<AAncientWorldGameMode>(GetWorld()->GetAuthGameMode());
	TArray<TSubclassOf<AAPToolBase>> spawnList = GM->GetSpawnToolList();

	for (TSubclassOf<AAPToolBase> toolClass : spawnList)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AAPToolBase* spawnedTool = GetWorld()->SpawnActor<AAPToolBase>(toolClass, InteractPointComp->GetComponentLocation(), InteractPointComp->GetComponentRotation(), ActorSpawnParams);
		// Add fname / spawn tool to the tool base
		m_spawnedToolList.Add(spawnedTool->m_ItemID, spawnedTool);
		// Disable the tool
		spawnedTool->SetOwner(this);
		spawnedTool->DeSelected();
	}
}
void AAncientWorldCharacter::SetSelectingItem(FInventoryItem* _item)
{
	if (_item != nullptr) {
		// Deselected previous one
		if (m_currentItem) {
			AAPToolBase* previous_Tool = *m_spawnedToolList.Find(m_currentItem->ItemID);
			if (previous_Tool) {
				previous_Tool->OnDeSelected();
			}
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("No previous item"));
		}

		// Select the current one
		m_currentItem = _item;
		AAPToolBase* currentTool = *m_spawnedToolList.Find(m_currentItem->ItemID);
		if (currentTool) {
			m_usingTool = currentTool;
			currentTool->OnSelected();
		}

		UE_LOG(LogTemp, Log, TEXT("Using: %s"), *m_currentItem->ItemID.ToString());
	}

}

void AAncientWorldCharacter::ClearItem()
{
	if (m_currentItem != nullptr) {
		// Deselected previous one
		AAPToolBase* previousItem = *m_spawnedToolList.Find(m_currentItem->ItemID);
		if (previousItem) {
			previousItem->OnDeSelected();
		}

		// clear
		m_currentItem = nullptr;
		m_usingTool = nullptr;
		UE_LOG(LogTemp, Log, TEXT("Clear my hand"));

	}
}

void AAncientWorldCharacter::ThrowItem(const FInventoryItem& _spawnItem)
{
	if (!_spawnItem.ItemID.IsEqual(TEXT("None"))) {
		TSubclassOf<AAPPickUP> spawnedClass = _spawnItem.ItemPickUp;
		FActorSpawnParameters spawnPara;
		spawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AAPPickUP* spawnActor = GetWorld()->SpawnActor<AAPPickUP>(spawnedClass,GetActorLocation() + GetActorForwardVector() * 30.f, FRotator::ZeroRotator,spawnPara);

		spawnActor->AddImpulseToOverlapComp(GetActorForwardVector(), 500.f);
	}
}

void AAncientWorldCharacter::AddItemToInventory(FName itemID)
{
	AAncientWorldGameMode* GM = Cast<AAncientWorldGameMode>(GetWorld()->GetAuthGameMode());

	UDataTable* table = GM->GetItemDB();

	FInventoryItem* ItemToAdd = table->FindRow<FInventoryItem>(itemID, "");

	if (ItemToAdd) {
		bool repeatedItem = false;
		int exisitID = 0;
		for (int i = 0; i < Inventory.Num(); i++)
		{
			if (Inventory[i].ItemID.IsEqual(ItemToAdd->ItemID) && ItemToAdd->bCanStack) {
				repeatedItem = true;
				exisitID = i;
				break;
			}
		}
		if (repeatedItem) {
			Inventory[exisitID].Value++;
			OnAddExistingItem(itemID);
			UE_LOG(LogTemp, Log, TEXT("Item [%s] already exist, increase to value [%d]"), *Inventory[exisitID].Name.ToString(), Inventory[exisitID].Value);
		}
		else {
			Inventory.Add(*ItemToAdd);
			OnAddNewItem(*ItemToAdd);
			UE_LOG(LogTemp, Log, TEXT("Add Item [%s] to inventory, it's value is [%d]"), *ItemToAdd->Name.ToString(), ItemToAdd->Value);

		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ItemID"));
	}
}

void AAncientWorldCharacter::SwitchToItem(int slotID)
{
	if (Inventory.Num() <= slotID) { ClearItem(); return; }

	SetSelectingItem(&Inventory[slotID]);
}

v

void AAncientWorldCharacter::RemoveItemFromInventory(FName itemID, int _amount)
{
	FInventoryItem* tempItem = nullptr;
	int idx = -1;
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].ItemID.IsEqual(itemID)) {
			tempItem = &Inventory[i];
			idx = i;
			break;
		}
	}
	if (tempItem != nullptr) {
		tempItem->Value -= _amount;
		UE_LOG(LogTemp, Log, TEXT("%s Amount of item: %d"), *tempItem->ItemID.ToString(), tempItem->Value);

		if (tempItem->Value <= 0) {
			tempItem->Value = 0;

			// if current using one is going to be removed
			if (m_currentItem != nullptr && m_currentItem->ItemID.IsEqual(Inventory[idx].ItemID)) {
				ClearItem();
			}
			// remove the used item
			Inventory.RemoveAt(idx);
			UE_LOG(LogTemp, Log, TEXT("Size of inventory after remove item: %d"), Inventory.Num());
		}
	}


}

#pragma endregion
*/
void AAncientWorldCharacter::InteractWithTool(AAPInteractItemBase* interactBase)
{
	if (interactBase) {
		if (interactBase->m_bRequireTool) {
			if (m_usingTool != nullptr) {
				m_usingTool->StartUse(interactBase);
			}
		}
		else {
			interactBase->Interact();
		}
	}
	else {
		if (m_usingTool != nullptr) {
			m_usingTool->StartUse(nullptr);
		}
	}

}

void AAncientWorldCharacter::OnMouseClick()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	Cast<AAncientWorldPlayerController>(GetController())->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		SetNewMoveDestination(Hit);
	}

}


void AAncientWorldCharacter::SetNewMoveDestination(const FHitResult& outHit)
{
	APawn* const MyPawn = this;
	if (MyPawn)
	{
		AAPInteractItemBase* interactBase = Cast<AAPInteractItemBase>(outHit.Actor);

		if (interactBase) {
			float const Distance = FVector::Dist(outHit.ImpactPoint, MyPawn->GetActorLocation());

			if ((Distance > 120.0f))
			{
				if (!interactBase->GetCanPawnInteract())
					MoveTo(GetController(), outHit.ImpactPoint);
				//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, outHit.ImpactPoint);
			}

			if (interactBase->GetCanPawnInteract()) {
				InteractWithTool(interactBase);
				FRotator newROt = (outHit.Actor->GetActorLocation() - GetActorLocation()).Rotation();
				newROt.Pitch = 0;
				newROt.Roll = 0;
				SetActorRotation(newROt);
			}
		}
		else {
			// not interacting with interactItemBase
			InteractWithTool(nullptr);
		}


	}
}

UPathFollowingComponent* AAncientWorldCharacter::InitNavigationControl(AController& Controller)
{

	AAIController* AsAIController = Cast<AAIController>(&Controller);
	UPathFollowingComponent* PathFollowingComp = nullptr;

	if (AsAIController)
	{
		PathFollowingComp = AsAIController->GetPathFollowingComponent();
	}
	else
	{
		PathFollowingComp = Controller.FindComponentByClass<UPathFollowingComponent>();
		if (PathFollowingComp == nullptr)
		{
			PathFollowingComp = NewObject<UPathFollowingComponent>(&Controller);
			PathFollowingComp->RegisterComponentWithWorld(Controller.GetWorld());
			PathFollowingComp->Initialize();
		}
	}

	return PathFollowingComp;

}

void AAncientWorldCharacter::MoveTo(AController* i_Controller, const FVector& GoalLocation)
{
	UNavigationSystemV1* NavSys = i_Controller ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(i_Controller->GetWorld()) : nullptr;
	if (NavSys == nullptr || i_Controller == nullptr || i_Controller->GetPawn() == nullptr)
	{
		UE_LOG(LogNavigation, Warning, TEXT("UNavigationSystemV1::SimpleMoveToActor called for NavSys:%s i_Controller:%s controlling Pawn:%s (if any of these is None then there's your problem"),
			*GetNameSafe(NavSys), *GetNameSafe(i_Controller), i_Controller ? *GetNameSafe(i_Controller->GetPawn()) : TEXT("NULL"));
		return;
	}

	m_PFollowComp = InitNavigationControl(*i_Controller);
	UE_LOG(LogTemp, Log, TEXT("init: %d"), m_PFollowComp->bIsActive);

	if (m_PFollowComp == nullptr)
	{
		return;
	}

	if (!m_PFollowComp->IsPathFollowingAllowed())
	{
		return;
	}

	const bool bAlreadyAtGoal = m_PFollowComp->HasReached(GoalLocation, EPathFollowingReachMode::OverlapAgent);

	// script source, keep only one move request at time
	if (m_PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		m_PFollowComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::AnyRequest, bAlreadyAtGoal ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
	}

	// script source, keep only one move request at time
	if (m_PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		m_PFollowComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
	}

	if (bAlreadyAtGoal)
	{
		m_PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
	}
	else
	{
		const ANavigationData* NavData = NavSys->GetNavDataForProps(i_Controller->GetNavAgentPropertiesRef());
		if (NavData)
		{
			FPathFindingQuery Query(i_Controller, *NavData, i_Controller->GetNavAgentLocation(), GoalLocation);
			FPathFindingResult Result = NavSys->FindPathSync(Query);
			if (Result.IsSuccessful())
			{
				m_PFollowComp->RequestMove(FAIMoveRequest(GoalLocation), Result.Path);
			}
			else if (m_PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
			{
				m_PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
			}
		}
	}
}

void AAncientWorldCharacter::CancelMoveToLocation()
{
	if (m_PFollowComp && m_PFollowComp->GetStatus() == EPathFollowingStatus::Moving)
		m_PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Aborted);
}
