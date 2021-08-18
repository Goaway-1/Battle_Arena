// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "AttackWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COMBATARENA_API AAttackWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	AAttackWeapon();

	virtual void Equip(class AMainPlayer* Player) override;

#pragma	region ATTACK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange;

	FORCEINLINE float GetAttackRange() { return AttackRange; }

#pragma endregion
#pragma region DAMAGE
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Combat")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;
#pragma endregion
};
