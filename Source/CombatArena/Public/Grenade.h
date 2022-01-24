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

private:
	UPROPERTY(EditAnywhere, Category = "Grenade | Mesh", Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Grenade | Mesh", Meta = (AllowPrivateAccess = true))
	class USphereComponent* Collision;

	UPROPERTY(EditAnywhere, Category = "Grenade | Mesh", Meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditAnywhere, Category = "Grenade | Smoke", Meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent* Smoke;

	/** Time */
	FTimerHandle SpawnSmokeHandle;
	float SpawnSmokeTime;
	float SmokeTime = 6.0f;		//Smoke가 다 필때까지의 시간.

	bool isGrowing = false;
public:
	void SpawnSmoke();
	FORCEINLINE void GrowingSmoke() { isGrowing = true; }
	void DestorySmoke();
	void SetFire(FRotator Rot);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};