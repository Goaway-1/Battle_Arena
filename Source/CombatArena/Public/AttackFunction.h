#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "MainPlayer.h"
#include "Enemy.h"
#include "Components/ActorComponent.h"
#include "AttackFunction.h"
#include "Engine/SkeletalMeshSocket.h"
#include "AttackFunction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	class AActor* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	class UAnimInstance* OwnerAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	class ACharacter* Hited = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	class AController* Controller;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	class USkeletalMeshComponent* Mesh;

public:
	UFUNCTION()
	void SetOwner(USkeletalMeshComponent* TakeMesh, AController* TakeController);

	UFUNCTION(BlueprintCallable)
	virtual void AttackStart(FVector Location, FVector Forward, TSubclassOf<UDamageType> DamageType, FString Type, UParticleSystem* HitParticle,float AttackRange, float Damage);

	/** DamageText */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FVector2D DamageTextVec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	class UDamageTextWidget* DamageWidget;

	UFUNCTION()
	void SpawnDamageText(FVector WorldLocation, float Damage, TSubclassOf<UDamageTextWidget> DamageTextWidget, AController* DisplayController);
};
