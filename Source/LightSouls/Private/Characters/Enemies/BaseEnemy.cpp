// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/BaseEnemy.h"
#include "Components/WidgetComponent.h"
#include "Components/StatusComponent.h"
#include "Components/TargetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/EnemyHealthBar.h"
#include "Controllers/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Characters/PlayerCharacter.h"

ABaseEnemy::ABaseEnemy()
{
#pragma region Default Manny
	//GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, 270.0f, 0.0f));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNY(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	//if (SK_MANNY.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(SK_MANNY.Object);
	//}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//static ConstructorHelpers::FClassFinder<UAnimInstance> ANIMBP_C(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Mannequins/Animations/ABP_PlayerCharacter.ABP_PlayerCharacter_C'"));
	//if (ANIMBP_C.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(ANIMBP_C.Class);
	//}
#pragma endregion

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);

	TargetComponent = CreateDefaultSubobject<UTargetComponent>(TEXT("TargetComponent"));

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BEHAVIOR_TREE(TEXT("/Script/AIModule.BehaviorTree'/Game/BehaviorTree/BT_Enemy.BT_Enemy'"));
	if (BEHAVIOR_TREE.Succeeded())
	{
		BehaviorTree = BEHAVIOR_TREE.Object;
	}
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::OnGetHit(float Damage, EAttackType AttackType, EAttackIntensityType AttackIntensityType, AActor* DamageCauser, FHitResult HitResult)
{
	if (!CanBeHit()) return;

	ReceiveDamage(Damage);

	// ÇÇ°Ý ÀÌÆåÆ®
	if (HitParticle && GetWorld())
	{
		FVector ToHit = (HitResult.Location - GetActorLocation()).GetSafeNormal();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.Location, ToHit.Rotation() + FRotator(-90.f, 0.f, 0.f), FVector(HitParticleScale));
	}

	if (StatusComponent)
	{
		if (StatusComponent->GetCurrentHealth() > 0)
		{
			// Hit Reaction
		}
		else
		{
			// Death
			isAlive = false;
			canBeHit = false;
			if (DeathMontage)
			{
				PlayAnimMontage(DeathMontage);
			}
			
			if (OnEnemyDead.IsBound()) OnEnemyDead.Broadcast();
		}
	}
}

float ABaseEnemy::CalculateDamage()
{
	return 18.0f;
}

void ABaseEnemy::SetEnemyState(EEnemyState InEnemyState)
{
	EnemyState = InEnemyState;
}

UAnimMontage* ABaseEnemy::GetAttackMontage(int Index) const
{
	return AttackMontages[FMath::Clamp(Index, 0, AttackMontages.Num())];
}

APlayerCharacter* ABaseEnemy::SearchTargetPlayer()
{
	TArray<UTargetComponent*> Result;

	FVector EnemyLoc = GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::SphereTraceMultiForObjects(this, EnemyLoc, EnemyLoc, SearchRadius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	for (FHitResult HitResult : HitResults)
	{
		if (HitResult.GetActor())
		{
			if (APlayerCharacter* NewTargetPlayer = Cast<APlayerCharacter>(HitResult.GetActor()))
			{
				TargetPlayer = NewTargetPlayer;
				TargetPlayer->OnGetDetected(this);
				return TargetPlayer;
			}
		}
	}

	return nullptr;
}
