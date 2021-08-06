#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"

const FName AEnemyController::HomePosKey(TEXT("HomePosKey"));
const FName AEnemyController::PatrolPosKey(TEXT("PatrolPosKey"));
const FName AEnemyController::TargetActor(TEXT("TargetActor"));

AEnemyController::AEnemyController() {
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void AEnemyController::BeginPlay() {
	Super::BeginPlay();

    //���� �����ϴ� OnTargetPerceptionUpdated ��������Ʈ�� Controller�� �ƴ� C++���� ����
    AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::Sense);

    if (UseBlackboard(BData, Blackboard))
    {
        Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());	//Location�� HomePosKey��..
        UE_LOG(LogTemp, Warning, TEXT("%s"), *GetPawn()->GetActorLocation().ToString());
        if (!RunBehaviorTree(BTree)) return;    //����
    }
}

void AEnemyController::Sense(AActor* Actor, FAIStimulus Stimulus) {
    if (Stimulus.WasSuccessfullySensed()) Blackboard->SetValueAsObject(TargetActor, Actor);
	else Blackboard->ClearValue(TargetActor);
}

void AEnemyController::StopBeTree() {
    UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
    BTComp->StopTree();
}