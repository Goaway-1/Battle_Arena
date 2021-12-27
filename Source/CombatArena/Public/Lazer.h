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
	
	/** Overlap & Dealing */
	UPROPERTY(VisibleAnywhere, Category = "HitInfo", Meta = (AllowPrivateAccess = true))
	TArray<class AMainPlayer*> OverlapingEnemies;

	UPROPERTY(VisibleAnywhere, Category = "HitInfo", Meta = (AllowPrivateAccess = true))
	bool bContinueDealing;

	UPROPERTY(VisibleAnywhere, Category = "HitInfo", Meta = (AllowPrivateAccess = true))
	FLatentActionInfo LatentInfo;

	UPROPERTY(EditAnywhere, Category = "HitInfo", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> LazerDamageType;

	UPROPERTY(EditAnywhere, Category = "HitInfo", Meta = (AllowPrivateAccess = true))
	AController* SpawnController;

	UPROPERTY(VisibleAnywhere, Category = "HitInfo", Meta = (AllowPrivateAccess = true))
	int HitCnt;

public:
	UFUNCTION()
	void OverlapBeginActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OverlapEndActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void Dealing();

	FORCEINLINE void SetCnt(int cnt) { HitCnt = cnt; }
};
