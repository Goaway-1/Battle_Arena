#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_MoveOrSkill.generated.h"

/**
 * Differentiate use skill and movement based on the probability.
 */
UCLASS()
class COMBATARENA_API UBTDecorator_MoveOrSkill : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_MoveOrSkill();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
