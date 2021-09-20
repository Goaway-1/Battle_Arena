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
	EMS_Death		UMETA(DisplayName = "Death"),

	EMS_Default		UMETA(DisplayName = "Default")
}; 

/** Weapon�� ���� ���� */
UENUM(BlueprintType)
enum class EWeaponStatus : uint8 {
	EWS_Normal		UMETA(DisplayName = "Normal"),
	EWS_Weapon		UMETA(DisplayName = "Weapon"),

	EWS_Default		UMETA(DisplayName = "Default")
};

/** Shield�� ���� ���� (����) */
UENUM(BlueprintType)
enum class ECombatStatus : uint8 {
	ECS_Normal			UMETA(DisplayName = "Normal"),
	ECS_Attacking		UMETA(DisplayName = "Attacking"),
	ECS_Blocking		UMETA(DisplayName = "Blocking"),

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	//Hited Camera Shake
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UMatineeCameraShake> CamShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class APlayerCameraManager* CameraManager;
private:

	UPROPERTY(EditDefaultsOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	float CameraSpeed = 20.f;

public:
	void Lookup(float value);

	void Turn(float value);

	/** If Sprinting end then Zoom the camera and shaking */
	UFUNCTION()
	void ZoomInCam(FVector Pos, FRotator Rot = FRotator(0.f));

	UFUNCTION()
	void ZoomOutCam();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UMatineeCameraShake> RunShake;

	UFUNCTION()
	void RunCamShake();

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
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float SprintingSpeed;

	//���� ĳ������ ���¸� ǥ�� (����,����,����,���� ���)
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

	//����
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

	/** Target ���� */
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

	bool bLMBDown;			//���� Ű�� ���ȴ��� ����
	bool bAttacking;		//���������� ����
	bool bIsAttackCheck;	//�� ������ ������ ���� ����
	int ComboCnt;			//���� ���� Ƚ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float DefaultAttackRange;		//���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange;		//���� ����

	FORCEINLINE float GetAttackRange() { return AttackRange; }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	int ComboMaxCnt;		//�ִ� ���� Ƚ��

	/** Weapon�� ���� ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponStatus WeaponStatus;

	FORCEINLINE void SetWeaponStatus(EWeaponStatus Status) { WeaponStatus = Status; }

	UFUNCTION()
	FORCEINLINE EWeaponStatus GetWeaponStatus() { return WeaponStatus; }

	/** ���� �ο� ������ ����  (Attack, Shield,,,)*/

	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> PlayerDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DefaultDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage;

	FORCEINLINE void SetAttackDamage(float Value) { AttackDamage = Value; }

	/** ��Ƽ���̸� ���Ͽ� ȣ�� �Ǹ� Sweep ��ü ���� */
	UFUNCTION(BlueprintCallable)
	void StartAttack();

	/** bAttacking�� false�� ��ȯ */
	UFUNCTION(BlueprintCallable)
	void EndAttack();

	/** �޺��� ������ ���ؼ� ��� */
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

#pragma endregion
#pragma region SKILL
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* SkillAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Decal")
	class UDecalComponent* SkillDecal;

	/** Skill Test*/
	UFUNCTION()
	void SkillBegin();

	UFUNCTION()
	void SkillEnd();

	//Lazer
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Lazer")
	AActor* Lazer;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | Lazer")
	TSubclassOf<class AActor> LazerClass;

	UFUNCTION()
	void LazerAttack();

	UFUNCTION()
	void LazerEnd();

	/** Targeting On Ground */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Ground")
	bool bGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Ground")
	FVector out;

	UFUNCTION()
	void GroundAttack();

	UFUNCTION()
	void SetSkillLocation(FVector& OutViewPoint);

	UFUNCTION()
	void ConfirmTargetAndContinue();
#pragma endregion

#pragma region HEALTH
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY()		//ü�� ����
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

	//���� ��ģ ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Weapon")
	class AItem* ActiveOverlappingItem;

	FORCEINLINE void SetActiveOverlappingItem(AItem* item) { ActiveOverlappingItem = item; }

	//���� �������� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Weapon")
	class AShieldWeapon* CurrentLeftWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AAttackWeapon* CurrentRightWeapon;

	void SetLeftCurrentWeapon(AShieldWeapon* Weapon);
	FORCEINLINE AShieldWeapon* GetLeftCurrentWeapon() { return CurrentLeftWeapon; }

	void SetRightCurrentWeapon(AAttackWeapon* Weapon);
	FORCEINLINE AAttackWeapon* GetRightCurrentWeapon() { return CurrentRightWeapon; }

	UFUNCTION()
	void ItemEquip();

	UFUNCTION()
	void ItemDrop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")	//�ǰ� ȿ��
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
