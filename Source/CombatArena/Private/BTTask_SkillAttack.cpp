#include "BTTask_SkillAttack.h"
#include "EnemyController.h"
#include "Enemy.h"
#include "MainPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SkillAttack::UBTTask_SkillAttack() {
	NodeName = "SkillAttack";

	bNotifyTick = true;	//tick ���
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_SkillAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy)	return EBTNodeResult::Failed;

	AMainPlayer* Target = Cast<AMainPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyController::TargetActor));
	if (!Target) return EBTNodeResult::Failed;

	FVector LookVec = Target->GetActorLocation() - Enemy->GetActorLocation();
	LookVec.Z = 0;
	FRotator LookRot = FRotationMatrix::MakeFromX(LookVec).Rotator();
	Enemy->SetActorRotation(LookRot);

	Enemy->Attack("Skill");
	IsAttacking = true;
	Enemy->OnAttackEnd.AddLambda([this]()-> void
		{
			IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UBTTask_SkillAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//���⼭ �׽�ũ ����
	if (!IsAttacking) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}