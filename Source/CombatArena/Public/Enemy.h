#pragma once

#include "EngineMinimal.h"
#include "MainController.h"
#include "EnemyAttackFunction.h"
#include "EnemyAnim.h"
#include "EnemyController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class AMainController;

UCLASS()
class COMBATARENA_API AEnemy : public ACharacter
{
	GENERATED_BODY()

#pragma region INIT
public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion
#pragma region MOVEMENT
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	class UEnemyAnim* Anim;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	class AEnemyController* EnemyController;
#pragma endregion
#pragma region MONTAGE
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* IsInFogMontage;
	
public:
	UFUNCTION()
	virtual FName GetAttackMontageSection(FString Type);
#pragma endregion
#pragma region ATTACK
private:
	class UEnemyAttackFunction* AttackFunction;

	UPROPERTY(EditAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool IsAttacking = false;

	/** DamageType */
	UPROPERTY(EditAnywhere,Category = "Attack", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> CollisionDamageType;

	UPROPERTY(EditAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> InternalDamageType;

	/** Handle */
	bool bIsback;
	FVector BackVector;
	FTimerHandle backHandle;
	FTimerHandle FogHandle;
	float SmokeTime;

	/** Weapon */
	UPROPERTY(EditAnywhere, Category = "Attack | Collision", Meta = (AllowPrivateAccess = true))
	UCapsuleComponent* LeftWeapon;

	UPROPERTY(EditAnywhere, Category = "Attack | Collision", Meta = (AllowPrivateAccess = true))
	UCapsuleComponent* RightWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int AttackCnt;
	
	//일반 공격의 종류
	UPROPERTY(VisibleAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int AttackTypeCnt = 2;

	UPROPERTY(VisibleAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FString LastAttack = "";

	UPROPERTY(VisibleAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FString CurrentAttack = "";

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY()
	float KnockBackPower;

	FString SkillType;

public:
	FORCEINLINE bool GetIsAttacking() { return IsAttacking; }

	FORCEINLINE void SetIsAttacking(bool val) { IsAttacking = val; }
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UEnemyAttackFunction* GetAttackFuntion() { return AttackFunction; }

	FORCEINLINE float GetAttackRange() { return AttackRange; }

	FORCEINLINE int GetAttackTypeCnt() { return AttackTypeCnt; }
	
	UFUNCTION()
	virtual void Attack(FString type);

	/** If Attack Ready and go forward a little bit */
	UFUNCTION(BlueprintCallable)
	void AttackReady();

	/** Sweap 실행 */
	UFUNCTION(BlueprintCallable)	
	void AttackStart_Internal();

	void KnockBack(FVector Backward);
	void knockBackEnd();
	void IsKnockBack();

	/** Skill */
	void LaunchSky(FVector Pos);
	void SetVisibleInFog(bool bisin, int time = 0);
	void ActiveFogEvent();
	void DeactiveFogEvent();

	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void AttackStart_Collision(bool value);		/** if true then rightweapon On*/
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd_Collision();

	FORCEINLINE void SetCurrentAttack(FString Value) { CurrentAttack = Value; }
#pragma endregion
#pragma region HEALTH
protected:
	UPROPERTY(EditAnywhere, Category = "Particle", Meta = (AllowPrivateAccess = true))	//피격 효과
	class UParticleSystem* HitParticle;

	UPROPERTY(VisibleAnywhere, Category = "Health", Meta = (AllowPrivateAccess = true))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Health", Meta = (AllowPrivateAccess = true))
	float CurrentHealth;

public:
	FORCEINLINE UParticleSystem* GetHitParticle() { return HitParticle; }

	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator,AActor* DamageCauser) override;

	UFUNCTION()
	void DeathEnd();

	UFUNCTION(BlueprintCallable)
	void DestroyEnemy();
#pragma endregion
#pragma region HUD
private:
	UPROPERTY(VisibleAnywhere, Category = "Widget | EnemyWidget", Meta = (AllowPrivateAccess = true))
	class UWidgetComponent* HealthWidget;

	UPROPERTY()
	class UHealthWidget* HealthBar;

	UPROPERTY()
	float HealthRatio = 1.f;
	
	UPROPERTY(EditAnywhere,Category = "Widget | TargetWidget", Meta = (AllowPrivateAccess = true))
	class UDecalComponent* TargetingDecal;
public:
	/** DamageText */
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UDamageTextWidget> DamageTextWidget;

	//Call at MainPlayer
	UFUNCTION()
	void ShowEnemyTarget();
	
	UFUNCTION()
	void HideEnemyTarget();

	UFUNCTION()
	void ShowEnemyHUD();

	UFUNCTION()
	void HideEnemyHUD();

	//HealthBar
	UFUNCTION(BlueprintCallable)
	void SetHealthRatio();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthRatio() { return HealthRatio; }

#pragma endregion
#pragma region SOUND
private:
	//Hited
	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (AllowPrivateAccess = true))
	class USoundBase* HitedSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (AllowPrivateAccess = true))
	class USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (AllowPrivateAccess = true))
	class USoundBase* SwingSound;

public:
	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	UFUNCTION(BlueprintCallable)
	void PlayDeathSound();

	UFUNCTION(BlueprintCallable)
	void PlayHitedSound();
#pragma endregion
};
