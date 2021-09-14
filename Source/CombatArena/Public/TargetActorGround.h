#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetActorGround.generated.h"

UCLASS()
class COMBATARENA_API ATargetActorGround : public AActor
{
	GENERATED_BODY()
	
public:	
	ATargetActorGround();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Targeting")
	APlayerController* Controller;

	UFUNCTION()
	void StartTargeting(APlayerController* Con);

	UFUNCTION()
	void ConfrimTargetingAndContinue();

	UFUNCTION()
	bool GetPlayerLookingPoint(OUT FVector& OutViewPoint);

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Targeting")
	float Radius;
};
