#include "BTTask_Faint.h"
#include "Enemy.h"

UBTTask_Faint::UBTTask_Faint() {
	NodeName = "Faint";

	bNotifyTick = true;	//tick 사용
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Faint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy)	return EBTNodeResult::Failed;

	//여기 로직 수정.
	/*Enemy->ActiveFaint();
	IsAttacking = true;
	Enemy->OnAttackEnd.AddLambda([this]()-> void
		{
			IsAttacking = false;
		});*/

	return EBTNodeResult::InProgress;
}

void UBTTask_Faint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//여기서 테스크 종료
	/*if (!IsAttacking) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}*/
}