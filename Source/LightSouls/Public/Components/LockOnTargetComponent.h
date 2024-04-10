// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnTargetComponent.generated.h"

class UTargetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGHTSOULS_API ULockOnTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockOnTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void LockOn();
	void LockOn(APawn* InPawn);
	void LockOn(UTargetComponent* InTarget);

	void StopLockOn();

	FORCEINLINE bool IsLockingOn() const { return isLockingOn; }

	void RotateToTarget();

private:
	bool isActorOnScreen(AActor* InTarget);

	TArray<UTargetComponent*> SearchTargets();
	UTargetComponent* Target;

	void LookAtTarget(float DeltaTime);

	FRotator CalculateCameraRotation(float DeltaTime);
	FVector CalculateTargetLocation();

	float LockOnRadius = 2000.f;

	class APlayerCharacter* Player;

	bool isLockingOn = false;

	TSubclassOf<class UEnemyHealthBar> EnemyHealthBarClass;
	class UEnemyHealthBar* EnemyHealthBar;

	TSubclassOf<class UBeatScreen> BeatScreenClass;
	class UBeatScreen* BeatScreen;

	UFUNCTION()
	void ShowBeatScreen();
};
