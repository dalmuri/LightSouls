// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponCollisionComponent.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/Combatable.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UWeaponCollisionComponent::UWeaponCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UWeaponCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UWeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isTracingCollision)
	{
		TraceCollision();
	}
}

void UWeaponCollisionComponent::AddActorToIgnoreCollision(AActor* InActor)
{
	ActorsToIgnoreCollision.AddUnique(InActor);
}

void UWeaponCollisionComponent::SetCollisionSocketName(FName StartSocketName, FName EndSocketName)
{
	CollisionStartSocketName = StartSocketName;
	CollisionEndSocketName = EndSocketName;
}

void UWeaponCollisionComponent::SetTraceRadius(float InRadius)
{
	TraceRadius = InRadius;
}

void UWeaponCollisionComponent::ToggleTraceCollision(bool Traces)
{
	if (Traces)
	{
		HitActors.Empty();
		isTracingCollision = true;
	}
	else
	{
		isTracingCollision = false;
	}
}

void UWeaponCollisionComponent::SetAttackIntensityType(EAttackIntensityType InAttackIntensityType)
{
	AttackIntensityType = InAttackIntensityType;
}

void UWeaponCollisionComponent::TraceCollision()
{
	if (!OwnerCharacter) return;

	FVector StartLoc, EndLoc;

	if (WeaponMeshComponent)
	{
		StartLoc = WeaponMeshComponent->GetSocketLocation(CollisionStartSocketName);
		EndLoc = WeaponMeshComponent->GetSocketLocation(CollisionEndSocketName);
	}
	else
	{
		StartLoc = OwnerCharacter->GetMesh()->GetSocketLocation(CollisionStartSocketName);
		EndLoc = OwnerCharacter->GetMesh()->GetSocketLocation(CollisionEndSocketName);
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartLoc, EndLoc, TraceRadius, ObjectTypes, false, ActorsToIgnoreCollision, EDrawDebugTrace::None, HitResults, true);

	for (FHitResult HitResult : HitResults)
	{
		if (HitResult.GetActor() && HitResult.GetActor()->GetClass()->ImplementsInterface(UCombatable::StaticClass()) && !HitActors.Contains(HitResult.GetActor()))
		{
			HitActors.Add(HitResult.GetActor());
			ICombatable* CombatableActor = Cast<ICombatable>(HitResult.GetActor());
			CombatableActor->OnGetHit(OwnerCharacter->CalculateDamage(), EAttackType::EAT_MeleeAttack, AttackIntensityType, OwnerCharacter, HitResult);
		}
	}
}

