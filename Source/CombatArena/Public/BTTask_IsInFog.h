#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IsInFog.generated.h"

/**
 * The reaction when the enemy is in the fogs
 */
UCLASS()
class COMBATARENA_API UBTTask_IsInFog : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_IsInFog();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
