// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask/BTTask_Attack.h"
#include "Controllers/EnemyAIController.h"
#include "Characters/Enemies/BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	UE_LOG(LogTemp, Warning, TEXT("Attack Execute"));

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;

	ABaseEnemy* ControlledEnemy = Cast<ABaseEnemy>(ControlledPawn);
	if (!ControlledEnemy) return EBTNodeResult::Failed;

	int AttackIndex = AttackIndexes[FMath::RandRange(0, AttackIndexes.Num() - 1)];

	if (UAnimMontage* AttackMontage = ControlledEnemy->GetAttackMontage(AttackIndex))
	{
		ControlledEnemy->SetEnemyState(EEnemyState::EES_Attack);
		ControlledEnemy->OnActionEnded.AddUniqueDynamic(this, &UBTTask_Attack::EndAction);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::IsActingKey, true);

		ControlledEnemy->PlayAnimMontage(AttackMontage);
		return EBTNodeResult::InProgress;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (isActionEnded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action Ended"));
		isActionEnded = false;
		Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn())->OnActionEnded.RemoveDynamic(this, &UBTTask_Attack::EndAction);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::IsActingKey, false);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::EnemyStateKey, (uint8)EEnemyState::EES_Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_Attack::EndAction()
{
	UE_LOG(LogTemp, Warning, TEXT("BTTask Attack End Action!!"));
	isActionEnded = true;
}
