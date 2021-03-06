#pragma once

#include "EngineMinimal.h"
#include "SkillFunction.h"
#include "EnemySkillFunction.generated.h"
 
DECLARE_DELEGATE(FSkillEnd)   //Meteor

UCLASS()
class COMBATARENA_API UEnemySkillFunction : public USkillFunction
{
	GENERATED_BODY()

public:
	UEnemySkillFunction();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Skill | Decal")
	class UDecalComponent* ESkillDecal;

	UPROPERTY(EditAnywhere, Category = "Skill | Decal", Meta = (AllowPrivateAccess = true))
	class UMaterialInterface* DecalMaterial;

	UPROPERTY(EditAnywhere, Category = "Skill | Meteor", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> MeteorDamageType;

	UPROPERTY(VisibleAnywhere, Category = "Skill | Meteor")
	class AMeteor* Meteor;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | Meteor")
	TSubclassOf<class AMeteor> MeteorClass;

	UPROPERTY(VisibleAnywhere, Category = "Skill | Lazer")
	FVector LazerLoc;

	UPROPERTY(VisibleAnywhere, Category = "Skill | Lazer")
	FRotator LazerRot;

	UPROPERTY(EditAnywhere, Category = "Skill | Lazer", Meta = (AllowPrivateAccess = true))
	int LazerCnt = 12;

	UPROPERTY(VisibleAnywhere, Category = "Skill | Magic")
	class AMagicBall* Magic;

	UPROPERTY(EditAnywhere, Category = "Skill | Magic", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AActor> MagicClass;

	UPROPERTY(VisibleAnywhere, Category = "Skill | Info", Meta = (AllowPrivateAccess = true))
	int HitCnt;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | Sound", Meta = (AllowPrivateAccess = true))
	class USoundBase* LazerSound;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | Sound", Meta = (AllowPrivateAccess = true))
	TArray<class USoundBase*> MeteorSound;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetInitial(APawn* P, USkeletalMeshComponent* S, AController* C, AActor* A) override;

	virtual void LazerAttack() override;

	virtual void LazerEnd() override;

	virtual void GroundAttack() override;

	virtual void SetSkillLocation() override;

	virtual void ConfirmTargetAndContinue() override;

	UFUNCTION()
	void SpawnMeteor();

	FSkillEnd SkillDelegate;

	UFUNCTION()
	void MagicAttack();

	UFUNCTION()
	void MagicEnd();

	UFUNCTION()
	void RandPos(FVector& Loc, FRotator& Rot);

	UFUNCTION()
	void SetHitCnt();
};
