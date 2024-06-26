// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetIsActing.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API UBTTask_SetIsActing : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_SetIsActing();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditInstanceOnly)
	bool InIsActing = false;
};
