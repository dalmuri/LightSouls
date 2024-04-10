// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/BaseWeapon.h"
#include "Characters/BaseCharacter.h"
#include "Components/WeaponCollisionComponent.h"

ABaseWeapon::ABaseWeapon()
{
	EquipableSocketName = FName(TEXT("RightHandSocket"));

	WeaponCollisionComponent = CreateDefaultSubobject<UWeaponCollisionComponent>(TEXT("WeaponCollisionComponent"));
	WeaponCollisionComponent->SetWeaponMeshComponent(ItemMesh);
	WeaponCollisionComponent->AddActorToIgnoreCollision(this);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::OnEquipped(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	Super::OnEquipped(InParent, InSocketName, NewOwner, NewInstigator);

	if (WeaponCollisionComponent)
	{
		WeaponCollisionComponent->AddActorToIgnoreCollision(NewOwner);
		WeaponCollisionComponent->SetOwnerActor(Cast<ABaseCharacter>(NewOwner));
	}
}

UAnimMontage* ABaseWeapon::GetAttackMontage(EPlayerAttackType PlayerAttackType, int AttackCount)
{
	UAnimMontage* AttackMontage;
	switch (PlayerAttackType)
	{
	case EPlayerAttackType::EPAT_LightAttack:
	default:
		AttackMontage = LightAttackMontages[AttackCount];
		break;
	case EPlayerAttackType::EPAT_HeavyAttack:
		AttackMontage = HeavyAttackMontage;
		break;
	case EPlayerAttackType::EPAT_RunningAttack:
		AttackMontage = RunningAttackMontage;
		break;
	}
	return AttackMontage;
}

void ABaseWeapon::ToggleTraceCollision(bool Traces)
{
	if (WeaponCollisionComponent)
	{
		WeaponCollisionComponent->ToggleTraceCollision(Traces);
	}
}
