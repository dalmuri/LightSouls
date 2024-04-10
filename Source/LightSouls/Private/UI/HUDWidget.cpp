// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"
#include "Components/StatusComponent.h"
#include "Components/ProgressBar.h"

void UHUDWidget::UpdateHUD(EStatusType StatusType, float Value)
{
	if (StatusComponent)
	{
		switch (StatusType)
		{
		case EStatusType::EST_Health:
			if (HealthBar)
			{
				float HealthPercent = StatusComponent->GetCurrentHealth() / StatusComponent->GetMaxHealth();
				HealthBar->SetPercent(HealthPercent);
			}
			break;
		case EStatusType::EST_Stamina:
			if (StaminaBar)
			{
				float StaminaPercent = StatusComponent->GetCurrentStamina() / StatusComponent->GetMaxStamina();
				StaminaBar->SetPercent(StaminaPercent);
			}
			break;
		default:
			break;
		}
	}
}

void UHUDWidget::SetStatusComponent(UStatusComponent* InStatusComponent)
{
	StatusComponent = InStatusComponent;
	StatusComponent->OnStatusValueChanged.AddDynamic(this, &UHUDWidget::UpdateHUD);
}
