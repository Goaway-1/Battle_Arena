#include "BTTask_LookAround.h"
#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_LookAround::UBTTask_LookAround() {
	NodeName = TEXT("LookAround");
}

EBTNodeResult::Type UBTTask_LookAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	/** ���� �� �ɾ�� ������ �÷��̾� �ֺ� Ž�� */
	int rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);
	bool bResult = (rand == 0) ? true : false;
	if(!bResult) return EBTNodeResult::Failed; 

	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	/** �ֺ��� ���ƴٴ� (������) */ 
	rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);
	bResult = (rand == 0) ? true : false;
	Enemy->StartLookAround(bResult);
	return EBTNodeResult::Succeeded;
}
