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

private:
	UPROPERTY(EditAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRange;

	/** Arrow */
	UPROPERTY(EditDefaultsOnly, Category = "Arrow", Meta = (AllowPrivateAccess = true))
	class AArrow* Arrow;

	UPROPERTY(EditDefaultsOnly, Category = "Arrow", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AArrow> ArrowClass;

	/** Charge & Shoot */
	UPROPERTY(EditAnyWhere, Category = "Arrow", Meta = (AllowPrivateAccess = true))
	float ChargeAmount;

	AActor* BowOwner;
	AController* BowController;

	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (AllowPrivateAccess = true))
	class USoundWave* DrowSound;

public:
	virtual void Equip(class AMainPlayer* Player) override;

	FORCEINLINE float GetAttackRange() { return AttackRange; }

	FORCEINLINE float GetChargeAmount() { return ChargeAmount; }

	UFUNCTION(BlueprintCallable)
	void SetChargeAmount(float value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginCharge();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndCharge();

	UFUNCTION()
	void Fire(int Cnt);
	
	UFUNCTION()
	void Reload();

	UFUNCTION()
	void InitalBow(AActor* BOwner, AController* BController);
};
