// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/AttackType.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_AOEAttack.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API UAN_AOEAttack : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	UPROPERTY(EditInstanceOnly)
	FName LocationSocketName;

	UPROPERTY(EditInstanceOnly)
	float Radius;

	UPROPERTY(EditInstanceOnly)
	EAttackType AttackType = EAttackType::EAT_AOESpell;

	UPROPERTY(EditInstanceOnly)
	EAttackIntensityType AttackIntensityType = EAttackIntensityType::EAIT_Flinch;
};
