// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Classes/GameFramework/Character.h"

// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	currentLevelName = FName(TEXT(""));
}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelManager::LoadLevel(FName levelName)
{
	FLatentActionInfo LatentInfo;

	UGameplayStatics::LoadStreamLevel(this, levelName, true, true, LatentInfo);

	UGameplayStatics::UnloadStreamLevel(this, currentLevelName, CurrentLatentInfo, true);

	UE_LOG(LogTemp, Log, TEXT("currentLevelName: %s"), *(currentLevelName.ToString()));
	CurrentLatentInfo = LatentInfo;
	currentLevelName = levelName;
	UE_LOG(LogTemp, Log, TEXT("after currentLevelName: %s"), *(currentLevelName.ToString()));
	//UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->K2_DestroyActor();
}

