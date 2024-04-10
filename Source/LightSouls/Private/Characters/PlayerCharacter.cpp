// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StatusComponent.h"
#include "Components/LockOnTargetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "UI/HUDWidget.h"
#include "UI/DeathScreen.h"
#include "Items/Weapons/BaseWeapon.h"
#include "Items/Shields/BaseShield.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Characters/Enemies/BaseEnemy.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogPlayerCharacter);

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->GravityScale = 1.75f; // Custom
	GetCharacterMovement()->MaxAcceleration = 1500.f; // Custom
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetUsingAbsoluteRotation(true); // Custom for preventing camera zitter when using SetActorRotation

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, 270.0f, 0.0f));

	// Component
	LockOnTargetComponent = CreateDefaultSubobject<ULockOnTargetComponent>(TEXT("LockOnTargetComponent"));

	// Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SKELETAL_MESH(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Echo/Meshes/Echo.Echo'"));
	if (SKELETAL_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKELETAL_MESH.Object);
	}

	// AnimBP
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIMBP_C(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Echo/Animations/ABP_PlayerCharacter_Echo.ABP_PlayerCharacter_Echo_C'"));
	if (ANIMBP_C.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIMBP_C.Class);
	}

#pragma region Skin
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString(TEXT("head"));
	static ConstructorHelpers::FObjectFinder<UGroomAsset> GROOM_ASSET_HAIR(TEXT("/Script/HairStrandsCore.GroomAsset'/Game/Characters/Echo/Hair/Hair_S_UpdoBuns.Hair_S_UpdoBuns'"));
	if (GROOM_ASSET_HAIR.Succeeded())
	{
		Hair->GroomAsset = GROOM_ASSET_HAIR.Object;
	}

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString(TEXT("head"));
	static ConstructorHelpers::FObjectFinder<UGroomAsset> GROOM_ASSET_EYEBROWS(TEXT("/Script/HairStrandsCore.GroomAsset'/Game/Characters/Echo/Hair/Eyebrows_L_Echo.Eyebrows_L_Echo'"));
	if (GROOM_ASSET_EYEBROWS.Succeeded())
	{
		Eyebrows->GroomAsset = GROOM_ASSET_EYEBROWS.Object;
	}
#pragma endregion

#pragma region Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_DEATH(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/Death/AM_Death.AM_Death'"));
	if (AM_DEATH.Succeeded())
	{
		DeathMontage = AM_DEATH.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_ROLL(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/Dodge/AM_Roll.AM_Roll'"));
	if (AM_ROLL.Succeeded())
	{
		RollMontage = AM_ROLL.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HIT_FLINCH_FRONT(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/HitReaction/Flinch/AM_HitReact_Flinch_Front.AM_HitReact_Flinch_Front'"));
	if (AM_HIT_FLINCH_FRONT.Succeeded())
	{
		HitReactionFlinchFromFrontMontage = AM_HIT_FLINCH_FRONT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HIT_FLINCH_RIGHT(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/HitReaction/Flinch/AM_HitReact_Flinch_Right.AM_HitReact_Flinch_Right'"));
	if (AM_HIT_FLINCH_RIGHT.Succeeded())
	{
		HitReactionFlinchFromRightMontage = AM_HIT_FLINCH_RIGHT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HIT_FLINCH_BACK(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/HitReaction/Flinch/AM_HitReact_Flinch_Back.AM_HitReact_Flinch_Back'"));
	if (AM_HIT_FLINCH_BACK.Succeeded())
	{
		HitReactionFlinchFromBackMontage = AM_HIT_FLINCH_BACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HIT_FLINCH_LEFT(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/HitReaction/Flinch/AM_HitReact_Flinch_Left.AM_HitReact_Flinch_Left'"));
	if (AM_HIT_FLINCH_LEFT.Succeeded())
	{
		HitReactionFlinchFromLeftMontage = AM_HIT_FLINCH_LEFT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HIT_KNOCKBACK_FRONT(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/HitReaction/KnockBack/AM_HitReact_KnockBack_Front.AM_HitReact_KnockBack_Front'"));
	if (AM_HIT_KNOCKBACK_FRONT.Succeeded())
	{
		HitReactionKnockBackFromFrontMontage = AM_HIT_KNOCKBACK_FRONT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HIT_KNOCKBACK_RIGHT(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/HitReaction/KnockBack/AM_HitReact_KnockBack_Right.AM_HitReact_KnockBack_Right'"));
	if (AM_HIT_KNOCKBACK_RIGHT.Succeeded())
	{
		HitReactionKnockBackFromRightMontage = AM_HIT_KNOCKBACK_RIGHT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HIT_KNOCKBACK_BACK(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/HitReaction/KnockBack/AM_HitReact_KnockBack_Back.AM_HitReact_KnockBack_Back'"));
	if (AM_HIT_KNOCKBACK_BACK.Succeeded())
	{
		HitReactionKnockBackFromBackMontage = AM_HIT_KNOCKBACK_BACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HIT_KNOCKBACK_LEFT(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/HitReaction/KnockBack/AM_HitReact_KnockBack_Left.AM_HitReact_KnockBack_Left'"));
	if (AM_HIT_KNOCKBACK_LEFT.Succeeded())
	{
		HitReactionKnockBackFromLeftMontage = AM_HIT_KNOCKBACK_LEFT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_GUARD_REACTION(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/Guard/AM_Guard_Reaction.AM_Guard_Reaction'"));
	if (AM_GUARD_REACTION.Succeeded())
	{
		GuardReactionMontage = AM_GUARD_REACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_GUARD_BREAK(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Echo/Animations/Guard/AM_Guard_Break.AM_Guard_Break'"));
	if (AM_GUARD_BREAK.Succeeded())
	{
		GuardBreakMontage = AM_GUARD_BREAK.Object;
	}
#pragma endregion

#pragma region Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_DEFAULT(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (IMC_DEFAULT.Succeeded())
	{
		DefaultInputMappingContext = IMC_DEFAULT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOVE(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (IA_MOVE.Succeeded())
	{
		MoveAction = IA_MOVE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LOOK(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'"));
	if (IA_LOOK.Succeeded())
	{
		LookAction = IA_LOOK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_JUMP(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (IA_JUMP.Succeeded())
	{
		JumpAction = IA_JUMP.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_DODGE(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Dodge.IA_Dodge'"));
	if (IA_DODGE.Succeeded())
	{
		DodgeAction = IA_DODGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_RUN(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Run.IA_Run'"));
	if (IA_RUN.Succeeded())
	{
		RunAction = IA_RUN.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LIGHT_ATTACK(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_LightAttack.IA_LightAttack'"));
	if (IA_LIGHT_ATTACK.Succeeded())
	{
		LightAttackAction = IA_LIGHT_ATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LOCK_ON_TARGET(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_LockOn.IA_LockOn'"));
	if (IA_LOCK_ON_TARGET.Succeeded())
	{
		LockOnTargetAction = IA_LOCK_ON_TARGET.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_GUARD(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Guard.IA_Guard'"));
	if (IA_GUARD.Succeeded())
	{
		GuardAction = IA_GUARD.Object;
	}
#pragma endregion

#pragma region UI
	static ConstructorHelpers::FClassFinder<UHUDWidget> UI_WIDGET_HUD_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_HUDWidget.WBP_HUDWidget_C'"));
	if (UI_WIDGET_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_WIDGET_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UDeathScreen> UI_DEATH_SCREEN_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_DeathScreen.WBP_DeathScreen_C'"));
	if (UI_DEATH_SCREEN_C.Succeeded())
	{
		DeathScreenClass = UI_DEATH_SCREEN_C.Class;
	}
#pragma endregion

#pragma region Particle
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE_HIT(TEXT("/Script/Engine.ParticleSystem'/Game/Effects/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
	if (PARTICLE_HIT.Succeeded())
	{
		HitParticle = PARTICLE_HIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE_PARRY(TEXT("/Script/Engine.ParticleSystem'/Game/Effects/Aura/P_Aura_Default_Upheaval_Charge.P_Aura_Default_Upheaval_Charge'"));
	if (PARTICLE_PARRY.Succeeded())
	{
		ParryParticle = PARTICLE_PARRY.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> PARTICLE_DODGE(TEXT("/Script/Niagara.NiagaraSystem'/Game/Effects/EnergyExplosion/P_EnergyExplosion.P_EnergyExplosion'"));
	if (PARTICLE_DODGE.Succeeded())
	{
		DodgeParticle = PARTICLE_DODGE.Object;
	}
#pragma endregion

#pragma region Initial Equipment
	static ConstructorHelpers::FClassFinder<ABaseWeapon> WEAPON(TEXT("/Script/Engine.Blueprint'/Game/Items/Weapons/GreystoneToughSword/BP_GreystoneToughSword.BP_GreystoneToughSword_C'"));
	if (WEAPON.Succeeded())
	{
		InitialWeapon = WEAPON.Class;
	}

	static ConstructorHelpers::FClassFinder<ABaseEquipableItem> SHIELD(TEXT("/Script/Engine.Blueprint'/Game/Items/Shields/GreystoneToughShield/BP_GreystoneToughShield.BP_GreystoneToughShield_C'"));
	if (SHIELD.Succeeded())
	{
		InitialShield = SHIELD.Class;
	}
#pragma endregion
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}

		// Camera Pitch 각도 제한
		PlayerController->PlayerCameraManager->ViewPitchMax = 10.f;
		PlayerController->PlayerCameraManager->ViewPitchMin = -70.f;
	}

#pragma region UI
	// HUD UI
	HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->SetStatusComponent(StatusComponent);
	HUDWidget->AddToViewport();

	// Death Screen UI
	DeathScreen = CreateWidget<UDeathScreen>(GetWorld(), DeathScreenClass);
#pragma endregion

	// Regen Stamina
	StatusComponent->ExcuteStaminaTimer();

	// Initial Equipment
	if (InitialWeapon)
	{
		Equip(InitialWeapon);
	}
	if (InitialShield)
	{
		Equip(InitialShield);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// Dodge
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dodge);

		// Running
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APlayerCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopRun);

		// Attack
		EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LightAttack);

		// Lock On
		EnhancedInputComponent->BindAction(LockOnTargetAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ToggleLockOn);

		// Guard
		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Started, this, &APlayerCharacter::Guard);
		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopGuard);

		// Mobile Touch
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APlayerCharacter::TouchPressed);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &APlayerCharacter::TouchMoved);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &APlayerCharacter::TouchReleased);
	}
	else
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && IsAlive())
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (LockOnTargetComponent && LockOnTargetComponent->IsLockingOn()) return;

	if (Controller != nullptr && IsAlive())
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
	if (CanRoll())
	{
		if (canAct)
		{
			// 움직이는 중인지 확인
			if (HasMovementKeyInput()) Roll(GetLastMovementInputVector().Rotation(), RollStamina);
			else Roll(GetActorRotation(), RollStamina);
		}
		else
		{
			// Input Buffer
		}
	}
}

void APlayerCharacter::Roll(FRotator Rotation, float InRollStamina)
{
	PlayerCharacterState = EPlayerCharacterState::EPS_Rolling;
	StatusComponent->PauseStaminaTimer();
	HandleCanInput(false); // Input 차단
	StatusComponent->DecreaseStamina(InRollStamina);
	SetActorRotation(Rotation);
	float Duation = PlayAnimMontage(RollMontage);
}

bool APlayerCharacter::CanRoll()
{
	if (!StatusComponent) return false;
	if (!RollMontage) return false;
	return canInput && StatusComponent->GetCurrentStamina() >= RollStamina;
}

void APlayerCharacter::Run()
{
	isRunKeyPressed = true;
	if (CanRun())
	{
		PlayerCharacterState = EPlayerCharacterState::EPS_Running;
		GetCharacterMovement()->MaxWalkSpeed = StatusComponent->GetRunSpeed(); // Speed 변경
		StatusComponent->SetStaminaChangeRate(RunStaminaReductionRate * -1); // Stamina 소모
		StatusComponent->UnpauseStaminaTimer();
		StatusComponent->OnStaminaTimerPaused.AddDynamic(this, &APlayerCharacter::StopRun);
	}
}

void APlayerCharacter::StopRun()
{
	isRunKeyPressed = false;
	if (!StatusComponent) return;

	GetCharacterMovement()->MaxWalkSpeed = StatusComponent->GetWalkSpeed(); // Speed 변경
	StatusComponent->OnStaminaTimerPaused.RemoveDynamic(this, &APlayerCharacter::StopRun);

	HandleCanAct(true);
	PlayerCharacterState = EPlayerCharacterState::EPS_Idle;
	StatusComponent->PauseStaminaTimer();
	StatusComponent->SetStaminaChangeRate(StatusComponent->GetDefaultStaminaRegenRate());
	StatusComponent->UnpauseStaminaTimer(0.75f);
}

bool APlayerCharacter::CanRun()
{
	if (!StatusComponent) return false;
	return canInput && canAct && StatusComponent->GetCurrentStamina() >= RunStaminaThreshold && !isGuarding;
}

void APlayerCharacter::LightAttack(const FInputActionValue& Value)
{
	if (IsGuarding())
	{
		if (CanParry())
		{
			Parry();
		}
	}
	else
	{
		if (CanAttack())
		{
			if (PlayerCharacterState == EPlayerCharacterState::EPS_Attacking)
			{
				isAttackSaved = true;
			}
			else if (canAct)
			{
				Attack(EPlayerAttackType::EPAT_LightAttack, LightAttackStamina);
			}
			else
			{
				// Input Buffer
			}
		}
	}
}

void APlayerCharacter::Attack(EPlayerAttackType PlayerAttackType, float InAttackStamina)
{
	UAnimMontage* AttackMontage = EquippedWeapon->GetAttackMontage(PlayerAttackType, AttackCount);
	if (AttackMontage)
	{
		PlayerCharacterState = EPlayerCharacterState::EPS_Attacking;
		StatusComponent->PauseStaminaTimer();
		StatusComponent->DecreaseStamina(InAttackStamina);
		HandleCanInput(false); // Input 차단

		PlayAnimMontage(AttackMontage);
	}
}

bool APlayerCharacter::CanAttack()
{
	if (!StatusComponent) return false;
	if (!EquippedWeapon) return false;
	return (canInput || PlayerCharacterState == EPlayerCharacterState::EPS_Attacking) && StatusComponent->GetCurrentStamina() >= LightAttackStamina;
}

void APlayerCharacter::Guard()
{
	isGuardKeyPressed = true;

	if (PlayerCharacterState == EPlayerCharacterState::EPS_Running) StopRun();

	if (CanGuard())
	{
		isGuarding = true;
		StatusComponent->SetStaminaChangeRate(StatusComponent->GetGuardStaminaRegenRate());
		UE_LOG(LogPlayerCharacter, Warning, TEXT("Guard"));
	}
}

void APlayerCharacter::StopGuard()
{
	isGuardKeyPressed = false;
	isGuarding = false;

	HandleCanAct(true);
	PlayerCharacterState = EPlayerCharacterState::EPS_Idle;
	StatusComponent->PauseStaminaTimer();
	StatusComponent->SetStaminaChangeRate(StatusComponent->GetDefaultStaminaRegenRate());
	StatusComponent->UnpauseStaminaTimer(0.75f);
}

bool APlayerCharacter::CanGuard()
{
	if (!StatusComponent) return false;
	if (!EquippedShield) return false;
	return canInput && canAct;
}

void APlayerCharacter::Parry()
{
	if (CanParry())
	{
		UAnimMontage* ParryMontage = EquippedShield->GetParryMontage();
		if (ParryMontage)
		{
			PlayerCharacterState = EPlayerCharacterState::EPS_Parrying;
			StatusComponent->PauseStaminaTimer();
			isGuarding = false;
			HandleCanInput(false); // Input 차단

			PlayAnimMontage(ParryMontage);
		}
	}
}

bool APlayerCharacter::CanParry()
{
	if (!StatusComponent) return false;
	if (!EquippedShield) return false;
	return canInput && canAct;
}

void APlayerCharacter::ToggleLockOn(const FInputActionValue& Value)
{
	if (!LockOnTargetComponent) return;

	if (LockOnTargetComponent->IsLockingOn())
	{
		LockOnTargetComponent->StopLockOn();
	}
	else
	{
		LockOnTargetComponent->LockOn();
	}
}

void APlayerCharacter::TouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	// Debug Text
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("TouchPressed"));
	UE_LOG(LogPlayerCharacter, Log, TEXT("TouchPressed"));

	TouchPressedLocation = FVector2D(Location); // 첫 Touch 위치 저장

	// HoldTimeThreshold가 지나도 움직이지 않았다면 Hold() 발동
	GetWorldTimerManager().SetTimer(HoldingTimer, FTimerDelegate::CreateLambda([&]()
		{
			if (!isSwipe)
			{
				Hold();
			}
		}), HoldTimeThreshold, false);
}

void APlayerCharacter::TouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
	FVector2D TouchMovedVector = FVector2D(Location) - TouchPressedLocation;

	float DistX = FMath::Abs(TouchMovedVector.X);
	float DistY = FMath::Abs(TouchMovedVector.Y);

	float TouchMovedDistance = TouchMovedVector.Size();

	if (TouchMovedDistance >= SwipeDistanceThreshold && !isSwipe)
	{
		isSwipe = true;

		if (DistX >= DistY) // X축 이동 거리가 더 길 경우
		{
			if (TouchMovedVector.X > 0)
			{
				Swipe(EDirection::ED_Right);
			}
			else
			{
				Swipe(EDirection::ED_Left);
			}
		}
		else // Y축 이동 거리가 더 길 경우
		{
			if (TouchMovedVector.Y < 0)
			{
				if (isHolding)
				{
					if (CanParry()) Parry();
				}
				else
				{
					Swipe(EDirection::ED_Front);
				}
			}
			else
			{
				Swipe(EDirection::ED_Back);
			}
		}
	}
}

void APlayerCharacter::TouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
	// Debug
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("TouchReleased"));
	UE_LOG(LogPlayerCharacter, Log, TEXT("TouchReleased"));

	// Tap
	if (GetWorldTimerManager().IsTimerActive(HoldingTimer) && !isSwipe) Tap();

	// Release
	GetWorldTimerManager().ClearTimer(HoldingTimer);
	isSwipe = false;
	if (isHolding) HoldReleased();
}

void APlayerCharacter::Tap()
{
	// Debug
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Tap"));
	UE_LOG(LogPlayerCharacter, Log, TEXT("Tap"));

	if (CanAttack())
	{
		if (PlayerCharacterState == EPlayerCharacterState::EPS_Attacking)
		{
			isAttackSaved = true;
		}
		else if (canAct)
		{
			Attack(EPlayerAttackType::EPAT_LightAttack, LightAttackStamina);
		}
		else
		{
			// Input Buffer
		}
	}
}

void APlayerCharacter::Hold()
{
	// Debug
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Hold"));
	UE_LOG(LogPlayerCharacter, Log, TEXT("Hold"));

	isGuardKeyPressed = true;
	isHolding = true;

	Guard();
}

void APlayerCharacter::HoldReleased()
{
	// Debug
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("HoldReleased"));
	UE_LOG(LogPlayerCharacter, Log, TEXT("HoldReleased"));

	isGuardKeyPressed = false;
	isHolding = false;
	if (isGuarding) StopGuard();
}

void APlayerCharacter::Swipe(EDirection Direction)
{
	if (CanRoll())
	{
		if (canAct)
		{
			FRotator ControlRot = GetControlRotation();
			switch (Direction)
			{
			case EDirection::ED_Front:
				//Roll(GetActorForwardVector().Rotation(), RollStamina);
				Roll(ControlRot, RollStamina);
				break;
			case EDirection::ED_Back:
				//Roll((GetActorForwardVector() * -1).Rotation(), RollStamina);
				ControlRot.Yaw += 180;
				Roll(ControlRot, RollStamina);
				break;
			case EDirection::ED_Right:
				//Roll(GetActorRightVector().Rotation(), RollStamina);
				ControlRot.Yaw += 90;
				Roll(ControlRot, RollStamina);
				break;
			case EDirection::ED_Left:
				//Roll((GetActorRightVector() * -1).Rotation(), RollStamina);
				ControlRot.Yaw += 270;
				Roll(ControlRot, RollStamina);
				break;
			}
		}
		else
		{
			// Input Buffer
		}
	}
}

void APlayerCharacter::HandleCanInput(bool InCanInput, float Delay)
{
	if (InCanInput)
	{
		if (Delay <= 0.f)
		{
			canInput = true;
		}
		else
		{
			FTimerHandle TempDelayedTimer;
			GetWorldTimerManager().SetTimer(TempDelayedTimer, FTimerDelegate::CreateLambda([&]()
				{
					canInput = true;
				}), Delay, false);
		}
	}
	else
	{
		canAct = false;
		canInput = false;
	}
}

void APlayerCharacter::HandleCanAct(bool InCanAct, float Delay)
{
	if (InCanAct)
	{
		if (Delay <= 0.f)
		{
			canAct = true;
			canInput = true;
			// Handle Input Buffer
		}
		else
		{
			FTimerHandle TempDelayedTimer;
			GetWorldTimerManager().SetTimer(TempDelayedTimer, FTimerDelegate::CreateLambda([&]()
				{
					canAct = true;
					canInput = true;
					// Handle Input Buffer
				}), Delay, false);
		}
	}
	else
	{
		canAct = false;
	}
}

void APlayerCharacter::OnGetHit(float Damage, EAttackType AttackType, EAttackIntensityType AttackIntensityType, AActor* DamageCauser, FHitResult HitResult)
{
	if (!StatusComponent) return;

	if (!CanBeHit())
	{
		if (isInvincible)
		{
			StatusComponent->IncreaseStamina(RollStamina / 2.f);
			if (DodgeParticle) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DodgeParticle, GetActorLocation(), GetActorRightVector().Rotation()); // IFrame Effect
		}
		return;
	}

	if (Damage <= 0) return;

	EDirection DamageCauserDirection = CalculateDirection(DamageCauser);

	if (isParrying && AttackType == EAttackType::EAT_MeleeAttack)
	{
		UE_LOG(LogPlayerCharacter, Warning, TEXT("Parry Succeed"));

		StatusComponent->IncreaseStamina(RollStamina);

		if (DamageCauser->GetClass()->ImplementsInterface(UCombatable::StaticClass()))
		{
			ICombatable* CombatableActor = Cast<ICombatable>(DamageCauser);
			CombatableActor->OnGetParried();
		}

		if (GetWorld())
		{
			if (ParryParticle) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParryParticle, EquippedShield->GetActorLocation(), GetActorForwardVector().Rotation(), true);
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f); 
		}
		return;
	}

	if (isGuarding)
	{
		if (DamageCauserDirection == EDirection::ED_Front)
		{
			float StaminaToDecrease = Damage * 1.2f;
			StatusComponent->PauseStaminaTimer();
			StatusComponent->UnpauseStaminaTimer(0.75f);
			if (StatusComponent->GetCurrentStamina() >= StaminaToDecrease) // Guard 성공
			{
				StatusComponent->DecreaseStamina(StaminaToDecrease);

				if (GuardReactionMontage)
				{
					HandleCanInput(false);
					canBeHit = false;

					PlayAnimMontage(GuardReactionMontage);
				}

				return;
			}
			else // Guard 실패
			{
				Damage -= StatusComponent->GetCurrentStamina();
				StatusComponent->DecreaseStamina(StaminaToDecrease);

				if (GuardBreakMontage)
				{
					HandleCanInput(false);
					canBeHit = false;

					PlayAnimMontage(GuardBreakMontage);
				}

				ReceiveDamage(Damage);
				return;
			}
		}
	}

	ReceiveDamage(Damage);

	if (HitParticle && GetWorld())
	{
		FVector ToHit = (HitResult.Location - GetActorLocation()).GetSafeNormal();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.Location, ToHit.Rotation() + FRotator(-90.f, 0.f, 0.f), FVector(HitParticleScale));
	}

	if (StatusComponent->GetCurrentHealth() > 0)
	{
		// Hit Reaction
		UAnimMontage* HitReactionMontage;
		switch (AttackIntensityType)
		{
		case EAttackIntensityType::EAIT_Flinch:
		default:
			switch (DamageCauserDirection)
			{
			case EDirection::ED_Front:
			default:
				HitReactionMontage = HitReactionFlinchFromFrontMontage;
				break;
			case EDirection::ED_Right:
				HitReactionMontage = HitReactionFlinchFromRightMontage;
				break;
			case EDirection::ED_Back:
				HitReactionMontage = HitReactionFlinchFromBackMontage;
				break;
			case EDirection::ED_Left:
				HitReactionMontage = HitReactionFlinchFromLeftMontage;
				break;
			}
			break;
		case EAttackIntensityType::EAIT_KnockBack:
			switch (DamageCauserDirection)
			{
			case EDirection::ED_Front:
			default:
				HitReactionMontage = HitReactionKnockBackFromFrontMontage;
				break;
			case EDirection::ED_Right:
				HitReactionMontage = HitReactionKnockBackFromRightMontage;
				break;
			case EDirection::ED_Back:
				HitReactionMontage = HitReactionKnockBackFromBackMontage;
				break;
			case EDirection::ED_Left:
				HitReactionMontage = HitReactionKnockBackFromLeftMontage;
				break;
			}
			break;
		}

		if (HitReactionMontage)
		{
			HandleCanInput(false);
			canBeHit = false;

			PlayAnimMontage(HitReactionMontage);
		}
	}
	else
	{
		// Death
		isAlive = false;
		HandleCanInput(false);
		canBeHit = false;
		LockOnTargetComponent->StopLockOn();

		if (DeathMontage)
		{
			PlayAnimMontage(DeathMontage);

			if (DeathScreen) DeathScreen->AddToViewport();
		}
	}
}

void APlayerCharacter::Equip(TSubclassOf<ABaseEquipableItem> EquipableItem)
{
	UWorld* World = GetWorld();
	if (World && EquipableItem)
	{
		ABaseEquipableItem* SpawnedEquipableItem = World->SpawnActor<ABaseEquipableItem>(EquipableItem, GetActorTransform());
		SpawnedEquipableItem->OnEquipped(GetMesh(), SpawnedEquipableItem->GetEquipableSocketName(), this, this);
		if (ABaseWeapon* SpawnedWeapon = Cast<ABaseWeapon>(SpawnedEquipableItem))
		{
			if (EquippedWeapon) EquippedWeapon->Destroy();

			EquippedWeapon = SpawnedWeapon;
		}
		else if (ABaseShield* SpawnedShield = Cast<ABaseShield>(SpawnedEquipableItem))
		{
			if (EquippedShield) EquippedShield->Destroy();

			EquippedShield = SpawnedShield;
		}
		else
		{
			EquippedItems.Add(SpawnedEquipableItem);
		}
	}
}

void APlayerCharacter::EndAction()
{
	Super::EndAction();

	UE_LOG(LogPlayerCharacter, Warning, TEXT("EndAction"));

	if (LockOnTargetComponent && LockOnTargetComponent->IsLockingOn()) LockOnTargetComponent->RotateToTarget();

	if (GetWorld()) UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);

	HandleCanAct(true);
	canBeHit = true;

	if (isGuardKeyPressed)
	{
		Guard();
	}
	else if (isRunKeyPressed)
	{
		Run();
	}
	else
	{
		PlayerCharacterState = EPlayerCharacterState::EPS_Idle;
		StatusComponent->SetStaminaChangeRate(StatusComponent->GetDefaultStaminaRegenRate());
		StatusComponent->UnpauseStaminaTimer(0.75f);
	}
}

void APlayerCharacter::ComboAttack()
{
	if (isAttackSaved && EquippedWeapon->GetNumOfLightAttackMontages() > 0)
	{
		isAttackSaved = false;
		AttackCount = (AttackCount + 1) % EquippedWeapon->GetNumOfLightAttackMontages();
		Attack(EPlayerAttackType::EPAT_LightAttack, LightAttackStamina);
	}
	else
	{
		AttackCount = 0;
		isAttackSaved = false;
	}
}

//void APlayerCharacter::ResetComboAttackCount()
//{
//	AttackCount = 0;
//	isAttackSaved = false;
//}

void APlayerCharacter::ToggleTraceAttackCollision(bool Traces, EAttackWeaponType AttackWeaponType, EAttackIntensityType AttackIntensityType)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->ToggleTraceCollision(Traces);
	}
}

float APlayerCharacter::CalculateDamage()
{
	if (PlayerCharacterState != EPlayerCharacterState::EPS_Attacking) return 0.f;
	if (!EquippedWeapon) return 0.f;

	float BaseDamage = EquippedWeapon->Damage;

	switch (NowAttackType)
	{
	case EPlayerAttackType::EPAT_LightAttack:
	default:
		return BaseDamage;
	case EPlayerAttackType::EPAT_HeavyAttack:
		return BaseDamage * 1.8f;
	case EPlayerAttackType::EPAT_RunningAttack:
		return BaseDamage * 1.2f;
	}
}

bool APlayerCharacter::CanBeHit() const
{
	return canBeHit && !isInvincible;
}

void APlayerCharacter::ToggleIFrame(bool InIsInvincible)
{
	isInvincible = InIsInvincible;
}

void APlayerCharacter::ToggleParry(bool InIsParrying)
{
	isParrying = InIsParrying;
}

void APlayerCharacter::OnGetDetected(ABaseEnemy* Enemy)
{
	LockOnTargetComponent->LockOn(Enemy);
}

bool APlayerCharacter::HasMovementKeyInput() const
{
	return GetCharacterMovement()->GetLastInputVector().Length() > 0.001f; // 오차 범위
}
