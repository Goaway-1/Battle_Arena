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
	EMS_Drawing		UMETA(DisplayName = "Drawing"),
	EMS_Throwing	UMETA(DisplayName = "Throwing"),
	EMS_Death		UMETA(DisplayName = "Death"),

	EMS_Default		UMETA(DisplayName = "Default")
}; 

/** Weapon의 장착 여부 */
UENUM(BlueprintType)
enum class EWeaponStatus : uint8 {
	EWS_Normal		UMETA(DisplayName = "Normal"),
	EWS_Shield		UMETA(DisplayName = "Shield"),
	EWS_Melee		UMETA(DisplayName = "Melee"),
	EWS_Bow			UMETA(DisplayName = "Bow"),

	EWS_Default		UMETA(DisplayName = "Default")
};

/** Shield의 상태 여부 (방어여부) */
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
	class AMainController* PlayerController;		//GetController and Save

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region AIPERCEPTION
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AIPERCEPTION")
	class UAIPerceptionStimuliSourceComponent* AIPerceptionSource;
#pragma endregion

#pragma region CAMERA
	/** SpringArms */ 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USceneComponent* SpringArmSence;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm_Sprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm_Attacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm_Drawing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm_Skilling;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	/** Hited Camera Shake */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UMatineeCameraShake> CamShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class APlayerCameraManager* CameraManager;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	float CameraSpeed = 20.f;

	/** Turn Axis */
	int TurnAxis = 0;

public:
	void Lookup(float value);

	void Turn(float value);

	/** If Sprinting end then Zoom the camera and shaking */
	UFUNCTION()
	void ZoomInCam(USpringArmComponent* Arm, FRotator Rot = FRotator(0.f));

	UFUNCTION()
	void ZoomOutCam();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UMatineeCameraShake> RunShake;

	UFUNCTION()
	void RunCamShake();

	UFUNCTION()
	void TurnInPlace(float value);

	UFUNCTION()
	FORCEINLINE int GetTurnAxis() {return TurnAxis; }

	UFUNCTION()
	void SetArms(USpringArmComponent* Arm);
#pragma endregion

#pragma region MOVEMENT
public:
	void MoveForward(float Value);

	void MoveRight(float Value);

	virtual void Jump() override;

	void OnSprinting();
	void OffSprinting();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float BowSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float SprintingSpeed;

	//현재 캐릭터의 상태를 표현 (스턴,러닝,정지,공격 등등)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = true))
	EMovementStatus MovementStatus;	

	void SetMovementStatus(EMovementStatus Status);

	FORCEINLINE EMovementStatus GetMovementStatus() { return MovementStatus; }

	/** Dodge */
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Dodge")
	float DodgeSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Dodge")
	float DodgeCoolDownTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Dodge")
	float DodgeStopTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Dodge")
	bool bCanDodge;

	//방향
	UPROPERTY(VisibleAnywhere, Category = "Movement | Dodge")
	float DirX;

	UPROPERTY(VisibleAnywhere ,Category = "Movement | Dodge")
	float DirY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Dodge")
	FTimerHandle DodgeHandle;

	UFUNCTION()
	void Dodge();

	UFUNCTION()
	void DodgeEnd();

	UFUNCTION()
	void ResetDodge();

	UFUNCTION()
	void AnimDodge();

	UFUNCTION()
	bool IsCanMove();

	/** Target 관련 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting")
	bool bTargeting;

	UFUNCTION()
	void Targeting();

	UFUNCTION()
	void SetTargeting();

	UFUNCTION()
	void OnTargeting();

	UFUNCTION()
	void OffTargeting();

	UFUNCTION()
	bool GetTargeting() { return bTargeting; }
#pragma endregion

#pragma region ATTACK
public:
	UPROPERTY()
	class UPlayerAttackFunction* AttackFunction;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UPlayerAttackFunction* GetAttackFunction() { return AttackFunction; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	class UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* WeaponAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* SkillAttackMontage;

	bool bLMBDown;			//공격 키가 눌렸는지 여부
	bool bAttacking;		//공격중인지 여부
	bool bIsAttackCheck;	//또 공격할 건지에 대한 여부
	int ComboCnt;			//현재 공격 횟수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float DefaultAttackRange;		//공격 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange;		//공격 범위

	FORCEINLINE float GetAttackRange() { return AttackRange; }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	int ComboMaxCnt;		//최대 공격 횟수

	/** Weapon의 장착 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponStatus WeaponStatus;

	FORCEINLINE void SetWeaponStatus(EWeaponStatus Status) { WeaponStatus = Status; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EWeaponStatus GetWeaponStatus() { return WeaponStatus; }

	/** 현재 싸움 상태의 종류  (Attack, Shield,,,)*/

	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> PlayerDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DefaultDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage;

	FORCEINLINE void SetAttackDamage(float Value) { AttackDamage = Value; }

	/** 노티파이를 통하여 호출 되며 Sweep 구체 생성 */
	UFUNCTION(BlueprintCallable)
	void StartAttack();

	/** bAttacking을 false로 전환 */
	UFUNCTION(BlueprintCallable)
	void EndAttack();

	/** 콤보의 구현을 위해서 사용 */
	UFUNCTION(BlueprintCallable)
	void AttackInputCheck();

	void LMBDown();
	FORCEINLINE void LMBUp() { bLMBDown = false; }

	FName GetAttackMontageSection(FString Type,int32 Section);

	UFUNCTION()
	void Kick();

	UFUNCTION(BlueprintCallable)
	void KickStart();

	/** Death */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* DeathMontage;

	UFUNCTION()
	void Death();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	/** Shield */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ECombatStatus CombatStatus;

	FORCEINLINE void SetCombatStatus(ECombatStatus State) { CombatStatus = State; }
	FORCEINLINE ECombatStatus GetCombatStatus() { return CombatStatus; }

	UFUNCTION()
	void Blocking();

	UFUNCTION()
	void UnBlocking();

	UFUNCTION()
	bool IsBlockingSuccess(AActor* DamageCauser);

	/** Bow */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bow")
	class ABowWeapon* Bow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bow")
	float ChargeAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	bool bBowCharging = false;

	UFUNCTION()
	void BeginCharge();

	UFUNCTION()
	void EndCharge();

	UFUNCTION()
	void BowAnimCharge();

#pragma endregion

#pragma region SKILL
	/** Skill */
	UFUNCTION()
	void SkillController();

	UFUNCTION()
	void SkillBegin();

	UFUNCTION()
	void SkillEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	class UPlayerSkillFunction* SkillFunction;
#pragma endregion

#pragma region THROW
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throw | Grenade")
	TSubclassOf<class AGrenade> GrenadeClass;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Throw | Grenade")
	class AGrenade* Grenade;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Throw | Grenade")
	bool bisThrow = false;	//test

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Throw | Grenade")
	FTimerHandle ThrowTimer;	//test

	UFUNCTION()
	void Throw();

	UFUNCTION()
	void StartThrow();

	UFUNCTION()
	void Throwing();

	UFUNCTION()
	void EndThrow();

	UFUNCTION()
	void RealEndThrow();
#pragma endregion

#pragma region HEALTH
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY()		//체력 비율
	float HealthRatio = 0.f;

	UFUNCTION()
	void SetHealthRatio();

	UFUNCTION()
	FORCEINLINE float GetHealthRatio() { return HealthRatio; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Stamina */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float CoolUpStamina; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float CoolDownStamina;

	UPROPERTY()		
	float StaminaRatio = 0.f;

	UFUNCTION()
	void SetStaminaRatio();

	UFUNCTION()
	FORCEINLINE float GetStaminaRatio() { return StaminaRatio; }

#pragma endregion

#pragma region ACTIVE

	//현재 겹친 아이템
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Weapon")
	class AItem* ActiveOverlappingItem;

	FORCEINLINE void SetActiveOverlappingItem(AItem* item) { ActiveOverlappingItem = item; }

	//현재 장착중인 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Weapon")
	class AShieldWeapon* CurrentShieldWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* CurrentAttackWeapon;

	void SetShieldCurrentWeapon(AShieldWeapon* Weapon);
	FORCEINLINE AShieldWeapon* GetShieldCurrentWeapon() { return CurrentShieldWeapon; }

	void SetAttackCurrentWeapon(AWeapon* Weapon);
	FORCEINLINE AWeapon* GetAttackCurrentWeapon() { return CurrentAttackWeapon; }

	UFUNCTION()
	void ItemEquip();

	UFUNCTION()
	void ItemDrop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")	//피격 효과
	class UParticleSystem* HitParticle;

	FORCEINLINE UParticleSystem* GetHitParticle() { return HitParticle; }
#pragma endregion

#pragma region HUD

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	class AEnemy* CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "HUD")
	USphereComponent* EnemyHUDOverlap;

	UFUNCTION()
	void OnEnemyHUD_OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEnemyHUD_OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** DamageText */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="HUD")
	TSubclassOf<class UDamageTextWidget> DamageTextWidget;

	/** Pause Menu */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "HUD")
	bool bESCDown;

	UFUNCTION()
	void ESCUp();
	
	UFUNCTION()
	void ESCDown();
#pragma endregion

#pragma region SAVE&LOAD
public:
	UFUNCTION(BlueprintCallable)
	void SaveData();

	UFUNCTION(BlueprintCallable)
	void LoadData();
#pragma endregion
};
