#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "MainPlayer.h"
#include "Enemy.h"
#include "Components/ActorComponent.h"
#include "AttackFunction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATARENA_API UAttackFunction : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttackFunction();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
