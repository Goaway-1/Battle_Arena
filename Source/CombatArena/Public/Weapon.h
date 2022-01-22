#pragma once

#include "EngineMinimal.h"
#include "Item.h"
#include "MainPlayer.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponPos : uint8 {
	EWP_Empty		UMETA(DIsplayName = "Empty"),
	EWP_Shield		UMETA(DisplayName = "Shield"),
	EWP_Melee		UMETA(DisplayName = "Melee"),
	EWP_Bow			UMETA(DisplayName = "Bow"),
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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", Meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", Meta = (AllowPrivateAccess = true))
	EWeaponPos WeaponPos;
public:
	virtual void Equip(class AMainPlayer* Player);

	void UnEquip();

	FORCEINLINE void SetWeaponPosLoc(EWeaponPos Pos) { WeaponPos = Pos; }
	FORCEINLINE EWeaponPos GetWeaponPos() { return WeaponPos; }
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() { return SkeletalMesh; }
};
