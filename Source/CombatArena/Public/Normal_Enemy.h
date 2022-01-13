#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Normal_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class COMBATARENA_API ANormal_Enemy : public AEnemy
{
	GENERATED_BODY()
public:
	ANormal_Enemy(); 
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack(FString type) override;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
