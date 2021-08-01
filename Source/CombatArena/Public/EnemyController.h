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

	//키값들 -> 다른 코드에서 참조하기 편리하다.
	static const FName HomePosKey;
	static const FName PatrolPosKey;	

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BTree;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBlackboardData* BData;	
};
