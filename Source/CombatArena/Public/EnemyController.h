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

	virtual void Tick(float DeltaTime) override;

	//키값들 -> 다른 코드에서 참조하기 편리하다.
	static const FName HomePosKey;
	static const FName PatrolPosKey;	
	static const FName TargetActor;
	static const FName IsInFog; 
	static const FName IsFaint;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BTree;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBlackboardData* BData;	

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category= "AI")
	class UAIPerceptionComponent* AIPerception;

	class AMainGameStateBase* MyGameState;
public:
	UFUNCTION()
	void Sense(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()	//Enemy 사망시
	void StopBeTree();

	UFUNCTION()
	FVector GetTargetVec();

	UFUNCTION()
	class AActor* GetCurrentTarget();

	/** Is In Fog */
	UFUNCTION()
	void SetVisibleInFog(bool bisin);

	/** IsFaint */
	UFUNCTION()
	void SetIsFaint(bool bisin);
};
