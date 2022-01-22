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
	UPROPERTY(EditAnywhere, Category = "Weapon | Bow", Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Bow", Meta = (AllowPrivateAccess = true))
	class AArrow* Arrow;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Bow", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AArrow> ArrowClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Bow | Sound", Meta = (AllowPrivateAccess = true))
	class USoundWave* DrowSound;

	AActor* BowOwner;
	AController* BowController;
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Arrow")
	float ChargeAmount;

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
