#include "BTTask_Attack.h"
#include "EnemyController.h"
#include "Enemy.h"

UBTTask_Attack::UBTTask_Attack() {
	NodeName = "Attack";

	bNotifyTick = true;	//tick 사용
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy)	return EBTNodeResult::Failed;

	Enemy->Attack("Melee");
	IsAttacking = true;
	Enemy->OnAttackEnd.AddLambda([this]()-> void
	{
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//여기서 테스크 종료
	if (!IsAttacking) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}