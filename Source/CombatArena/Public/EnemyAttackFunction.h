#pragma once

#include "EngineMinimal.h"
#include "AttackFunction.h"
#include "EnemyAttackFunction.generated.h"

/**
 * 
 */
UCLASS()
class COMBATARENA_API UEnemyAttackFunction : public UAttackFunction
{
	GENERATED_BODY()
public:
	UEnemyAttackFunction();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	class AEnemy* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	class UAnimInstance* OwnerAnimInstance;

	UFUNCTION()
	void AttackStart();
};
