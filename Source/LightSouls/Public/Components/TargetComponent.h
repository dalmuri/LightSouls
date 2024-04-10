// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGHTSOULS_API UTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE bool CanBeLockedOn() const { return canBeLockedOn; }
	void SetCanBeLockedOn(bool InCanBeLockedOn);

	FORCEINLINE bool IsLockedOn() const { return isLockedOn; }

	AActor* GetTargetActor() const;

private:
	bool canBeLockedOn = true;
	bool isLockedOn = false;
};
