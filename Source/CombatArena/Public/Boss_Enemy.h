#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Boss_Enemy.generated.h"

UCLASS()
class COMBATARENA_API ABoss_Enemy : public AEnemy
{
	GENERATED_BODY()
#pragma region INIT
private:
	ABoss_Enemy();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Attack(FString type) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
#pragma endregion
#pragma region MONATGE
private:
	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* SkillAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* FaintMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* LookAroundMontage;
#pragma endregion
#pragma region SKILL
private:
	UPROPERTY(VisibleAnywhere, Category = "Skill", Meta = (AllowPrivateAccess = true))
	class UEnemySkillFunction* ESkillFunction;
	
	bool bisSkill = false;
	FTimerHandle SKillCoolTimer;
public:
	UFUNCTION(BlueprintCallable)
	void SkillAttack();

	UFUNCTION(BlueprintCallable)
	void SkillAttackEnd();
	
	void DashSkill();
	
	UFUNCTION()
	void StartLookAround(bool isLeft);
#pragma endregion
#pragma region BALANCE
private:
	class UBalance* Balance;
	FTimerHandle BalanceHandle;
	float DecreaseBalanceTime;
	
	UPROPERTY(VisibleAnywhere, Category = "Balance")
	bool bIsFainted = false;
public:
	void BrokenBalance();
	void ActiveFaint();
	
	UFUNCTION(BlueprintCallable)
	void DeactiveFaint();
	
	void SpecialHitMontage();

	FORCEINLINE UBalance* GetBalance() { return Balance; }
	FORCEINLINE bool GetIsFainted() { return bIsFainted; }
#pragma endregion
};
