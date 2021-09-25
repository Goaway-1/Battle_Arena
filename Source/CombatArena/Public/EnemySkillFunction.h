#pragma once

#include "CoreMinimal.h"
#include "SkillFunction.h"
#include "EnemySkillFunction.generated.h"

/**
 * 
 */
UCLASS()
class COMBATARENA_API UEnemySkillFunction : public USkillFunction
{
	GENERATED_BODY()
public:
	UEnemySkillFunction();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Ground */
	virtual void GroundAttack() override;

	virtual void SetSkillLocation() override;

	virtual void ConfirmTargetAndContinue() override;
};
