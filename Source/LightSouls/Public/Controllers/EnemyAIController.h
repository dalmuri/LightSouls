// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	static const FName TargetPlayerKey;
	static const FName EnemyStateKey;
	static const FName IsActingKey;

	FORCEINLINE bool CanRotate() const { return canRotate; }
	void SetCanRotate(bool InCanRotate);
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

	class ABaseEnemy* Enemy;

	bool canRotate = false;
};
