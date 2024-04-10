// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerCharacterState : uint8
{
	EPS_Idle UMETA(DisplayName = "Idle"),
	EPS_Dead UMETA(DisplayName = "Dead"),
	EPS_Running UMETA(DisplayName = "Running"),
	EPS_Rolling UMETA(DisplayName = "Rolling"),
	EPS_Attacking UMETA(DisplayName = "Attacking"),
	EPS_Parrying UMETA(DisplayName = "Parrying")
};