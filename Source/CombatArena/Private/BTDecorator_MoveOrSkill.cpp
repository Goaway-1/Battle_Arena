#include "BTDecorator_MoveOrSkill.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecorator_MoveOrSkill::UBTDecorator_MoveOrSkill()
{
	NodeName = TEXT("MoveOrSkill");
}

bool UBTDecorator_MoveOrSkill::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	/** Ȯ���� �������� ��ų�� ������� ���� */
	int rand = UKismetMathLibrary::RandomIntegerInRange(0,100);
	bResult = (rand > 50) ? true : false;
	
	UE_LOG(LogTemp, Warning, TEXT("Move Or Skill : %d"),rand);

	return bResult;
}