// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Weapon.h"
#include "ShieldWeapon.generated.h"

/**
 * ShieldWeapon class
 */
UCLASS()
class COMBATARENA_API AShieldWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	AShieldWeapon();

	virtual void BeginPlay() override;

	virtual void Equip(class AMainPlayer* Player) override;

#pragma region SHIELDCOMPONENT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical")
	class UBoxComponent* ShieldBox;
	
	UFUNCTION()
	void OnShieldOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnShieldOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma endregion

};
