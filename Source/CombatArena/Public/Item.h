#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8 {
	EIT_Normal		UMETA(DisplayName = "Normal"),
	EIT_Weapon		UMETA(DisplayName = "Weapon"),
	EIT_Item		UMETA(DisplayName = "Item"),

	EIT_Default		UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Normal	UMETA(DisplayName = "Normal"),
	EIS_Ground  UMETA(DisplayName = "Ground"),
	EIS_Equip	UMETA(DisplayName = "Equip"),

	EIS_Default	UMETA(DisplayName = "Default")
};

UCLASS()
class COMBATARENA_API AItem : public AActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	AItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", Meta = (AllowPrivateAccess = true))
	EItemType ItemType;

	/** ItemState */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", Meta = (AllowPrivateAccess = true))
	EItemState ItemState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", Meta = (AllowPrivateAccess = true))
	float SpinSpeed;

	FORCEINLINE void SetItemState(EItemState State) { ItemState = State; }
	FORCEINLINE EItemState GetItemState() { return ItemState; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
	class USphereComponent* CollisionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
	class UParticleSystemComponent* IdleParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
	class UParticleSystem* OverlapParticle;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SetSpinItem();
public:
	UFUNCTION()
	FORCEINLINE EItemType GetItemType() { return ItemType; }
};
