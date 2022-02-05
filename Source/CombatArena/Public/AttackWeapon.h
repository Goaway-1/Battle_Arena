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

protected:
	virtual void BeginPlay() override;

public:
	AAttackWeapon();

	virtual void Equip(class AMainPlayer* Player) override;

private:
	UPROPERTY(EditAnywhere, Category = "Weapon", Meta = (AllowPrivateAccess = true))
	EWeaponName WeaponName;

	UPROPERTY(EditAnywhere, Category = "Weapon", Meta = (AllowPrivateAccess = true))
	UCapsuleComponent* AttackCollision;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class AController* AtController;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class AActor* AtOwner;

	UPROPERTY(EditAnywhere, Category = "Weapon", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere,Category = "Weapon", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> AtDamageType;

	UPROPERTY(EditAnywhere, Category = "Weapon", Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(EditAnyWhere, Category = "Weapon", Meta = (AllowPrivateAccess = true))
	float Damage;

public:
	FORCEINLINE float GetAttackRange() { return AttackRange; }
	FORCEINLINE float GetDamage() { return Damage; }
	FORCEINLINE EWeaponName GetWeaponName() { return WeaponName; }

	UFUNCTION()
	void OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void SetAttackCollision(bool value);

	void SetAttackInit(AController* CauserController, AActor* Causer, TSubclassOf<UDamageType> Type);
};
