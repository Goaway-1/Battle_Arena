// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "EngineMinimal.h"
#include "AttackFunction.h"
#include "Engine/SkeletalMeshSocket.h"
#include "PlayerAttackFunction.generated.h"

/**
 * 
 */
UCLASS()
class COMBATARENA_API UPlayerAttackFunction : public UAttackFunction
{
	GENERATED_BODY()

public:
	UPlayerAttackFunction();

	virtual void BeginPlay() override;

#pragma region KICK

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack | Kick")
	float KickRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack | Kick")
	float KickRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | Kick")
	bool bKicking;			//현재 Kick 중인지

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | Kick")
	bool bCanKick;			//Kick이 가능한지

	UFUNCTION()
	void Kick(UAnimInstance* Anim, UAnimMontage* Montage);

	UFUNCTION()
	void KickStart(FVector Location,FVector Forward);

	UFUNCTION(BlueprintCallable)
	void KickEnd();

#pragma endregion
};
