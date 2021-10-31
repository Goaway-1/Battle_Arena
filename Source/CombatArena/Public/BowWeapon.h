#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "BowWeapon.generated.h"

 UENUM(BlueprintType)
enum class EBowStatus : uint8 {
	EBS_Normal		UMETA(DisplayName = "Normal"),
	EBS_Drawing		UMETA(DisplayName = "Drawing"),

	EBS_Default		UMETA(DisplayName = "Default")
};
UCLASS()
class COMBATARENA_API ABowWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	ABowWeapon();

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

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
	class AArrow* Arrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AArrow> ArrowClass;

	/** Charge & Shoot */
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
