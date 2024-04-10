// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyHealthBar.h"
#include "Characters/Enemies/BaseEnemy.h"
#include "Components/StatusComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyHealthBar::UpdateHealthBar(EStatusType StatusType, float Value)
{
	if (StatusComponent && StatusType == EStatusType::EST_Health)
	{
		float HealthPercent = StatusComponent->GetCurrentHealth() / StatusComponent->GetMaxHealth();
		HealthBar->SetPercent(HealthPercent);
	}
}

void UEnemyHealthBar::SetEnemy(ABaseEnemy* Enemy)
{
	StatusComponent = Enemy->GetStatusComponent();
	StatusComponent->OnStatusValueChanged.AddDynamic(this, &UEnemyHealthBar::UpdateHealthBar);

	UpdateHealthBar(EStatusType::EST_Health, StatusComponent->GetCurrentHealth());

	EnemyNameText->SetText(Enemy->GetEnemyName());
}
