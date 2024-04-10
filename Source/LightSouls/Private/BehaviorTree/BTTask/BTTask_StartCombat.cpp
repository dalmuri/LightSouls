// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask/BTTask_StartCombat.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Controllers/EnemyAIController.h"
#include "Characters/Enemies/BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_StartCombat::UBTTask_StartCombat()
{
	NodeName = TEXT("Start Combat");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_StartCombat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;

	ABaseEnemy* ControlledEnemy = Cast<ABaseEnemy>(ControlledPawn);
	if (!ControlledEnemy) return EBTNodeResult::Failed;

	if (UAnimMontage* CombatStartMontage = ControlledEnemy->GetCombatStartMontage())
	{
		ControlledEnemy->SetEnemyState(EEnemyState::EES_CombatStart);
		ControlledEnemy->OnActionEnded.AddUniqueDynamic(this, &UBTTask_StartCombat::EndAction);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::IsActingKey, true);

		ControlledEnemy->PlayAnimMontage(CombatStartMontage);
		return EBTNodeResult::InProgress;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

void UBTTask_StartCombat::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (isActionEnded)
	{
		isActionEnded = false;
		Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn())->OnActionEnded.RemoveDynamic(this, &UBTTask_StartCombat::EndAction);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::IsActingKey, false);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::EnemyStateKey, (uint8)EEnemyState::EES_Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_StartCombat::EndAction()
{
	isActionEnded = true;
}
