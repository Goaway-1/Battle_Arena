#pragma once

#include "EngineMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

UCLASS()
class COMBATARENA_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();

	virtual void BeginPlay() override;

	//Ű���� -> �ٸ� �ڵ忡�� �����ϱ� ���ϴ�.
	static const FName HomePosKey;
	static const FName PatrolPosKey;	

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BTree;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBlackboardData* BData;	
};
