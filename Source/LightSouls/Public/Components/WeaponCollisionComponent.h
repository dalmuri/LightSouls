// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/AttackType.h"
#include "WeaponCollisionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGHTSOULS_API UWeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponCollisionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetWeaponMeshComponent(UStaticMeshComponent* InStaticMeshComponent) { WeaponMeshComponent = InStaticMeshComponent; }

	FORCEINLINE void SetOwnerActor(class ABaseCharacter* InCharacter) { OwnerCharacter = InCharacter; }
	void AddActorToIgnoreCollision(AActor* InActor);

	void SetCollisionSocketName(FName StartSocketName, FName EndSocketName);
	void SetTraceRadius(float InRadius);

	void ToggleTraceCollision(bool Traces);

	void SetAttackIntensityType(EAttackIntensityType InAttackIntensityType);

private:
	class ABaseCharacter* OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initialize", meta = (AllowPrivateAccess = "true"))
	FName CollisionStartSocketName = FName(TEXT("StartPoint"));
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initialize", meta = (AllowPrivateAccess = "true"))
	FName CollisionEndSocketName = FName(TEXT("EndPoint"));
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initialize", meta = (AllowPrivateAccess = "true"))
	float TraceRadius = 20.f;
	TArray<AActor*> ActorsToIgnoreCollision;

	UStaticMeshComponent* WeaponMeshComponent;

	bool isTracingCollision = false;
	EAttackIntensityType AttackIntensityType = EAttackIntensityType::EAIT_Flinch;

	TArray<AActor*> HitActors;

	void TraceCollision();
};
