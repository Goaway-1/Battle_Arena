#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteor.generated.h"

UCLASS()
class COMBATARENA_API AMeteor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeteor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meteor", Meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent* Flying_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meteor | Particle", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* Moving_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meteor | Particle", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* Impact_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meteor | Pos", Meta = (AllowPrivateAccess = true))
	FRotator Rotate_Impact_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meteor | Pos", Meta = (AllowPrivateAccess = true))
	FVector Location_Offset_Impact_Particle;

	UPROPERTY(EditAnywhere, Category = "Meteor | Damage", Meta = (AllowPrivateAccess = true))
	float Damage = 12.0f;

	class UEnemySkillFunction* SkillFunction;

	bool bIsEnd = false;

public:
	void SetInitial(UEnemySkillFunction* Function);

	void IsInGround();

	FORCEINLINE float GetDamage() { return Damage; }
};
