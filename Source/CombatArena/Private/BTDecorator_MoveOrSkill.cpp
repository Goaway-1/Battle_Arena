#include "BTDecorator_MoveOrSkill.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecorator_MoveOrSkill::UBTDecorator_MoveOrSkill()
{
	NodeName = TEXT("MoveOrSkill");
}

bool UBTDecorator_MoveOrSkill::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	/* 확률로 움직일지 스킬을 사용할지 측정
	*  0 ~ 3까지의 확률로 0일 경우 이동, 나머지는 스킬의 사용
	*/
	/*int rand = UKismetMathLibrary::RandomIntegerInRange(0, 2);
	bResult = (rand > 1) ? true : false;
	return bResult;*/

	//현재 skill 테스트중
	return true;
}