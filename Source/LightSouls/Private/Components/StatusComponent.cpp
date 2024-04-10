// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatusComponent.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeStatus();
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UStatusComponent::InitializeStatus()
{
	CurrentHealth = MaxHealth;
	if (OnStatusValueChanged.IsBound()) OnStatusValueChanged.Broadcast(EStatusType::EST_Health, CurrentHealth);

	CurrentStamina = MaxStamina;
	if (OnStatusValueChanged.IsBound()) OnStatusValueChanged.Broadcast(EStatusType::EST_Stamina, CurrentStamina);
}

void UStatusComponent::IncreaseHealth(float Amount)
{
	float NewHealth = FMath::Clamp(CurrentHealth + Amount, 0, MaxHealth);
	CurrentHealth = NewHealth;
	if (OnStatusValueChanged.IsBound()) OnStatusValueChanged.Broadcast(EStatusType::EST_Health, NewHealth);
}

void UStatusComponent::DecreaseHealth(float Amount)
{
	float NewHealth = FMath::Clamp(CurrentHealth - Amount, 0, MaxHealth);
	CurrentHealth = NewHealth;
	if (OnStatusValueChanged.IsBound()) OnStatusValueChanged.Broadcast(EStatusType::EST_Health, NewHealth);
}

void UStatusComponent::IncreaseStamina(float Amount)
{
	float NewStamina = FMath::Clamp(CurrentStamina + Amount, 0, MaxStamina);
	CurrentStamina = NewStamina;
	if (OnStatusValueChanged.IsBound()) OnStatusValueChanged.Broadcast(EStatusType::EST_Stamina, NewStamina);
}

void UStatusComponent::DecreaseStamina(float Amount)
{
	float NewStamina = FMath::Clamp(CurrentStamina - Amount, 0, MaxStamina);
	CurrentStamina = NewStamina;
	if (OnStatusValueChanged.IsBound()) OnStatusValueChanged.Broadcast(EStatusType::EST_Stamina, NewStamina);
}

bool UStatusComponent::IsStaminaFull() const
{
	return CurrentStamina >= MaxStamina;
}

void UStatusComponent::ExcuteStaminaTimer(float FirstDelay)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(StaminaTimer)) StopStaminaTimer(); // StaminaTimer가 활성화되어 있으면 해제

	TimerManager.SetTimer(StaminaTimer, this, &UStatusComponent::StaminaTimerTick, 0.1f, true, FirstDelay);
}

void UStatusComponent::StopStaminaTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);

	if (OnStaminaTimerPaused.IsBound()) OnStaminaTimerPaused.Broadcast();
}

void UStatusComponent::PauseStaminaTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(DelayTimer)) TimerManager.ClearTimer(DelayTimer);
	if (TimerManager.IsTimerActive(StaminaTimer)) TimerManager.PauseTimer(StaminaTimer);
}

void UStatusComponent::UnpauseStaminaTimer(float FirstDelay)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (FirstDelay > 0)
	{
		if (TimerManager.IsTimerPaused(StaminaTimer))
		{
			TimerManager.SetTimer(DelayTimer, FTimerDelegate::CreateLambda([&]()
				{
					TimerManager.UnPauseTimer(StaminaTimer);
				}), FirstDelay, false);
		}
	}
	else
	{
		TimerManager.UnPauseTimer(StaminaTimer);
	}
	
}

bool UStatusComponent::IsPausedStaminaTimer() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	return TimerManager.IsTimerPaused(StaminaTimer);
}

void UStatusComponent::SetStaminaChangeRate(float InChangeRate)
{
	CurrentStaminaChangeRate = InChangeRate;
}

void UStatusComponent::StaminaTimerTick()
{
	if (CurrentStaminaChangeRate > 0)
	{
		IncreaseStamina(CurrentStaminaChangeRate);

		if (IsStaminaFull())
		{
			PauseStaminaTimer();
		}
	}
	else if (CurrentStaminaChangeRate < 0)
	{
		DecreaseStamina(CurrentStaminaChangeRate * -1);

		if (CurrentStamina <= 0)
		{
			PauseStaminaTimer();
		}
	}
}
