#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Boss_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class COMBATARENA_API ABoss_Enemy : public AEnemy
{
	GENERATED_BODY()
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	class UEnemySkillFunction* ESkillFunction;

	bool bisSkill = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	FTimerHandle SKillCoolTimer;

	UFUNCTION(BlueprintCallable)
	void SkillAttack();

	UFUNCTION(BlueprintCallable)
	void SkillAttackEnd();

	UFUNCTION()
	void DashSkill();
	/*
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);*/
};
