#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class COMBATARENA_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrenade();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class USphereComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UProjectileMovementComponent* Projectile;

	/** Time */
	UPROPERTY(VisibleAnywhere, Category = "Smoke")
	FTimerHandle SpawnSmokeHandle;

	UPROPERTY(VisibleAnywhere, Category = "Smoke")
	float SpawnSmokeTime;

	/** Smoke Growing Check*/
	UPROPERTY(VisibleAnywhere, Category = "Smoke")
	bool isGrowing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smoke")
	class UParticleSystem* SmokeParticle;

public:
	UFUNCTION()
	void SpawnSmoke();

	UFUNCTION()
	void GrowingSmoke();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};