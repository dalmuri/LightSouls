// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LockOnTargetComponent.h"
#include "Components/TargetComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/Enemies/BaseEnemy.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/EnemyHealthBar.h"
#include "UI/BeatScreen.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
ULockOnTargetComponent::ULockOnTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UEnemyHealthBar> UI_ENEMY_HEALTHBAR_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_EnemyHealthBar.WBP_EnemyHealthBar_C'"));
	if (UI_ENEMY_HEALTHBAR_C.Succeeded())
	{
		EnemyHealthBarClass = UI_ENEMY_HEALTHBAR_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UBeatScreen> UI_BEAT_SCREEN_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_BeatScreen.WBP_BeatScreen_C'"));
	if (UI_BEAT_SCREEN_C.Succeeded())
	{
		BeatScreenClass = UI_BEAT_SCREEN_C.Class;
	}
}


// Called when the game starts
void ULockOnTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<APlayerCharacter>(GetOwner());

	// Enemy Health Bar UI
	EnemyHealthBar = CreateWidget<UEnemyHealthBar>(GetWorld(), EnemyHealthBarClass);

	// Beat Screen UI
	BeatScreen = CreateWidget<UBeatScreen>(GetWorld(), BeatScreenClass);
}


// Called every frame
void ULockOnTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isLockingOn)
	{
		LookAtTarget(DeltaTime);
	}
}

void ULockOnTargetComponent::LockOn()
{
	TArray<UTargetComponent*> Targetables = SearchTargets();
	if (Targetables.Num() > 0)
	{
		LockOn(Targetables[0]);
	}
}

void ULockOnTargetComponent::LockOn(APawn* InPawn)
{
	UTargetComponent* NewTarget = Cast<UTargetComponent>(InPawn->GetComponentByClass(UTargetComponent::StaticClass()));
	if (NewTarget)
	{
		LockOn(NewTarget);
	}
}

void ULockOnTargetComponent::LockOn(UTargetComponent* InTarget)
{
	if (InTarget)
	{
		Player->bUseControllerRotationYaw = false;
		Player->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		Player->GetCharacterMovement()->bOrientRotationToMovement = false;
		Target = InTarget;
		isLockingOn = true;

		if (ABaseEnemy* TargetEnemy = Cast<ABaseEnemy>(InTarget->GetTargetActor()))
		{
			EnemyHealthBar->SetEnemy(TargetEnemy);
			EnemyHealthBar->AddToViewport();

			TargetEnemy->OnEnemyDead.AddDynamic(this, &ULockOnTargetComponent::ShowBeatScreen);
		}

		UE_LOG(LogTemp, Warning, TEXT("Lock On Complete!"));
	}
}

void ULockOnTargetComponent::StopLockOn()
{
	if (isLockingOn)
	{
		if (ABaseEnemy* TargetEnemy = Cast<ABaseEnemy>(Target->GetTargetActor()))
		{
			if (EnemyHealthBar->IsInViewport()) EnemyHealthBar->RemoveFromParent();
			
			TargetEnemy->OnEnemyDead.RemoveDynamic(this, &ULockOnTargetComponent::ShowBeatScreen);
		}

		Player->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		Player->GetCharacterMovement()->bOrientRotationToMovement = true;
		isLockingOn = false;
		Target = nullptr;
	}
}

void ULockOnTargetComponent::RotateToTarget()
{
	if (IsLockingOn())
	{
		FVector PlayerLoc = Player->GetActorLocation();
		FVector TargetLoc = Target->GetTargetActor()->GetActorLocation();

		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(PlayerLoc, TargetLoc);
		Rotation.Roll = Player->GetActorRotation().Roll;
		Rotation.Pitch = Player->GetActorRotation().Pitch;

		Player->SetActorRotation(Rotation);
	}
}

bool ULockOnTargetComponent::isActorOnScreen(AActor* InTarget)
{
	bool Result = false;

	if (Player && Player->GetController())
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
		{
			FVector2D ScreenPosition;

			if (PlayerController->ProjectWorldLocationToScreen(InTarget->GetActorLocation(), ScreenPosition, true))
			{
				int32 ScreenSizeX, ScreenSizeY;
				PlayerController->GetViewportSize(ScreenSizeX, ScreenSizeY);

				Result = 0 < ScreenPosition.X && ScreenPosition.X < ScreenSizeX
					&& 0 < ScreenPosition.Y && ScreenPosition.Y < ScreenSizeY;
			}
		}
	}
	return Result;
}

TArray<UTargetComponent*> ULockOnTargetComponent::SearchTargets()
{
	TArray<UTargetComponent*> Result;

	FVector PlayerLoc = Player->GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Player);

	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::SphereTraceMultiForObjects(this, PlayerLoc, PlayerLoc, LockOnRadius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);
	for (FHitResult HitResult : HitResults)
	{
		if (HitResult.GetActor())
		{
			UTargetComponent* NewTarget = Cast<UTargetComponent>(HitResult.GetActor()->GetComponentByClass(UTargetComponent::StaticClass()));
			if (NewTarget && !Result.Contains(NewTarget))
			{
				if (isActorOnScreen(NewTarget->GetOwner()))
				{
					Result.Add(NewTarget);
				}
			}
		}
	}

	return Result;
}

void ULockOnTargetComponent::LookAtTarget(float DeltaTime)
{
	Player->GetController()->SetControlRotation(CalculateCameraRotation(DeltaTime));
}

FRotator ULockOnTargetComponent::CalculateCameraRotation(float DeltaTime)
{
	FVector CameraLoc = Player->GetFollowCamera()->GetComponentLocation();
	FVector PlayerLoc = Player->GetActorLocation();
	FVector TargetLoc = CalculateTargetLocation();

	TargetLoc.Z = TargetLoc.Z - FVector::Distance(PlayerLoc, TargetLoc) / 2;

	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(CameraLoc, TargetLoc);
	FRotator ControlRot = Player->GetControlRotation();

	Rotation = UKismetMathLibrary::RInterpTo(ControlRot, Rotation, DeltaTime, 15.0f);
	Rotation.Roll = ControlRot.Roll;

	return Rotation;
}

FVector ULockOnTargetComponent::CalculateTargetLocation()
{
	// TODO Interpolate
	return Target->GetTargetActor()->GetActorLocation();
}

void ULockOnTargetComponent::ShowBeatScreen()
{
	UE_LOG(LogTemp, Warning, TEXT("Show Beat Screen"));
	BeatScreen->AddToViewport();
}

