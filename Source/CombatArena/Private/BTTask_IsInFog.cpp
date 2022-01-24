#include "BTTask_IsInFog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy.h"
#include "EnemyController.h"

UBTTask_IsInFog::UBTTask_IsInFog() {
	NodeName = TEXT("IsInFog");
}

EBTNodeResult::Type UBTTask_IsInFog::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!bIsinFog) {
		AEnemyController* EnemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
		AEnemy* Enemy = Cast<AEnemy>(EnemyController->GetPawn());
		if (!Enemy) return EBTNodeResult::Failed;
		Enemy->ActiveFogEvent();

		bIsinFog = true;
		return EBTNodeResult::Succeeded;
	}
	bIsinFog = false;
	return EBTNodeResult::Failed;
}