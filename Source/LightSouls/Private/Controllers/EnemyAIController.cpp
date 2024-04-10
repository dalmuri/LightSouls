// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/EnemyAIController.h"
#include "Characters/Enemies/BaseEnemy.h"

const FName AEnemyAIController::TargetPlayerKey(TEXT("TargetPlayer"));
const FName AEnemyAIController::EnemyStateKey(TEXT("EnemyState"));
const FName AEnemyAIController::IsActingKey(TEXT("IsActing"));

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ABaseEnemy* InEnemy = Cast<ABaseEnemy>(InPawn))
	{
		Enemy = InEnemy;

		RunBehaviorTree(Enemy->GetBehaviorTree());
	}
}

void AEnemyAIController::SetCanRotate(bool InCanRotate)
{
	canRotate = InCanRotate;
}
