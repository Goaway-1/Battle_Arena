#pragma once

#include "EngineMinimal.h"
#include "AttackFunction.h"
#include "Engine/SkeletalMeshSocket.h"
#include "PlayerAttackFunction.generated.h"

UCLASS()
class COMBATARENA_API UPlayerAttackFunction : public UAttackFunction
{
	GENERATED_BODY()

public:
	UPlayerAttackFunction();

	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "AttackFunction | Kick", Meta = (AllowPrivateAccess = true))
	float KickRange;

	UPROPERTY(EditAnywhere, Category = "AttackFunction | Kick", Meta = (AllowPrivateAccess = true))
	float KickRadius;

	UPROPERTY(VisibleAnywhere, Category = "AttackFunction | Kick")
	bool bKicking;

	UPROPERTY(VisibleAnywhere, Category = "AttackFunction | Kick")
	bool bCanKick;			
public:
	void Kick(UAnimInstance* Anim, UAnimMontage* Montage);
	void KickStart(FVector Location,FVector Forward);

	UFUNCTION(BlueprintCallable)
	void KickEnd();

	FORCEINLINE bool GetKicking() { return bKicking; }
};
