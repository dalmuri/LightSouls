// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/StatusType.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatusValueChangedDelegate, EStatusType, StatusType, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaTimerPausedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGHTSOULS_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeStatus();

	FStatusValueChangedDelegate OnStatusValueChanged;

	FORCEINLINE float GetWalkSpeed() const { return WalkSpeed; }
	FORCEINLINE float GetRunSpeed() const { return RunSpeed; }

	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE void SetMaxHealth(float InMaxHealth) { MaxHealth = InMaxHealth; }

	void IncreaseHealth(float Amount);
	void DecreaseHealth(float Amount);

	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE void SetMaxStamina(float InMaxStamina) { MaxStamina = InMaxStamina; }

	void IncreaseStamina(float Amount);
	void DecreaseStamina(float Amount);
	bool IsStaminaFull() const;

	FORCEINLINE float GetDefaultStaminaRegenRate() const { return DefaultStaminaRegenRate; }
	FORCEINLINE float GetGuardStaminaRegenRate() const { return GuardStaminaRegenRate; }

	FStaminaTimerPausedDelegate OnStaminaTimerPaused;
	void ExcuteStaminaTimer(float FirstDelay = -1.f);
	void StopStaminaTimer();
	void PauseStaminaTimer();
	void UnpauseStaminaTimer(float FirstDelay = -1.f);
	bool IsPausedStaminaTimer() const;

	void SetStaminaChangeRate(float InChangeRate);

private:
	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = true))
	float WalkSpeed = 500.f;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = true))
	float RunSpeed = 700.f;

	UPROPERTY(VisibleAnywhere, Category = "Health", meta = (AllowPrivateAccess = true))
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Health", meta = (AllowPrivateAccess = true))
	float CurrentHealth = MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = true))
	float MaxStamina = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = true))
	float CurrentStamina = MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = true))
	float DefaultStaminaRegenRate = 2.f;

	UPROPERTY(VisibleAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = true))
	float GuardStaminaRegenRate = 1.f;

	UPROPERTY(VisibleAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = true))
	float CurrentStaminaChangeRate = DefaultStaminaRegenRate;

	FTimerHandle StaminaTimer;
	void StaminaTimerTick();

	FTimerHandle DelayTimer;
};
