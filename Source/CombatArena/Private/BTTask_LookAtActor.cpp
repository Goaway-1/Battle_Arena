#include "BTTask_LookAtActor.h"
#include "Enemy.h"
#include "AIController.h"
#include "EnemyController.h"
#include "MainPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_LookAtActor::UBTTask_LookAtActor() {
	NodeName = TEXT("LookAtActor");
}

EBTNodeResult::Type UBTTask_LookAtActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);
	FVector LookVec;

	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	AMainPlayer* Target = Cast<AMainPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyController::TargetActor));
	if (!Target) return EBTNodeResult::Failed;
	LookVec = Target->GetActorLocation() - Enemy->GetActorLocation();
	LookVec.Z = 0;
	FRotator LookRot = FRotationMatrix::MakeFromX(LookVec).Rotator();
	Enemy->SetActorRotation(FMath::RInterpTo(Enemy->GetActorRotation(), LookRot, GetWorld()->GetDeltaSeconds(), 2.0f));
	return EBTNodeResult::Succeeded;
}
