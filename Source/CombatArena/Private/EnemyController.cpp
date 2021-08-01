#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEnemyController::HomePosKey(TEXT("HomePosKey"));
const FName AEnemyController::PatrolPosKey(TEXT("PatrolPosKey"));

AEnemyController::AEnemyController() {

}

void AEnemyController::BeginPlay() {
	Super::BeginPlay();

    if (UseBlackboard(BData, Blackboard))
    {
        Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());	//Location을 HomePosKey에..
        UE_LOG(LogTemp, Warning, TEXT("%s"), *GetPawn()->GetActorLocation().ToString());
        if (!RunBehaviorTree(BTree)) return;    //수정
    }
}