// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StartCombat.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API UBTTask_StartCombat : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_StartCombat();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void EndAction();

private:
	bool isActionEnded = false;
};
