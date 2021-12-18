#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/DecalComponent.h"
#include "Enemy.h"
#include "SkillFunction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATARENA_API USkillFunction : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillFunction();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Initial */
	APawn* OwnerInstigator;
	USkeletalMeshComponent* OwnerSkeletal;
	AController* OwnerController;
	AActor* OwnerActor;

	/** Lazer */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Lazer")
	AActor* Lazer;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | Lazer")
	TSubclassOf<class AActor> LazerClass;

	/** Ground */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	class UMaterialInterface* DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Decal")
	class UDecalComponent* SkillDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Ground")
	bool bGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Ground")
	FVector out;

	/** 초기 설정 */
	UFUNCTION()
	void SetInitial(APawn* P, USkeletalMeshComponent* S, AController* C, AActor* A);

	/** Lazer */
	UFUNCTION()
	virtual void LazerAttack();

	UFUNCTION()
	virtual void LazerEnd();

	/** Ground */
	UFUNCTION()
	virtual void GroundAttack();

	UFUNCTION()
	virtual void SetSkillLocation();

	UFUNCTION()
	virtual void ConfirmTargetAndContinue();
};
