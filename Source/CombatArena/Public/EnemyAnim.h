#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class COMBATARENA_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	class AEnemy* Enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float Speed;
public:
	FOnAttackEndDelegate OnAttackEnd;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
