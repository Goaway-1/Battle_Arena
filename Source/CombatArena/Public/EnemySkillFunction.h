#pragma once

#include "CoreMinimal.h"
#include "SkillFunction.h"
#include "EnemySkillFunction.generated.h"
 
DECLARE_DELEGATE(FSkillEnd)   //SK_Meteor의 바닥 충돌 여부를 위함.

UCLASS()
class COMBATARENA_API UEnemySkillFunction : public USkillFunction
{
	GENERATED_BODY()
public:
	UEnemySkillFunction();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Meteor", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> MeteorDamageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Meteor", meta = (AllowPrivateAccess = "true"))
	class ASK_Meteor* Meteor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill | Meteor", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASK_Meteor> MeteorClass;
public:
	virtual void GroundAttack() override;

	virtual void SetSkillLocation() override;

	virtual void ConfirmTargetAndContinue() override;

	virtual void LazerAttack() override;

	virtual void LazerEnd() override;

	/** Not Override */
	UFUNCTION()
	void SpawnMeteor();

	FSkillEnd SkillDelegate;
};
