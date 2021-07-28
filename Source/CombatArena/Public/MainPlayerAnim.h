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

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();					//틱마다 동작

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	class AMainPlayer* MainPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float DirectionSpeed;

};
