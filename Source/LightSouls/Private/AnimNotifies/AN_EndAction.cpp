// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_EndAction.h"
#include "Characters/BaseCharacter.h"

void UAN_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		Character->EndAction();
	}
}
