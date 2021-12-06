#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Faint.generated.h"

/**
 * 
 */
UCLASS()
class COMBATARENA_API UBTTask_Faint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Faint();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	bool IsFaint = false;
};
