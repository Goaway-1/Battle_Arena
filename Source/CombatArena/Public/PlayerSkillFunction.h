#pragma once

#include "EngineMinimal.h"
#include "SkillFunction.h"
#include "PlayerSkillFunction.generated.h"

/**
 * 
 */
UCLASS()
class COMBATARENA_API UPlayerSkillFunction : public USkillFunction
{
	GENERATED_BODY()
public:
	UPlayerSkillFunction();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Decal", Meta = (AllowPrivateAccess = true))
	class UDecalComponent* PSkillDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Decal", Meta = (AllowPrivateAccess = true))
	class UMaterialInterface* DecalMaterial;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetInitial(APawn* P, USkeletalMeshComponent* S, AController* C, AActor* A) override;

	virtual void LazerAttack() override;

	virtual void LazerEnd() override;

	virtual void GroundAttack() override;

	virtual void SetSkillLocation() override;

	virtual void ConfirmTargetAndContinue() override;
};
