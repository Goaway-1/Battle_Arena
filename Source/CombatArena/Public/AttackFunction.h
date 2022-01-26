#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "Components/ActorComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "AttackFunction.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMBATARENA_API UAttackFunction : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttackFunction();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackFunction | Owner")
	class AActor* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackFunction | Owner")
	class UAnimInstance* OwnerAnimInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackFunction | Owner")
	class AController* Controller;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackFunction | Owner")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "AttackFunction | Debug", Meta = (AllowPrivateAccess = true))
	bool bDrawAttack = false;
public:
	UFUNCTION()
	void SetOwner(USkeletalMeshComponent* TakeMesh, AController* TakeController);

	UFUNCTION(BlueprintCallable)
	virtual void SkillAttackStart(FVector Location, FVector Forward, TSubclassOf<UDamageType> DamageType, FString Type, UParticleSystem* HitParticle, float AttackRange, float Damage, int AttackCnt);

	/** DamageText */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FVector2D DamageTextVec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	class UDamageTextWidget* DamageWidget;

	UFUNCTION()
	void SpawnDamageText(FVector WorldLocation, float Damage, TSubclassOf<UDamageTextWidget> DamageTextWidget, AController* DisplayController);
};