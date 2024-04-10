// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAttackWeaponType : uint8
{
	EAWT_MainWeapon UMETA(DisplayName = "Main Weapon"),
	EAWT_SubWeapon UMETA(DisplayName = "Sub Weapon"),
	EAWT_LeftHand UMETA(DisplayName = "Left Hand"),
	EAWT_RightHand UMETA(DisplayName = "Right Hand")
};
