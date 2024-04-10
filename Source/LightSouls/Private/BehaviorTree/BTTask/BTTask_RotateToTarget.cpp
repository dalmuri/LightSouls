// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask/BTTask_RotateToTarget.h"
#include "Controllers/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/BaseEnemy.h"
#include "Characters/PlayerCharacter.h"

UBTTask_RotateToTarget::UBTTask_RotateToTarget()
{
	NodeName = TEXT("Rotate to Target");
}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseEnemy* Enemy = Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetPlayerKey));
	if (!TargetPlayer) return EBTNodeResult::Failed;

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(Enemy->GetController());
	if (!EnemyAIController) return EBTNodeResult::Failed;

	if (EnemyAIController->CanRotate())
	{
		FVector LookVec = TargetPlayer->GetActorLocation() - Enemy->GetActorLocation();
		LookVec.Z = 0.f;

		FRotator EnemyRot = Enemy->GetActorRotation();
		FRotator TargetRot = FRotationMatrix::MakeFromX(LookVec).Rotator();

		Enemy->SetActorRotation(FMath::RInterpTo(EnemyRot, TargetRot, GetWorld()->GetDeltaSeconds(), 2.f));
	}

	return EBTNodeResult::Succeeded;
	
}
