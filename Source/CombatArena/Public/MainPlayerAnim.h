#pragma once

#include "EngineMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerAnim.generated.h"

UCLASS()
class COMBATARENA_API UMainPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;	//������ ����

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", Meta = (AllowPrivateAccess = true))
	class AMainPlayer* MainPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float DirectionSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	bool bTargetOn;

	/** Rotation Character View */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float ViewYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float ViewPitch;

	/** Rotation Character View */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = true))
	int TurnAxis = 0;	// Left(-1)/Idle(0)/Right(1)
};
