// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/AttackWeaponType.h"
#include "Enums/AttackType.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_TraceAttackCollision.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API UANS_TraceAttackCollision : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere)
	EAttackWeaponType AttackWeaponType = EAttackWeaponType::EAWT_MainWeapon;

	UPROPERTY(EditAnywhere)
	EAttackIntensityType AttackIntensityType = EAttackIntensityType::EAIT_Flinch;
};
