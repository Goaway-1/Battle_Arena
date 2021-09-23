#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* SkillAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Decal")
	class UDecalComponent* SkillDecal;

	/** 초기 설정 */
	UFUNCTION()
	void SetInitial(APawn* P, USkeletalMeshComponent* S, AController* C, AActor* A);

	APawn* OwnerInstigator;
	USkeletalMeshComponent* OwnerSkeletal;
	AController* OwnerController;
	AActor* OwnerActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	class UMaterialInterface* DecalMaterial;

	//Lazer
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Lazer")
	AActor* Lazer;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | Lazer")
	TSubclassOf<class AActor> LazerClass;

	UFUNCTION()
	void LazerAttack();

	UFUNCTION()
	void LazerEnd();

	/** Targeting On Ground */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Ground")
	bool bGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Ground")
	FVector out;

	UFUNCTION()
	void GroundAttack();

	UFUNCTION()
	void SetSkillLocation();

	UFUNCTION()
	void ConfirmTargetAndContinue();
};
