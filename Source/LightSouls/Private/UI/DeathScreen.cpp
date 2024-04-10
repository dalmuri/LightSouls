// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DeathScreen.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UDeathScreen::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnClicked.AddDynamic(this, &UDeathScreen::RestartButtonCallback);

	PlayAnimation(FadeIn);
}

void UDeathScreen::RestartButtonCallback()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("FrozenRock")));
}
