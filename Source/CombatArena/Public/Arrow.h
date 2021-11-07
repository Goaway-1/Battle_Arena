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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* ArrowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow")
	EArrowStatus ArrowStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow")
	bool bisFire = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow")
	float FirePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow | Damage")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow | Damage")
	class AActor* ArrowOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow | Damage")
	class AController* ArrowController;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Arrow | Damage")
	float Damage;

	UFUNCTION()
	void InitalArrow(AActor* AOwner, AController* AController);

	UFUNCTION()
	void Fire(float Amount);

	UFUNCTION()
	void OnStateBegin();

	UFUNCTION()
	void SetArrowStatus(EArrowStatus Status);

	FORCEINLINE EArrowStatus GetArrowStatus() { return ArrowStatus; }

	/** For Destructible Mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physic")
	class URadialForceComponent* RadiaForce;

	/** Collsion */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physic")
	USphereComponent* ArrowCollision;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
