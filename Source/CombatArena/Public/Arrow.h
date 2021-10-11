#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UENUM(BlueprintType)
enum class EArrowStatus : uint8 {
	EAS_Normal		UMETA(DisplayName = "Normal"),
	EAS_Unobtained	UMETA(DisplayName = "Unobtained"),
	EAS_InBow		UMETA(DisplayName = "InBow"),
	EAS_InArrow		UMETA(DisplayName = "InArrow")
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

	UFUNCTION()
	void Fire(float Amount);

	UFUNCTION()
	void OnStateBegine();

	UFUNCTION()
	void SetArrowStatus(EArrowStatus Status);

	FORCEINLINE EArrowStatus GetArrowStatus() { return ArrowStatus; }

	/** For Destructible Mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physic")
	class URadialForceComponent* RadiaForce;
};
