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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MagicBall", Meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MagicBall", Meta = (AllowPrivateAccess = true))
	class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MagicBall", Meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent* Flying_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MagicBall | Particle", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* Moving_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MagicBall | Particle", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* Impact_Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MagicBall | Pos", Meta = (AllowPrivateAccess = true))
	FRotator Rotate_Impact_Particle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MagicBall | Pos", Meta = (AllowPrivateAccess = true))
	FVector Location_Offset_Impact_Particle;

	UPROPERTY(EditAnywhere, Category = "MagicBall | HitInfo", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> MagicDamageType;

	UPROPERTY(VisibleAnywhere, Category = "MagicBall | HitInfo", Meta = (AllowPrivateAccess = true))
	int HitCnt;
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	FORCEINLINE void SetCnt(int cnt) { HitCnt = cnt;}

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
