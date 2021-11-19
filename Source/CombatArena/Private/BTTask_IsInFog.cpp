#include "BTTask_IsInFog.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IsInFog::UBTTask_IsInFog() {
	NodeName = TEXT("IsInFog");
}

EBTNodeResult::Type UBTTask_IsInFog::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	/*AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;*/

	UE_LOG(LogTemp, Warning, TEXT("Enemy is in fog"));
	return EBTNodeResult::Succeeded;
}
