// Fill out your copyright notice in the Description page of Project Settings.

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

	virtual void Equip(class AMainPlayer* Player) override;

#pragma region SHIELDRANGE
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Angle")
	float ShiledMinAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Angle")
	float ShiledMaxAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paticle")
	class UParticleSystem* HitedParticle;
#pragma endregion


};
