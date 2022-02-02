#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicBall.generated.h"

UCLASS()
class COMBATARENA_API AMagicBall : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicBall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "MagicBall", Meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "MagicBall", Meta = (AllowPrivateAccess = true))
	class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly, Category = "MagicBall", Meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent* Flying_Particle;

	UPROPERTY(EditDefaultsOnly, Category = "MagicBall | Particle", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* Moving_Particle;

	UPROPERTY(EditDefaultsOnly, Category = "MagicBall | Particle", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* Impact_Particle;
	
	UPROPERTY(EditDefaultsOnly, Category = "MagicBall | Pos", Meta = (AllowPrivateAccess = true))
	FVector Location_Offset_Impact_Particle;

	FRotator Rotate_Impact_Particle;

	UPROPERTY(EditAnywhere, Category = "MagicBall | HitInfo", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> MagicDamageType;

	UPROPERTY(EditDefaultsOnly, Category = "MagicBall | Sound", Meta = (AllowPrivateAccess = true))
	class USoundBase* MagicSound;

	UPROPERTY(EditAnywhere, Category = "MagicBall | Damage", Meta = (AllowPrivateAccess = true))
	float Damage = 10.0f;

	int HitCnt;
	FTimerHandle handle;
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	FORCEINLINE void SetCnt(int cnt) { HitCnt = cnt;}

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
