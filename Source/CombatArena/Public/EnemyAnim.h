#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"


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
};
