#include "BTTask_LookAround.h"
#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_LookAround::UBTTask_LookAround() {
	NodeName = TEXT("LookAround");
}

EBTNodeResult::Type UBTTask_LookAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	/** 실패 시 걸어가고 성공시 플레이어 주변 탐색 */
	int rand = UKismetMathLibrary::RandomIntegerInRange(0, 100);
	bool bResult = (rand > 50) ? true : false;
	UE_LOG(LogTemp, Warning, TEXT("Move : %d"), rand);
	if(!bResult) return EBTNodeResult::Failed; 

	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	/** 주변 탐색 */
	Enemy->LookAround();
	return EBTNodeResult::Succeeded;
}
