#pragma once

#include "EngineMinimal.h"
#include "Weapon.h"
#include "ShieldWeapon.generated.h"

/**
 * ShieldWeapon class
 */
UCLASS()
class COMBATARENA_API AShieldWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	AShieldWeapon();

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon | Angle")
	float ShiledMinAngle;

	UPROPERTY(VisibleAnywhere, Category = "Weapon | Angle")
	float ShiledMaxAngle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Paticle", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* HitedParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Sound", Meta = (AllowPrivateAccess = true))
	class USoundWave* HitedSound;
public:
	virtual void Equip(class AMainPlayer* Player) override;

	FORCEINLINE float& GetMinAngle() { return ShiledMinAngle; }
	FORCEINLINE float& GetMaxAngle() { return ShiledMaxAngle; }
	FORCEINLINE USoundWave* GetHitedSound() {return HitedSound; }
	FORCEINLINE UParticleSystem* GetHitedParticle() { return HitedParticle; }
};
