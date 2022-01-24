#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Lazer.generated.h"

UCLASS()
class COMBATARENA_API ALazer : public AActor
{
	GENERATED_BODY()
	
public:	
	ALazer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	USceneComponent* DefaultSceneRoot; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* LazerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	USpringArmComponent* LazerArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Meta = (AllowPrivateAccess = true))
	USphereComponent* LazerEndDetector;

	UPROPERTY(EditAnywhere, Category = "Lazer | Damage", Meta = (AllowPrivateAccess = true))
	float Damage = 7.0f;

	UPROPERTY(EditAnywhere, Category = "Lazer | HitInfo", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> LazerDamageType;
	
	/** Overlap & Dealing */
	UPROPERTY(VisibleAnywhere, Category = "Lazer | HitInfo")
	TArray<class AMainPlayer*> OverlapingEnemies;

	FLatentActionInfo LatentInfo;
	AController* HitedController;
	bool bContinueDealing = false;
	int HitCnt = 0;
public:
	UFUNCTION()
	void OverlapBeginActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OverlapEndActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Dealing();
	FORCEINLINE void SetCnt(int cnt) { HitCnt = cnt; }
};
