// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/APUnUsableTool.h"
#include "Components/StaticMeshComponent.h"

AAPUnUsableTool::AAPUnUsableTool()
{
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Super Mesh"));
	SuperMesh->SetupAttachment(SceneComp);
}

void AAPUnUsableTool::OnSelected()
{
	Super::OnSelected();
	SuperMesh->SetVisibility(true);
}

void AAPUnUsableTool::DeSelected()
{
	Super::DeSelected();

	SuperMesh->SetVisibility(false);
	SuperMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
