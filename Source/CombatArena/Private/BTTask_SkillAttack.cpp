#include "BTTask_SkillAttack.h"
#include "EnemyController.h"
#include "Boss_Enemy.h"
#include "MainPlayer.h"
#include "EnemyAnim.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SkillAttack::UBTTask_SkillAttack() {
	NodeName = "SkillAttack";

	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_SkillAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<ABoss_Enemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy)	return EBTNodeResult::Failed;

	AMainPlayer* Target = Cast<AMainPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyController::TargetActor));
	if (!Target) return EBTNodeResult::Failed;

	FVector LookVec = Target->GetActorLocation() - Enemy->GetActorLocation();
	LookVec.Z = 0;
	FRotator LookRot = FRotationMatrix::MakeFromX(LookVec).Rotator();
	Enemy->SetActorRotation(LookRot);

	Enemy->Attack("Skill");
	IsAttacking = true;
	auto Anim = Cast<UEnemyAnim>(Enemy->GetMesh()->GetAnimInstance());
	Anim->OnAttackEnd.AddLambda([this]()-> void{
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_SkillAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking) FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}