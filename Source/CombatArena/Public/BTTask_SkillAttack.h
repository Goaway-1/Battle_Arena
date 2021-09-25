#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SkillAttack.generated.h"

/**
 * 
 */
UCLASS()
class COMBATARENA_API UBTTask_SkillAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SkillAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool IsAttacking = false;
};
