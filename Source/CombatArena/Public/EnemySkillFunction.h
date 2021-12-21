#pragma once

#include "EngineMinimal.h"
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

private:
	UPROPERTY(VisibleAnywhere,  Category = "Skill | Decal")
	class UDecalComponent* ESkillDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Decal", Meta = (AllowPrivateAccess = true))
	class UMaterialInterface* DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Meteor", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> MeteorDamageType;

	UPROPERTY(VisibleAnywhere, Category = "Skill | Meteor")
	class ASK_Meteor* Meteor;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | Meteor")
	TSubclassOf<class ASK_Meteor> MeteorClass;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetInitial(APawn* P, USkeletalMeshComponent* S, AController* C, AActor* A) override;

	virtual void LazerAttack() override;

	virtual void LazerEnd() override;

	virtual void GroundAttack() override;

	virtual void SetSkillLocation() override;

	virtual void ConfirmTargetAndContinue() override;

	/** Not Override */
	UFUNCTION()
	void SpawnMeteor();

	FSkillEnd SkillDelegate;

	//@@@@

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Magic")
	AActor* Magic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Magic")
	TSubclassOf<class AActor> MagicClass;

	UFUNCTION()
	void MagicAttack();

	UFUNCTION()
	void MagicEnd();
};
