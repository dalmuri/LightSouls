// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EDirection : uint8
{
	ED_Front UMETA(DisplayName = "Front"),
	ED_Right UMETA(DisplayName = "Right"),
	ED_Back UMETA(DisplayName = "Back"),
	ED_Left UMETA(DisplayName = "Left")
};