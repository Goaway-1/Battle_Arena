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
#pragma region SKILL
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* SkillAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | Decal")
	class UDecalComponent* SkillDecal;

	//³»°¡ ¸¸µë
	UFUNCTION()
	void SetInitial(APawn* P, USkeletalMeshComponent* S, AController* C, AActor* A);

	APawn* OwnerInstigator;
	USkeletalMeshComponent* OwnerSkeletal;
	AController* OwnerController;
	AActor* OwnerActor;
	//³»°¡ ¸¸µë

	/** Skill Test*/
	UFUNCTION()
	void SkillBegin();

	UFUNCTION()
	void SkillEnd();

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
	void GroundAttack(FRotator Rot);

	UFUNCTION()
	void SetSkillLocation(FVector& OutViewPoint);

	UFUNCTION()
	void ConfirmTargetAndContinue();
#pragma endregion
		
};
