// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateBehavior.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API UBTService_UpdateBehavior : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateBehavior();

	UPROPERTY(EditInstanceOnly)
	float AttackRange;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
