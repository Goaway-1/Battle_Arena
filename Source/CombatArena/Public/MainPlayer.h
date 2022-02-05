#pragma once

#include "EngineMinimal.h"
#include "Weapon.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

class UMatineeCameraShake;

UENUM(BlueprintType)
enum class EMovementStatus : uint8 {
	EMS_Normal		UMETA(DisplayName = "Normal"),
	EMS_Walk		UMETA(DisplayName = "Walk"),
	EMS_Sprinting	UMETA(DisplayName = "Sprinting"),
	EMS_Dodge		UMETA(DisplayName = "Dodge"),
	EMS_Drawing		UMETA(DisplayName = "Drawing"),
	EMS_Throwing	UMETA(DisplayName = "Throwing"),
	EMS_Hited		UMETA(DisplayName = "Hited"),
	EMS_Faint		UMETA(DisplayName = "Faint"),
	EMS_Death		UMETA(DisplayName = "Death"),

	EMS_Default		UMETA(DisplayName = "Default")
}; 

/** Weapon�� ���� ���� */
UENUM(BlueprintType)
enum class EWeaponStatus : uint8 {
	EWS_Normal		UMETA(DisplayName = "Normal"),
	EWS_Shield		UMETA(DisplayName = "Shield"),
	EWS_Melee		UMETA(DisplayName = "Melee"),
	EWS_Bow			UMETA(DisplayName = "Bow"),

	EWS_Default		UMETA(DisplayName = "Default")
};

/** Shield�� ���� ���� (����) */
UENUM(BlueprintType)
enum class ECombatStatus : uint8 {
	ECS_Normal			UMETA(DisplayName = "Normal"),
	ECS_Attacking		UMETA(DisplayName = "Attacking"),
	ECS_Blocking		UMETA(DisplayName = "Blocking"),
	ECS_Skilling		UMETA(DisplayName = "Skilling"),

	ECS_Default			UMETA(DisplayName = "Default")
};

UCLASS()
class COMBATARENA_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AMainPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	/** AIPerception */
	class UAIPerceptionStimuliSourceComponent* AIPerceptionSource;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	class AMainController* PlayerController;	
public:
	FORCEINLINE AMainController* GetPlayerController() {return PlayerController;}

#pragma region CAMERA
private:
	class UCameraComponent* Camera;
	TSubclassOf<UMatineeCameraShake> CamShake;
	class APlayerCameraManager* CameraManager;
	class USceneComponent* SpringArmSence;

	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* SpringArm_Sprinting;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* SpringArm_Attacking;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* SpringArm_Drawing;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* SpringArm_Skilling;

	int TurnAxis = 0;	//회전 각도

	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
	float CameraSpeed = 20.f;

	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UMatineeCameraShake> RunShake;
public:
	void Lookup(float value);
	void Turn(float value);

	/** Set Player Rotaior */
	void TurnInPlace(float value);
	void TurnMove();
	FORCEINLINE int GetTurnAxis() { return TurnAxis; }

	/** If Sprinting end then Zoom the camera and shaking */
	void SetArms(USpringArmComponent* Arm);
	void ZoomInCam(USpringArmComponent* Arm, FRotator Rot = FRotator(0.f));
	void ZoomOutCam();
	void RunCamShake();
#pragma endregion
#pragma region MOVEMENT
private:
	UPROPERTY(EditDefaultsOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float BowSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float SprintingSpeed;

	/** Dodge */
	float DodgeSpeed;
	bool bCanDodge;

	/** Turn In Place */
	float DirX;
	float DirY;

	/** Target Enemy */
	bool bTargeting;
public:
	//���� ĳ������ ���¸� ǥ�� (����,����,����,���� ���)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementStatus MovementStatus;

	virtual void Jump() override;

	FORCEINLINE EMovementStatus GetMovementStatus() { return MovementStatus; }
	void SetMovementStatus(EMovementStatus Status);
	void MoveForward(float Value);
	void MoveRight(float Value);
	void OnSprinting();
	void OffSprinting();
	
	/** Dodge */
	void Dodge(); 
	void AnimDodge();
	bool IsCanMove();

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();

	/** Target Enemy */
	void Targeting();
	void SetTargeting();
	void OnTargeting();
	void OffTargeting();
	FORCEINLINE bool GetTargeting() { return bTargeting; }
#pragma endregion
#pragma region MONTAGE
	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	int ComboMaxCnt;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* SwordAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* MaceAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* SpearAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* SkillAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* HitedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* DeathMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* PickUpMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* FaintMontage;
#pragma endregion
#pragma region ATTACK
private:
	class UPlayerAttackFunction* AttackFunction;
	class UAnimInstance* AnimInstance; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat | Weapon", Meta = (AllowPrivateAccess = true))
	EWeaponStatus WeaponStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	ECombatStatus CombatStatus;

	UPROPERTY(EditAnywhere, Category = "Combat", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> PlayerDamageType;

	UPROPERTY(EditAnywhere, Category = "Combat", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> InternalDamageType;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	int AttackCnt = 0;

	int ComboCnt;			
	float DefaultAttackRange;		
	float DefaultDamage;
	float AttackRange;
	float AttackDamage;

	/** Attack Check */
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	FString LastAttack = "";

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	FString CurrentAttack = "";

	/** Powerful Attack */
	bool bAltPressed = false;
	bool bLMBDown;
	bool bAttacking;
	bool bIsAttackCheck;
	
	/** Bow */
	class ABowWeapon* Bow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat | Bow", Meta = (AllowPrivateAccess = true))
	bool bBowCharging = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Combat | Bow", Meta = (AllowPrivateAccess = true))
	float ChargeAmount = 0;
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EWeaponStatus GetWeaponStatus() { return WeaponStatus; }
	FORCEINLINE void SetWeaponStatus(EWeaponStatus Status) { WeaponStatus = Status; }

	FORCEINLINE void SetCombatStatus(ECombatStatus State) { CombatStatus = State; }
	FORCEINLINE ECombatStatus GetCombatStatus() { return CombatStatus; }

	void SetAttackCnt();
	FORCEINLINE int GetAttackCnt() { return AttackCnt; }

	FORCEINLINE void SetAttackRange(float value) { AttackRange = value; }
	FORCEINLINE float GetAttackRange() { return AttackRange; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UPlayerAttackFunction* GetAttackFunction() { return AttackFunction; }

	FORCEINLINE void SetAttackDamage(float Value) { AttackDamage = Value; }

	void Attack(bool bIsSpecial = false);
	FName GetAttackMontageSection(FString Type, int32 Section);

	/** ��Ƽ���̸� ���Ͽ� ȣ�� �Ǹ� Sweep ��ü ���� */
	UFUNCTION(BlueprintCallable)
	void StartAttack();

	UFUNCTION(BlueprintCallable)
	void EndAttack();

	UFUNCTION(BlueprintCallable)
	void StartPowerfulAttack();

	UFUNCTION(BlueprintCallable)
	void OnWeaponCollision();

	UFUNCTION(BlueprintCallable)
	void OffWeaponCollision();

	/** �޺��� ������ ���ؼ� ��� */
	UFUNCTION(BlueprintCallable)
	void AttackInputCheck();

	void LMBDown();
	FORCEINLINE void LMBUp() { bLMBDown = false; }

	/** Set Can Use Powerful Attack */ 
	FORCEINLINE void AltDown() { bAltPressed = true; }
	FORCEINLINE void AltUp() { bAltPressed = false; }
	FORCEINLINE void SetCurrentAttack(FString Value) { CurrentAttack = Value; }

	/** Kick */
	void Kick();

	UFUNCTION(BlueprintCallable)
	void KickStart();

	/** Death */
	void Death();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	/** Hited */
	void Hited();

	UFUNCTION(BlueprintCallable)
	void HitEnd();

	/** Shield */
	void Blocking();
	void UnBlocking();
	bool IsBlockingSuccess(AActor* DamageCauser);

	/** Bow */
	void BeginCharge();
	void EndCharge();
	void BowAnimCharge();
#pragma endregion
#pragma region BALANCE
private:
	class UBalance* Balance;
	class ABoss_Enemy* BalanceTarget; 
	FTimerHandle BalanceHandle;
	float DecreaseBalanceTime;
	USphereComponent* EnemyBalanceOverlap;
	bool bCanSpecialAttack = false;
public:
	void SetBalanceRatio();
	void SetEnemyBalanceRatio();
	void BrokenBalance();
	void RecoverBalance();
	void CanSpeicalAttackToEnemy();		//if Enemy has broken the balance then player can use SpecialAttack

	FORCEINLINE UBalance* GetBalance() { return Balance; }

	UFUNCTION()
	void OnEnemyBalance_OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEnemyBalance_OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion
#pragma region SKILL
private:
	/** Skill */
	class UPlayerSkillFunction* SkillFunction;

	/** Grenade */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill | Grenade", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AGrenade> GrenadeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill | Grenade", Meta = (AllowPrivateAccess = true))
	bool bisThrow = false;

	class AGrenade* Grenade;
	FTimerHandle ThrowTimer;
public:
	/** Skill */
	void SkillController();
	void SkillBegin();
	void SkillEnd();	

	/** Grenade */
	void Throw();
	void StartThrow();
	void Throwing();
	void EndThrow();
#pragma endregion
#pragma region HEALTH
private:
	/** Health */
	float MaxHealth;
	float CurrentHealth;
	float HealthRatio = 0.f;

	/** Stamina */
	float MaxStamina;
	float CurrentStamina;
	float CoolUpStamina;
	float CoolDownStamina;
	float StaminaRatio = 0.f;
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Health */
	void SetHealthRatio();
	FORCEINLINE float GetHealthRatio() { return HealthRatio; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }

	/** Stamia */
	void SetStaminaRatio();
	FORCEINLINE float GetStaminaRatio() { return StaminaRatio; }	
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetCurrentStamina() { return CurrentStamina; }
#pragma endregion
#pragma region ACTIVE
private:
	UPROPERTY(VisibleAnywhere, Category = "Active | Weapon")
	class AItem* ActiveOverlappingItem;

	UPROPERTY(VisibleAnywhere, Category = "Active | Weapon")
	class AShieldWeapon* CurrentShieldWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Active | Weapon")
	class AWeapon* CurrentAttackWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Active | Particle", Meta = (AllowPrivateAccess = true))	//�ǰ� ȿ��
	class UParticleSystem* HitParticle;
public:
	/** Set Weapon */
	void SetShieldCurrentWeapon(AShieldWeapon* Weapon);
	void SetAttackCurrentWeapon(AWeapon* Weapon);

	FORCEINLINE void SetActiveOverlappingItem(AItem* item) { ActiveOverlappingItem = item; }
	FORCEINLINE UParticleSystem* GetHitParticle() { return HitParticle; }
	FORCEINLINE AShieldWeapon* GetShieldCurrentWeapon() { return CurrentShieldWeapon; }
	FORCEINLINE AWeapon* GetAttackCurrentWeapon() { return CurrentAttackWeapon; }

	/** Special Attack (Balance Check) || EquipItem */
	void ActiveInteraction();
	void DeactiveInteraction();

	void ItemEquip();
	void ItemDrop();

	UFUNCTION(BlueprintCallable)
	void SpecialAttackApplyDamage();
	void ActiveSpecialAttack();
#pragma endregion
#pragma region HUD
private:
	/** In Range Of Enemy */
	UPROPERTY(VisibleAnywhere, Category = "HUD")
	class AEnemy* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "HUD", Meta = (AllowPrivateAccess = true))
	USphereComponent* EnemyHUDOverlap;

	/** DamageText */
	UPROPERTY(EditAnywhere, Category = "HUD", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UDamageTextWidget> DamageTextWidget;

	/** Pause Menu */
	bool bESCDown;
public:
	UFUNCTION()
	void OnEnemyHUD_OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEnemyHUD_OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ESCDown();

	void SetFogSplatter();	
#pragma endregion	
#pragma region SOUND
private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (AllowPrivateAccess = true))
	TArray<class USoundWave*> HitedSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (AllowPrivateAccess = true))
	TArray<class USoundWave*> DeathSound;
#pragma endregion
};
