#include "BTDecorator_MoveOrSkill.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecorator_MoveOrSkill::UBTDecorator_MoveOrSkill()
{
	NodeName = TEXT("MoveOrSkill");
}

bool UBTDecorator_MoveOrSkill::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	/* Ȯ���� �������� ��ų�� ������� ����
	*  0 ~ 3������ Ȯ���� 0�� ��� �̵�, �������� ��ų�� ���
	*/
	/*int rand = UKismetMathLibrary::RandomIntegerInRange(0, 2);
	bResult = (rand > 1) ? true : false;
	return bResult;*/

	//���� skill �׽�Ʈ��
	return true;
}