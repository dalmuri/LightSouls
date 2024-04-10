// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ANS_RotateToTarget.h"
#include "Characters/Enemies/BaseEnemy.h"
#include "Controllers/EnemyAIController.h"

void UANS_RotateToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (ABaseEnemy* EnemyCharacter = Cast<ABaseEnemy>(MeshComp->GetOwner()))
	{
		if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(EnemyCharacter->GetController()))
		{
			EnemyAIController->SetCanRotate(true);
		}
	}
}

void UANS_RotateToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ABaseEnemy* EnemyCharacter = Cast<ABaseEnemy>(MeshComp->GetOwner()))
	{
		if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(EnemyCharacter->GetController()))
		{
			EnemyAIController->SetCanRotate(false);
		}
	}
}
