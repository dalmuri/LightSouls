// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/PlayerCharacterState.h"
#include "Enums/Direction.h"
#include "Enums/AttackType.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;
class UParticleSystem;
class UNiagaraSystem;
class ABaseEquipableItem;
class ABaseWeapon;
class ABaseShield;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, Log, All);

UCLASS()
class LIGHTSOULS_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	// To add mapping context
	virtual void BeginPlay();

public:
	virtual void Tick(float DeltaTime) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Dodge input */
	void Dodge(const FInputActionValue& Value);
	void Roll(FRotator Rotation, float InRollStamina);
	bool CanRoll();

	/** Called for Run input */
	void Run();
	UFUNCTION()
	void StopRun();
	bool CanRun();

	/** Called for LightAttack input */
	void LightAttack(const FInputActionValue& Value);
	void Attack(EPlayerAttackType PlayerAttackType, float InAttackStamina);
	bool CanAttack();

	/** Called for Guard input */
	void Guard();
	void StopGuard();
	bool CanGuard();

	void Parry();
	bool CanParry();

	/** Called for LockOn input */
	void ToggleLockOn(const FInputActionValue& Value);

	void TouchPressed(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchMoved(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchReleased(ETouchIndex::Type FingerIndex, FVector Location);
	void Tap();
	void Hold();
	void HoldReleased();
	void Swipe(EDirection Direction);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	void HandleCanInput(bool InCanInput, float Delay = 0.f);
	UFUNCTION(BlueprintCallable)
	void HandleCanAct(bool InCanAct, float Delay = 0.f);

	virtual void OnGetHit(float Damage, EAttackType AttackType, EAttackIntensityType AttackIntensityType, AActor* DamageCauser, FHitResult HitResult) override;

	void Equip(TSubclassOf<ABaseEquipableItem> EquipableItem);

	/// <summary>
	/// AnimMontage 이후 상태를 기본값으로 Reset
	/// </summary>
	virtual void EndAction() override;

	void ComboAttack();
	//void ResetComboAttackCount();
	virtual void ToggleTraceAttackCollision(bool Traces, EAttackWeaponType AttackWeaponType = EAttackWeaponType::EAWT_MainWeapon, EAttackIntensityType AttackIntensityType = EAttackIntensityType::EAIT_Flinch) override;

	virtual float CalculateDamage() override;

	virtual bool CanBeHit() const override;

	void ToggleIFrame(bool InIsInvincible);

	void ToggleParry(bool InIsParrying);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsGuarding() const { return isGuarding; }

	void OnGetDetected(class ABaseEnemy* Enemy);

private:
	// Component
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	class ULockOnTargetComponent* LockOnTargetComponent;

	UPROPERTY(VisibleAnywhere, Category = "Skin")
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = "Skin")
	UGroomComponent* Eyebrows;

#pragma region Input
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultInputMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Dodge Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	/** Run Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	/** LightAttack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LightAttackAction;

	/** Guard Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* GuardAction;

	/** LockOnTarget Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnTargetAction;
#pragma endregion

#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RollMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage|Hit Reaction|Flinch", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionFlinchFromFrontMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage|Hit Reaction|Flinch", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionFlinchFromRightMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage|Hit Reaction|Flinch", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionFlinchFromBackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage|Hit Reaction|Flinch", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionFlinchFromLeftMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage|Hit Reaction|Knock-Back", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionKnockBackFromFrontMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage|Hit Reaction|Knock-Back", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionKnockBackFromRightMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage|Hit Reaction|Knock-Back", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionKnockBackFromBackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage|Hit Reaction|Knock-Back", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionKnockBackFromLeftMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GuardReactionMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GuardBreakMontage;
#pragma endregion

#pragma region Particle
	UPROPERTY(EditDefaultsOnly, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ParryParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* DodgeParticle;
#pragma endregion

#pragma region UI
	TSubclassOf<class UHUDWidget> HUDWidgetClass;
	class UHUDWidget* HUDWidget;

	TSubclassOf<class UDeathScreen> DeathScreenClass;
	class UDeathScreen* DeathScreen;
#pragma endregion

	// 행동 가능
	bool canInput = true;
	bool canAct = true;

	// Mobile input
	FVector2D TouchPressedLocation;
	bool isSwipe = false;
	bool isHolding = false;
	FTimerHandle HoldingTimer;
	float HoldTimeThreshold = 0.3f;
	UPROPERTY(EditAnywhere, Category = EndlessRunCoreSettings, meta = (DisplayName = "Minimum Swipe Distance"))
	float SwipeDistanceThreshold = 15;

	// Item
	ABaseWeapon* EquippedWeapon;
	ABaseShield* EquippedShield;
	TArray<ABaseEquipableItem*> EquippedItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initialize", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseWeapon> InitialWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initialize", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseEquipableItem> InitialShield;

	// State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPlayerCharacterState PlayerCharacterState = EPlayerCharacterState::EPS_Idle;

	bool isRunKeyPressed = false;
	bool isGuardKeyPressed = false;
	bool isGuarding = false;
	bool isParrying = false;
	bool isInvincible = false;

	/// <summary>
	/// 이동 키를 눌렀는지 확인
	/// </summary>
	/// <returns>true if move key is pressed</returns>
	bool HasMovementKeyInput() const;

	// Attack
	bool isAttackSaved = false;
	int AttackCount = 0;
	EPlayerAttackType NowAttackType;

#pragma region Stamina
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float LightAttackStamina = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float RollStamina = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float RunStaminaThreshold = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float RunStaminaReductionRate = 0.7f;
#pragma endregion
};
