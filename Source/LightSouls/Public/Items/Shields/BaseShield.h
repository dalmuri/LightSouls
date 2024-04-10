// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseEquipableItem.h"
#include "BaseShield.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ABaseShield : public ABaseEquipableItem
{
	GENERATED_BODY()
	
public:
	ABaseShield();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UAnimMontage* GetParryMontage();

	UAnimMontage* GetGuardHitReactionMontage();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* GuardHitReactionMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* ParryMontage;
};
