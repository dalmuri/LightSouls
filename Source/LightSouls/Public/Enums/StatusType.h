// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EStatusType : uint8
{
	EST_Health UMETA(DisplayName = "Health"),
	EST_Stamina UMETA(DisplayName = "Stamina")
};
