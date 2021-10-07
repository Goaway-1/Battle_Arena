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


	virtual void BeginPlay() override;

	virtual void Equip(class AMainPlayer* Player) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange;

	FORCEINLINE float GetAttackRange() { return AttackRange; }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Combat")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	/** Arrow */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	AArrow* Arow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AArrow> ArrowClass;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Arrow")
	float ChargeAmount;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginCharge();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopCharge();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndCharge();

	UFUNCTION()
	void Fire();	
	
	UFUNCTION()
	void Reload();
};
