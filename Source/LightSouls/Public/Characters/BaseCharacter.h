// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Combatable.h"
#include "Enums/Direction.h"
#include "Enums/AttackType.h"
#include "Enums/AttackWeaponType.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionEndedDelegate);

class UStatusComponent;

UCLASS()
class LIGHTSOULS_API ABaseCharacter : public ACharacter, public ICombatable
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ToggleTraceAttackCollision(bool Traces, EAttackWeaponType AttackWeaponType = EAttackWeaponType::EAWT_MainWeapon, EAttackIntensityType AttackIntensityType = EAttackIntensityType::EAIT_Flinch);

	virtual void OnGetHit(float Damage, EAttackType AttackType, EAttackIntensityType AttackIntensityType, AActor* DamageCauser, FHitResult HitResult) override;
	virtual float CalculateDamage();
	virtual void ReceiveDamage(float InDamage);

	virtual void OnGetParried() override;

	virtual void EndAction();
	FActionEndedDelegate OnActionEnded;

	virtual bool CanBeHit() const { return canBeHit; }
	FORCEINLINE bool IsAlive() const { return isAlive; }

	EDirection CalculateDirection(AActor* Target) const;
	EDirection CalculateDirection(FVector TargetLocation) const;

	FORCEINLINE UStatusComponent* GetStatusComponent() const { return StatusComponent; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* StaggerMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	class UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	float HitParticleScale = 1.f;

	UPROPERTY()
	UStatusComponent* StatusComponent;

	UPROPERTY()
	bool canBeHit = true;

	UPROPERTY()
	bool isAlive = true;

	virtual void Die();
};
