#include "BTTask_Faint.h"
#include "Boss_Enemy.h"
#include "Enemycontroller.h"

UBTTask_Faint::UBTTask_Faint() {
	NodeName = "Faint";

	bNotifyTick = false;	
	IsFaint = false;
}

EBTNodeResult::Type UBTTask_Faint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<ABoss_Enemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy)	return EBTNodeResult::Failed;

	Enemy->ActiveFaint();
	return EBTNodeResult::Succeeded;
}