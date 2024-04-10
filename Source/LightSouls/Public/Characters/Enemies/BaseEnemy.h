// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/EnemyState.h"
#include "Characters/BaseCharacter.h"
#include "BaseEnemy.generated.h"

class UBehaviorTree;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDeadDelegate);

UCLASS()
class LIGHTSOULS_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ABaseEnemy();

protected:
	virtual void BeginPlay();

public:
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE FText GetEnemyName() const { return EnemyName; }

	virtual void OnGetHit(float Damage, EAttackType AttackType, EAttackIntensityType AttackIntensityType, AActor* DamageCauser, FHitResult HitResult) override;

	virtual float CalculateDamage() override;

	FEnemyDeadDelegate OnEnemyDead;

	FORCEINLINE EEnemyState GetEnemyState() const { return EnemyState; }
	void SetEnemyState(EEnemyState InEnemyState);

	FORCEINLINE UAnimMontage* GetCombatStartMontage() const { return CombatStartMontage; }

	UAnimMontage* GetAttackMontage(int Index) const;

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	FORCEINLINE APlayerCharacter* GetTargetPlayer() const { return TargetPlayer; }
	APlayerCharacter* SearchTargetPlayer();

	//virtual void BeParried();

protected:
	FText EnemyName;
	EEnemyState EnemyState = EEnemyState::EES_Idle;
	

	class UTargetComponent* TargetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* CombatStartMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	float SearchRadius = 2000.f;
	APlayerCharacter* TargetPlayer;
};
