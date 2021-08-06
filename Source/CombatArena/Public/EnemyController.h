#pragma once

#include "EngineMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

UCLASS()
class COMBATARENA_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();

	virtual void BeginPlay() override;

	//Ű���� -> �ٸ� �ڵ忡�� �����ϱ� �����ϴ�.
	static const FName HomePosKey;
	static const FName PatrolPosKey;	
	static const FName TargetActor;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BTree;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBlackboardData* BData;	

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category= "AI")
	class UAIPerceptionComponent* AIPerception;

	UFUNCTION()
	void Sense(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()	//Ʈ���� ����. (Enemy �����)
	void StopBeTree();
};