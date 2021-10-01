#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "BowWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COMBATARENA_API ABowWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	ABowWeapon();

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
