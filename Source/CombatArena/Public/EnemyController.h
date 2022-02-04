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

private:
	UPROPERTY(EditAnyWhere, Category = "AI", Meta = (AllowPrivateAccess = true))
	class UBehaviorTree* BTree;

	UPROPERTY(EditAnyWhere, Category = "AI", Meta = (AllowPrivateAccess = true))
	class UBlackboardData* BData;	

	UPROPERTY(EditAnyWhere, Category= "AI", Meta = (AllowPrivateAccess = true))
	class UAIPerceptionComponent* AIPerception;

	UPROPERTY(EditAnyWhere, Category = "AI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UAISense> TSense;

	class AMainGameStateBase* MyGameState;
	bool bIsHaveTarget = false;
public:
	//BlackTree Keys
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetActor;
	static const FName IsInFog;
	static const FName IsFaint;

	UFUNCTION()
	void Sense(AActor* Actor, FAIStimulus Stimulus);

	void StopBeTree();
	FVector GetTargetVec();
	AActor* GetCurrentTarget();
	void SetVisibleInFog(bool bisin);
	void SetIsFaint(bool bisin);
};
