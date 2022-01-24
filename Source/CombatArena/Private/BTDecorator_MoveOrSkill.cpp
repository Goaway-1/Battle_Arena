#include "BTDecorator_MoveOrSkill.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecorator_MoveOrSkill::UBTDecorator_MoveOrSkill()
{
	NodeName = TEXT("MoveOrSkill");
}

bool UBTDecorator_MoveOrSkill::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, 5);
	bResult = (rand >= 1) ? true : false;
	return bResult;
}