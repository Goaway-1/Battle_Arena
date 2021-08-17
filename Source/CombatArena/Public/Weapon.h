#pragma once

#include "EngineMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponPos : uint8 {
	EWP_Empty		UMETA(DIsplayName = "Empty"),
	EWP_Left		UMETA(DisplayName = "Left"),
	EWP_Right		UMETA(DisplayName = "Right"),
	EWP_Full		UMETA(DisplayName = "Full"),

	EWP_Default		UMETA(DisplayName = "Default")
};

UCLASS()
class COMBATARENA_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeltalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	UFUNCTION()
	void Equip(class AMainPlayer* Player);

	UFUNCTION()
	void UnEquip();

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	//부착될 Pos 지정 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pos")
	EWeaponPos WeaponPos;

	FORCEINLINE void SetWeaponPosLoc(EWeaponPos Pos) { WeaponPos = Pos; }

	FORCEINLINE EWeaponPos GetWeaponPos() { return WeaponPos; }

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AController* WeaponInstigator;		//메인을 받아와야 함

	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }
#pragma endregion
};
