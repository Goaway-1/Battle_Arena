#pragma once

#include "EngineMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerAnim.generated.h"

UCLASS()
class COMBATARENA_API UMainPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;	//생성시 동작

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	class AMainPlayer* MainPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float DirectionSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bTargetOn;
};
