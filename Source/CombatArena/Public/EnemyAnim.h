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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class AEnemy* Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed;
};
