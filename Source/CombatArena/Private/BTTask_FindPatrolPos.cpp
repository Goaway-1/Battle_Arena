#include "BTTask_FindPatrolPos.h"
#include "EnemyController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
    NodeName = TEXT("FindPatrolPos");           
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)return EBTNodeResult::Failed;

    //NavigationSystem�� ����ϱ� ���ؼ� Enemy�� Pawn�� �ҷ��´�.
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
    if (nullptr == NavSystem)  return EBTNodeResult::Failed; 

    //EnemyController�� BeginPlay���� ������ HomePosKey�� ���� ������ ���� ���Ŀ��� ���
    FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AEnemyController::HomePosKey);
 
    FNavLocation NextPatrol;

    //200 ���� ������ �� �� �ִ� ���� ��ǥ�� NetPatrol�� �����ϰ� SetValueAsVector�� Ű���� �����͸� �����Ѵ�.
    if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 200.f, NextPatrol)){
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEnemyController::PatrolPosKey, NextPatrol.Location);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}