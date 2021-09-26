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

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite , meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;

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