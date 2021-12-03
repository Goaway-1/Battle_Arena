#pragma once

#include "EngineMinimal.h"
#include "MainController.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

class AMainController;

UCLASS()
class COMBATARENA_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	class UEnemyAnim* Anim;

	UPROPERTY()
	class AEnemyController* EnemyController;

#pragma region ATTACK
	UPROPERTY()
	class UEnemyAttackFunction* AttackFunction;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UEnemyAttackFunction* GetAttackFuntion() { return AttackFunction; }

	bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange;

	FORCEINLINE float GetAttackRange() { return AttackRange; }

	FOnAttackEndDelegate OnAttackEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* SkillAttackMontage;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void Attack(FString type);

	/** If Attack Ready and go forward a little bit */
	UFUNCTION(BlueprintCallable)
	void AttackReady();

	UFUNCTION()
	FName GetAttackMontageSection(FString Type);

	/** Sweap 실행 */
	UFUNCTION(BlueprintCallable)	
	void AttackStart_Internal();

	//Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> CollisionDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> InternalDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackDamage;

	/** knockback */
	UPROPERTY()
	float KnockBackPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KnockBack")
	bool bIsback;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KnockBack")
	FVector BackVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KnockBack")
	FTimerHandle backHandle;

	UFUNCTION()
	void KnockBack(FVector Backward);

	UFUNCTION()
	void knockBackEnd();

	UFUNCTION()
	void IsKnockBack();

	/** Up The Sky */
	UFUNCTION()
	void LaunchSky(FVector Pos);

	/** Hited AnimMontage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* HitedMontage;

	/** Is in fog */
	UFUNCTION()
	void SetVisibleInFog(bool bisin, int time = 0);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Grenade")
	FTimerHandle FogHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	class UAnimMontage* IsInFogMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grenade")
	float SmokeTime;

	UFUNCTION()
	void ActiveFogEvent();

	UFUNCTION()
	void DeactiveFogEvent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Collision")
	UCapsuleComponent* LeftWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Collision")
	UCapsuleComponent* RightWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")	//test
	int AttackCnt;

	UFUNCTION(BlueprintCallable)
	void AttackStart_Collision(bool value);		/** if true then rightweapon On*/
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd_Collision();

	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	FString LastAttack = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	FString CurrentAttack = "";

	FORCEINLINE void SetCurrentAttack(FString Value) { CurrentAttack = Value; }
#pragma endregion
#pragma region SKILL
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	class UEnemySkillFunction* SkillFunction;

	bool bisSkill = false;

	FString SkillType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	FTimerHandle SKillCoolTimer;

	UFUNCTION(BlueprintCallable)
	void SkillAttack();

	UFUNCTION(BlueprintCallable)
	void SkillAttackEnd();
#pragma endregion
#pragma region HEALTH
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")	//피격 효과
	class UParticleSystem* HitParticle;

	FORCEINLINE UParticleSystem* GetHitParticle() { return HitParticle; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator,AActor* DamageCauser) override;

	UFUNCTION()
	void DeathEnd();

	UFUNCTION(BlueprintCallable)
	void DestroyEnemy();
#pragma endregion
#pragma region HUD
	UPROPERTY(VisibleAnywhere, Category = "Widget | EnemyWidget")
	class UWidgetComponent* HealthWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | EnemyWidget")
	TSubclassOf<class UUserWidget> WEnemyHealth;

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
	UPROPERTY()
	class UHealthWidget* HealthBar;

	UPROPERTY()
	float HealthRatio = 0.f;

	UFUNCTION(BlueprintCallable)
	void SetHealthRatio();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthRatio() { return HealthRatio; }

	//Targeting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | TargetWidget")
	class UDecalComponent* TargetingDecal;

	/** DamageText */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UDamageTextWidget> DamageTextWidget;
#pragma endregion
private:
	UPROPERTY(VisibleAnywhere, Category = "BALANCE")
	FTimerHandle BalanceHandle;

	UPROPERTY(VisibleAnywhere, Category = "BALANCE")
	float DecreaseBalanceTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BALANCE", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* FaintMontage;
	
	UPROPERTY(VisibleAnywhere, Category = "BALANCE")
	class UBalance* Balance;

public:
	UFUNCTION()
	void BrokenBalance();

	UFUNCTION()
	void ActiveFaint();

	UFUNCTION()
	void DeactiveFaint();

	FORCEINLINE UBalance* GetBalance() { return Balance; } 
};
