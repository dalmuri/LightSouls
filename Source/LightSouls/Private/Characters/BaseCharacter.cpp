// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/StatusComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::ToggleTraceAttackCollision(bool Traces, EAttackWeaponType AttackWeaponType, EAttackIntensityType AttackIntensityType)
{
}

void ABaseCharacter::OnGetHit(float Damage, EAttackType AttackType, EAttackIntensityType AttackIntensityType, AActor* DamageCauser, FHitResult HitResult)
{
	if (!canBeHit) return;

	ReceiveDamage(Damage);

	if (HitParticle && GetWorld())
	{
		FVector ToHit = (HitResult.Location - GetActorLocation()).GetSafeNormal();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.Location, ToHit.Rotation() + FRotator(-90.f, 0.f, 0.f), FVector(HitParticleScale));
	}

	if (StatusComponent)
	{
		if (StatusComponent->GetCurrentHealth() <= 0)
		{
			Destroy();
		}
	}
}

float ABaseCharacter::CalculateDamage()
{
	return 10.0f;
}

void ABaseCharacter::ReceiveDamage(float InDamage)
{
	if (StatusComponent)
	{
		StatusComponent->DecreaseHealth(InDamage);
	}
}

void ABaseCharacter::OnGetParried()
{
	if (StaggerMontage)
	{
		PlayAnimMontage(StaggerMontage);
	}
	else
	{
		StopAnimMontage();
		EndAction();
	}
}

void ABaseCharacter::EndAction()
{
	if (OnActionEnded.IsBound()) OnActionEnded.Broadcast();
}

EDirection ABaseCharacter::CalculateDirection(AActor* Target) const
{
	return CalculateDirection(Target->GetActorLocation());
}

EDirection ABaseCharacter::CalculateDirection(FVector TargetLocation) const
{
	FVector CharacterLocation = GetActorLocation();
	TargetLocation.Z = CharacterLocation.Z; // ≥Ù¿Ã ∏¬√„

	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, TargetLocation);
	float Angle = UKismetMathLibrary::NormalizedDeltaRotator(Rotation, GetActorRotation()).Yaw;

	if (-45.f <= Angle && Angle < 45.f)
	{
		return EDirection::ED_Front;
	}
	else if (45.f <= Angle && Angle < 135.f)
	{
		return EDirection::ED_Right;
	}
	else if (-135.f <= Angle && Angle < -45.f)
	{
		return EDirection::ED_Left;
	}
	else
	{
		return EDirection::ED_Back;
	}
}

void ABaseCharacter::Die()
{
	isAlive = false;
	if (DeathMontage) PlayAnimMontage(DeathMontage);
}

