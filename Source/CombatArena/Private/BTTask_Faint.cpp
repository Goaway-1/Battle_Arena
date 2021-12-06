#include "BTTask_Faint.h"
#include "Enemy.h"
#include "Enemycontroller.h"

UBTTask_Faint::UBTTask_Faint() {
	NodeName = "Faint";

	bNotifyTick = false;	//tick »ç¿ë
	IsFaint = false;
}

EBTNodeResult::Type UBTTask_Faint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy)	return EBTNodeResult::Failed;

	Enemy->ActiveFaint();
	return EBTNodeResult::Succeeded;
}