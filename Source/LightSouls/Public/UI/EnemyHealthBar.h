// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/StatusType.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBar.generated.h"

class UStatusComponent;

UCLASS()
class LIGHTSOULS_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateHealthBar(EStatusType StatusType, float Value);

	void SetEnemy(class ABaseEnemy* Enemy);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EnemyNameText;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UStatusComponent* StatusComponent;
	
};
