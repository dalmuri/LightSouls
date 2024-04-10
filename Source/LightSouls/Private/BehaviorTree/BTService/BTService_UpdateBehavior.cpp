// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTService/BTService_UpdateBehavior.h"
#include "Controllers/EnemyAIController.h"
#include "Characters/Enemies/BaseEnemy.h"
#include "Characters/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enums/EnemyState.h"

UBTService_UpdateBehavior::UBTService_UpdateBehavior()
{
	NodeName = "BTService Update Behavior";
	Interval = 0.5f;
	RandomDeviation = 0.2f;

}

void UBTService_UpdateBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerComp.GetAIOwner()) return;

	ABaseEnemy* EnemyCharacter = Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyCharacter) return;

	if (!EnemyCharacter->IsAlive())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::EnemyStateKey, (uint8)EEnemyState::EES_Dead);
		return;
	}

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(AEnemyAIController::IsActingKey)) return; // IsActing == true일 경우 EnemyState를 변경하지 않고 대기

	APlayerCharacter* TargetPlayer = EnemyCharacter->GetTargetPlayer();
	if (!TargetPlayer)
	{
		TargetPlayer = EnemyCharacter->SearchTargetPlayer();
		if (!TargetPlayer)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::EnemyStateKey, (uint8)EEnemyState::EES_Idle);
			return;
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetPlayerKey, TargetPlayer);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::EnemyStateKey, (uint8)EEnemyState::EES_CombatStart);
		return;
	}

	float Distance = EnemyCharacter->GetDistanceTo(TargetPlayer);

	if (AttackRange < Distance)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::EnemyStateKey, (uint8)EEnemyState::EES_Chase);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::EnemyStateKey, (uint8)EEnemyState::EES_Attack);
	}

}
