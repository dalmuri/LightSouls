// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "BaseEquipableItem.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ABaseEquipableItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ABaseEquipableItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnEquipped(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	FORCEINLINE FName GetEquipableSocketName() const { return EquipableSocketName; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip")
	FName EquipableSocketName;
};
