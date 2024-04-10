// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/AttackType.h"
#include "Items/BaseEquipableItem.h"
#include "BaseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ABaseWeapon : public ABaseEquipableItem
{
	GENERATED_BODY()

public:
	ABaseWeapon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnEquipped(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator) override;

	UAnimMontage* GetAttackMontage(EPlayerAttackType PlayerAttackType, int AttackCount = 0);

	FORCEINLINE int GetNumOfLightAttackMontages() { return LightAttackMontages.Num(); }

	void ToggleTraceCollision(bool Traces);

	float Damage = 10.f;

protected:
	UPROPERTY()
	class UWeaponCollisionComponent* WeaponCollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TArray<UAnimMontage*> LightAttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* HeavyAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* RunningAttackMontage;
};
