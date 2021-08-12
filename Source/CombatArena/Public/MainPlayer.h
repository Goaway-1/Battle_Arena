#pragma once

#include "EngineMinimal.h"
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

UENUM(BlueprintType)
enum class EWeaponStatus : uint8 {
	EWS_Normal		UMETA(DisplayName = "Normal"),
	EWS_Weapon		UMETA(DisplayName = "Weapon"),

	EWS_Default		UMETA(DisplayName = "Default")
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

	//Camera Shake
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UMatineeCameraShake> CamShake;
private:

	UPROPERTY(EditDefaultsOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	float CameraSpeed = 20.f;

public:
	void Lookup(float value);

	void Turn(float value);

#pragma endregion

#pragma region MOVEMENT
public:
	void MoveForward(float Value);

	void MoveRight(float Value);

	virtual void Jump() override;

	void Switch_Sprinting();

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

	//���� �ӵ��� �����Ͽ� Idle�� walk�� ���� (ī�޶��� ȸ�� ����)
	void CheckIdle();

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
#pragma endregion

#pragma region ATTACK
public:
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	int ComboMaxCnt;		//�ִ� ���� Ƚ��

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponStatus WeaponStatus;

	FORCEINLINE void SetWeaponStatus(EWeaponStatus Status) { WeaponStatus = Status; }

	UFUNCTION()
	FORCEINLINE EWeaponStatus GetWeaponStatus() { return WeaponStatus; }

	void Attack();

	UFUNCTION(BlueprintCallable)
	void EndAttack();

	UFUNCTION(BlueprintCallable)
	void AttackInputCheck();

	void LMBDown();
	FORCEINLINE void LMBUp() { bLMBDown = false; }

	FName GetAttackMontageSection(FString Type,int32 Section);

	UFUNCTION(BlueprintCallable)	//Weapon�� �ݸ����� Ű�� ���� ���
	void ActiveWeaponCollision();

	UFUNCTION(BlueprintCallable)	//Weapon�� �ݸ����� Ű�� ���� ���
	void DeActiveWeaponCollision();

	//Another Attack Way
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack | Kick")
	float KickRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack | Kick")
	float KickRadius;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Attack | Kick")
	bool bKicking;			//���� Kick ������
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | Kick")
	bool bCanKick;			//Kick�� ��������

	UFUNCTION()
	void Kick();

	UFUNCTION(BlueprintCallable)
	void KickStart();

	UFUNCTION(BlueprintCallable)
	void KickEnd();
	//End

	//Death
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* DeathMontage;

	UFUNCTION()
	void Death();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();
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
#pragma endregion

#pragma region ACTIVE

	//���� ��ģ ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Weapon")
	class AItem* ActiveOverlappingItem;

	FORCEINLINE void SetActiveOverlappingItem(AItem* item) { ActiveOverlappingItem = item; }

	//���� �������� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Weapon")
	class AWeapon* CurrentWeapon;

	FORCEINLINE void SetCurrentWeapon(AWeapon* Weapon) { CurrentWeapon = Weapon; }
	FORCEINLINE AWeapon* GetCurrentWeapon() { return CurrentWeapon; }

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
#pragma endregion

};
