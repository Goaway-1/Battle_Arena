#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UENUM(BlueprintType)
enum class EArrowStatus : uint8 {
	EAS_InBow		UMETA(DisplayName = "InBow"),
	EAS_InArrow		UMETA(DisplayName = "InArrow"),
	EAS_Destroy		UMETA(DisplayName = "Destroy")
};

UCLASS()
class COMBATARENA_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	AArrow();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* ArrowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arrow | Physic", Meta = (AllowPrivateAccess = true))
	class URadialForceComponent* RadiaForce;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arrow | Physic", Meta = (AllowPrivateAccess = true))
	USphereComponent* ArrowCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow | Damage", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Arrow | Damage", Meta = (AllowPrivateAccess = true))
	float Damage = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Arrow | Sound", Meta = (AllowPrivateAccess = true))
	class USoundWave* ShotSound;

	UPROPERTY(VisibleAnywhere, Category = "Arrow")
	bool bisFire = false;

	UPROPERTY(VisibleAnywhere, Category = "Arrow")
	float FirePower;

	UPROPERTY(VisibleAnywhere, Category = "Arrow")
	EArrowStatus ArrowStatus;

	UPROPERTY(VisibleAnywhere, Category = "Arrow")
	class AActor* ArrowOwner;

	UPROPERTY(VisibleAnywhere, Category = "Arrow")
	class AController* ArrowController;

	UPROPERTY(VisibleAnywhere, Category = "Arrow | Damage")
	int AttackCnt;
public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void InitalArrow(AActor* AOwner, AController* AController);
	void Fire(float Amount,int Cnt);
	void SetBowState();
	void SetArrowStatus(EArrowStatus Status);

	FORCEINLINE EArrowStatus GetArrowStatus() { return ArrowStatus; }
};
