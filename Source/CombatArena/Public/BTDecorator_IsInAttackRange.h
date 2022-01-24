#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackRange.generated.h"


UCLASS()
class COMBATARENA_API UBTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float CanAttackDistance = 500.f;
public:
	UBTDecorator_IsInAttackRange();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
