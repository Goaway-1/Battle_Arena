#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

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
	bool IsAttacking = false;

	FOnAttackEndDelegate OnAttackEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	class UAnimMontage* AttackMontage;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void Attack();

	//Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category ="Attack")
	class UBoxComponent* AttackBox_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	class UBoxComponent* AttackBox_Right;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> EnemyDamageType;

	UFUNCTION()
	void OnAttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
	void ActiveOnCollision();
	
	UFUNCTION(BlueprintCallable)
	void DeActiveOnCollision();

	//knockback
	UPROPERTY()
	float KnockBackPower;

	UFUNCTION()
	void KnockBack();
#pragma endregion

#pragma region HEALTH
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")	//�ǰ� ȿ��
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
	void ShowEnemyHealth();
	
	UFUNCTION()
	void HideEnemyHealth();

	//HealthBar
	UPROPERTY()
	class UHealthWidget* HealthBar;

	UPROPERTY()
	float HealthRatio = 0.f;

	UFUNCTION(BlueprintCallable)
	void SetHealthRatio();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthRatio() { return HealthRatio; }
#pragma endregion
};
