// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "SkillFunction.h"
#include "PlayerSkillFunction.generated.h"

/**
 * 
 */
UCLASS()
class COMBATARENA_API UPlayerSkillFunction : public USkillFunction
{
	GENERATED_BODY()
public:
	UPlayerSkillFunction();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Ground */
	virtual void GroundAttack() override;

	virtual void SetSkillLocation() override;

	virtual void ConfirmTargetAndContinue() override;
};
