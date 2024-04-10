// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Shields/BaseShield.h"

ABaseShield::ABaseShield()
{
	EquipableSocketName = FName(TEXT("LeftArmSocket"));
}

void ABaseShield::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAnimMontage* ABaseShield::GetParryMontage()
{
	return ParryMontage;
}

UAnimMontage* ABaseShield::GetGuardHitReactionMontage()
{
	return GuardHitReactionMontage;
}
