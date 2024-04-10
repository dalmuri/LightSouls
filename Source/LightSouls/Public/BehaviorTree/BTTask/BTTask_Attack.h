// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void EndAction();

	UPROPERTY(EditInstanceOnly)
	TArray<int> AttackIndexes;

private:
	bool isActionEnded = false;
};
