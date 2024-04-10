// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Enemy_FrostGiant.h"
#include "Components/StatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WeaponCollisionComponent.h"

AEnemy_FrostGiant::AEnemy_FrostGiant()
{
	GetCapsuleComponent()->InitCapsuleSize(90.f, 200.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -200.0f), FRotator(0.0f, 270.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_FROST_GIANT(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Enemies/FrostGiant/Meshes/SK_Enemy_FrostGiant_Captain.SK_Enemy_FrostGiant_Captain'"));
	if (SK_FROST_GIANT.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_FROST_GIANT.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIMBP_C(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Enemies/FrostGiant/Animations/ABP_FrostGiant.ABP_FrostGiant_C'"));
	if (ANIMBP_C.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIMBP_C.Class);
	}

	LeftHandCollisionComponent = CreateDefaultSubobject<UWeaponCollisionComponent>(TEXT("LeftHandCollisionComponent"));
	LeftHandCollisionComponent->AddActorToIgnoreCollision(this);
	LeftHandCollisionComponent->SetOwnerActor(this);
	LeftHandCollisionComponent->SetCollisionSocketName(FName(TEXT("LeftHandCollisionSocket")), FName(TEXT("LeftHandCollisionSocket")));
	LeftHandCollisionComponent->SetTraceRadius(90.f);

	RightHandCollisionComponent = CreateDefaultSubobject<UWeaponCollisionComponent>(TEXT("RightHandCollisionComponent"));
	RightHandCollisionComponent->AddActorToIgnoreCollision(this);
	RightHandCollisionComponent->SetOwnerActor(this);
	RightHandCollisionComponent->SetCollisionSocketName(FName(TEXT("RightHandCollisionSocket")), FName(TEXT("RightHandCollisionSocket")));
	RightHandCollisionComponent->SetTraceRadius(90.f);

#pragma region Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_DEATH(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Enemies/FrostGiant/Animations/Death/AM_FrostGiant_Death.AM_FrostGiant_Death'"));
	if (AM_DEATH.Succeeded())
	{
		DeathMontage = AM_DEATH.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_COMBAT_START(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Enemies/FrostGiant/Animations/CombatStart/AM_FrostGiant_CombatStart.AM_FrostGiant_CombatStart'"));
	if (AM_COMBAT_START.Succeeded())
	{
		CombatStartMontage = AM_COMBAT_START.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_ATTACK0(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Enemies/FrostGiant/Animations/Attack/AM_FrostGiant_Attack_Melee_A.AM_FrostGiant_Attack_Melee_A'"));
	if (AM_ATTACK0.Succeeded())
	{
		AttackMontages.Add(AM_ATTACK0.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_ATTACK1(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Enemies/FrostGiant/Animations/Attack/AM_FrostGiant_Attack_Melee_B.AM_FrostGiant_Attack_Melee_B'"));
	if (AM_ATTACK1.Succeeded())
	{
		AttackMontages.Add(AM_ATTACK1.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_ATTACK2(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Enemies/FrostGiant/Animations/Attack/AM_FrostGiant_Attack_Melee_Combo.AM_FrostGiant_Attack_Melee_Combo'"));
	if (AM_ATTACK2.Succeeded())
	{
		AttackMontages.Add(AM_ATTACK2.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_STAGGER(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Enemies/FrostGiant/Animations/Stagger/AM_FrostGiant_Stagger.AM_FrostGiant_Stagger'"));
	if (AM_STAGGER.Succeeded())
	{
		StaggerMontage = AM_STAGGER.Object;
	}
#pragma endregion

#pragma region Particle
	HitParticleScale = 0.3f;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HIT_PARTICLE(TEXT("/Script/Engine.ParticleSystem'/Game/Effects/Rampage/P_Rampage_Ice_Rock_HitCharacter.P_Rampage_Ice_Rock_HitCharacter'"));
	if (HIT_PARTICLE.Succeeded())
	{
		HitParticle = HIT_PARTICLE.Object;
	}
#pragma endregion

	StatusComponent->SetMaxHealth(200.f);
}

void AEnemy_FrostGiant::BeginPlay()
{
	Super::BeginPlay();

	EnemyName = FText::FromString(TEXT("Frost Giant"));
}

void AEnemy_FrostGiant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy_FrostGiant::ToggleTraceAttackCollision(bool Traces, EAttackWeaponType AttackWeaponType, EAttackIntensityType AttackIntensityType)
{
	switch (AttackWeaponType)
	{
	case EAttackWeaponType::EAWT_LeftHand:
		if (Traces) LeftHandCollisionComponent->SetAttackIntensityType(AttackIntensityType);
		LeftHandCollisionComponent->ToggleTraceCollision(Traces);
		break;
	case EAttackWeaponType::EAWT_RightHand:
		if (Traces) RightHandCollisionComponent->SetAttackIntensityType(AttackIntensityType);
		RightHandCollisionComponent->ToggleTraceCollision(Traces);
		break;
	default:
		break;
	}
}
