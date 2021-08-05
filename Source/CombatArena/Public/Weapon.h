#pragma once

#include "EngineMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UCLASS()
class COMBATARENA_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeltalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	void Equip(class AMainPlayer* Player);

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

#pragma region ATTACK
	//공격 피직스담당
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Attack")
	class UBoxComponent* AttackBox;

	UFUNCTION()
	void OnAttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()	//WeaponBox의 콜리전을 키고 끄는 기능 (Main에게 호출 되어 사용)
	void ActiveOnCollision();

	UFUNCTION()	//WeaponBox의 콜리전을 키고 끄는 기능 (Main에게 호출 되어 사용)
	void DeActiveOnCollision();
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
