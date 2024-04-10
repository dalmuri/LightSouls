// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_MeleeAttack UMETA(DisplayName = "Melee Attack"),
	EAT_RangedAttack UMETA(DisplayName = "Ranged Attack"),
	EAT_AOESpell UMETA(DisplayName = "Area of Effect Spell")
};

UENUM(BlueprintType)
enum class EAttackIntensityType : uint8
{
	EAIT_Flinch UMETA(DisplayName = "Flinch"), // ¿òÂñ
	EAIT_KnockBack UMETA(DisplayName = "Knock-Back") // ¹ÐÄ§
	// EAIT_Pratfall UMETA(DisplayName = "Pratfall") // ¾ûµ¢¹æ¾Æ
};

UENUM(BlueprintType)
enum class EPlayerAttackType : uint8
{
	EPAT_LightAttack UMETA(DisplayName = "Light Attack"),
	EPAT_HeavyAttack UMETA(DisplayName = "Heavy Attack"),
	EPAT_RunningAttack UMETA(DisplayName = "Running Attack")
};
