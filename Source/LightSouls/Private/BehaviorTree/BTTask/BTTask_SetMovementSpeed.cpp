// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask/BTTask_SetMovementSpeed.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_SetMovementSpeed::UBTTask_SetMovementSpeed()
{
	NodeName = TEXT("Set Movement Speed");
}

EBTNodeResult::Type UBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;

	ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
	if (!ControlledCharacter) return EBTNodeResult::Failed;

	ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	return EBTNodeResult::Succeeded;
}

FString UBTTask_SetMovementSpeed::GetStaticDescription() const
{
	return FString::Printf(TEXT("Movement Speed: %.2f"), MovementSpeed);
}
