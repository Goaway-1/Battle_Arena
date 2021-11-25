#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "AttackWeapon.generated.h"

/**
 * MeleeWeapon
 */
UENUM(BlueprintType)
enum class EWeaponName : uint8 {
	EWN_Normal		UMETA(DisplayName = "Normal"),
	EWN_Sword		UMETA(DisplayName = "Sword"),
	EWN_Mace		UMETA(DisplayName = "Mace"),
	EWN_Spear		UMETA(DisplayName = "Spear")
};

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
	/** Weapon Name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = true))
	EWeaponName WeaponName;

	FORCEINLINE EWeaponName GetWeaponName() { return WeaponName; }
};
