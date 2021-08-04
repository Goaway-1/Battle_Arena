#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LookAtActor.generated.h"

UCLASS()
class COMBATARENA_API UBTTask_LookAtActor : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_LookAtActor();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
