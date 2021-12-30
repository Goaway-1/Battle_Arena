#include "BTDecorator_IsInAttackRange.h"
#include "EnemyController.h"
#include "MainPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");	
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)	return false;

	auto Target = Cast<AMainPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyController::TargetActor));
	if (nullptr == Target) return false;
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 520.0f);

	return bResult;
}