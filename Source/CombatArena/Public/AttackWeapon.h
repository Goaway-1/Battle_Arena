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

#pragma	region ATTACK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange;

	FORCEINLINE float GetAttackRange() { return AttackRange; }

	/** Weapon Name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = true))
	EWeaponName WeaponName;

	FORCEINLINE EWeaponName GetWeaponName() { return WeaponName; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UCapsuleComponent* AttackCollision;

	UFUNCTION()
	void OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void SetAttackCollision(bool value);
#pragma endregion
#pragma region DAMAGE
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Combat")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;
#pragma endregion
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class AController* AtController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class AActor* AtOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> AtDamageType;

	UFUNCTION()
	void SetAttackInit(AController* CauserController, AActor* Causer, TSubclassOf<UDamageType> Type);
};
