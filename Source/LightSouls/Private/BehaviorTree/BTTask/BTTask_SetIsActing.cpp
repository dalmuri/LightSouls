// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask/BTTask_SetIsActing.h"
#include "Controllers/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetIsActing::UBTTask_SetIsActing()
{
	NodeName = TEXT("Set IsActing");
}

EBTNodeResult::Type UBTTask_SetIsActing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::IsActingKey, InIsActing);
	return EBTNodeResult::Succeeded;
}
