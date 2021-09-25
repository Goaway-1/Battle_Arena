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
	AController* EnemyController;

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

	//test start
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* SkillAttackMontage;

	UFUNCTION(BlueprintCallable)
	void SkillAttack();

	UFUNCTION(BlueprintCallable)
	void SkillAttackEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	class UEnemySkillFunction* SkillFunction;

	bool bisSkill = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	FTimerHandle SKillCoolTimer;

	/// <summary>
	/// test end
	/// </summary>

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
	void AttackStart();

	//Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> EnemyDamageType;

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
};
