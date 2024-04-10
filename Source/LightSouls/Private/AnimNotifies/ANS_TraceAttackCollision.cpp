// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ANS_TraceAttackCollision.h"
#include "Characters/BaseCharacter.h"

void UANS_TraceAttackCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		Character->ToggleTraceAttackCollision(true, AttackWeaponType, AttackIntensityType);
	}
}

void UANS_TraceAttackCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		Character->ToggleTraceAttackCollision(false, AttackWeaponType);
	}
}
