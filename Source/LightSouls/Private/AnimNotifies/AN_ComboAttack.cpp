// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_ComboAttack.h"
#include "Characters/PlayerCharacter.h"

void UAN_ComboAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		PlayerCharacter->ComboAttack();
	}
}
