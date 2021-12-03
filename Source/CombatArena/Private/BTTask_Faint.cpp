#include "BTTask_Faint.h"
#include "Enemy.h"

UBTTask_Faint::UBTTask_Faint() {
	NodeName = "Faint";

	bNotifyTick = true;	//tick ���
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Faint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy)	return EBTNodeResult::Failed;

	//���� ���� ����.
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

	//���⼭ �׽�ũ ����
	/*if (!IsAttacking) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}*/
}