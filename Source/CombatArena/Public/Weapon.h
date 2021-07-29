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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeltalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	void Equip(class AMainPlayer* Player);

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
