// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BaseEquipableItem.h"

ABaseEquipableItem::ABaseEquipableItem()
{
}

void ABaseEquipableItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseEquipableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEquipableItem::OnEquipped(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	if (ItemMesh) ItemMesh->AttachToComponent(InParent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), InSocketName);
}
