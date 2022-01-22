#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "SK_Meteor.generated.h"

UCLASS()
class COMBATARENA_API ASK_Meteor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASK_Meteor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite , meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meteor", Meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent* Flying_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meteor | Particle", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* Moving_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meteor | Particle", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* Impact_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MagicBall | Pos", Meta = (AllowPrivateAccess = true))
	FRotator Rotate_Impact_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MagicBall | Pos", Meta = (AllowPrivateAccess = true))
	FVector Location_Offset_Impact_Particle;

	UPROPERTY(VisibleAnywhere)
	class UEnemySkillFunction* SkillFunction;

	UPROPERTY(VisibleAnywhere)
	bool bIsEnd = false;

public:
	/** Set Initial of Meteor */
	UFUNCTION()
	void SetInitial(UEnemySkillFunction* Function);

	/** Is Actor Detected a Ground */
	UFUNCTION()
	void IsInGround();
};