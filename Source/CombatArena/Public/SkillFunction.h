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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	APawn* OwnerInstigator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	USkeletalMeshComponent* OwnerSkeletal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	AController* OwnerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	AActor* OwnerActor;

	/** Lazer */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Lazer")
	TArray<AActor*> Lazer;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | Lazer")
	TSubclassOf<class AActor> LazerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Ground")
	bool bGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill | Ground")
	FVector out;

	/** 초기 설정 */
	UFUNCTION()
	virtual void SetInitial(APawn* P, USkeletalMeshComponent* S, AController* C, AActor* A);

	/** Lazer */
	UFUNCTION()
	virtual void LazerAttack();

	UFUNCTION()
	virtual void LazerEnd();

	UFUNCTION()
	virtual void GroundAttack();

	UFUNCTION()
	virtual void SetSkillLocation();

	UFUNCTION()
	virtual void ConfirmTargetAndContinue();
};
