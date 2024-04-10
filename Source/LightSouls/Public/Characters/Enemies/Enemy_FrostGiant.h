// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/BaseEnemy.h"
#include "Enemy_FrostGiant.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API AEnemy_FrostGiant : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	AEnemy_FrostGiant();

protected:
	virtual void BeginPlay();

public:
	virtual void Tick(float DeltaTime) override;

	virtual void ToggleTraceAttackCollision(bool Traces, EAttackWeaponType AttackWeaponType = EAttackWeaponType::EAWT_MainWeapon, EAttackIntensityType AttackIntensityType = EAttackIntensityType::EAIT_Flinch) override;

protected:
	UPROPERTY()
	class UWeaponCollisionComponent* LeftHandCollisionComponent;

	UPROPERTY()
	class UWeaponCollisionComponent* RightHandCollisionComponent;

};
