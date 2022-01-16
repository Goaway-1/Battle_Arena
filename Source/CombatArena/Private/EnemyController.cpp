#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "MainPlayer.h"

const FName AEnemyController::HomePosKey(TEXT("HomePosKey"));
const FName AEnemyController::PatrolPosKey(TEXT("PatrolPosKey"));
const FName AEnemyController::TargetActor(TEXT("TargetActor"));
const FName AEnemyController::IsInFog(TEXT("IsInFog")); 
const FName AEnemyController::IsFaint(TEXT("IsFaint"));

AEnemyController::AEnemyController() {
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void AEnemyController::BeginPlay() {
	Super::BeginPlay();

    //���� �����ϴ� OnTargetPerceptionUpdated ��������Ʈ�� Controller�� �ƴ� C++���� ����
    AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::Sense);

    if (UseBlackboard(BData, Blackboard)){
        if (!RunBehaviorTree(BTree)) return;
        Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());	//Location�� HomePosKey��..
    }
}

void AEnemyController::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
    if (!RunBehaviorTree(BTree)) return;
    Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());	
}

void AEnemyController::Sense(AActor* Actor, FAIStimulus Stimulus) {
    if (Stimulus.WasSuccessfullySensed()) {
        AMainPlayer* Player = Cast<AMainPlayer>(Actor);
        if(Player == nullptr) return;
        Blackboard->SetValueAsObject(TargetActor, Actor);
    }
	else Blackboard->ClearValue(TargetActor);
}

void AEnemyController::StopBeTree() {
    UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
    BTComp->StopTree();
}

FVector AEnemyController::GetTargetVec() {
    AMainPlayer* Player = Cast<AMainPlayer>(Blackboard->GetValueAsObject(TargetActor));
    if(Player) {
        FVector Vec =  Player->GetActorLocation();
        Vec.Z -= 86.f;      //�ٴڿ� Ÿ�� ǥ�� ����.
        return Vec; 
    }
    else return FVector(0.f);
}

AActor* AEnemyController::GetCurrentTarget() {
    AActor* Player = Cast<AActor>(Blackboard->GetValueAsObject(TargetActor));
    return Player;
}

void AEnemyController::SetVisibleInFog(bool bisin) {
    Blackboard->SetValueAsBool(IsInFog, bisin);
}

void AEnemyController::SetIsFaint(bool bisin) {
    Blackboard->SetValueAsBool(IsFaint, bisin);
}