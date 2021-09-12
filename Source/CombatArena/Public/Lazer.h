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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneRoot; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* LazerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* LazerArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* LazerEndDetector;
	
	/** Overlap & Dealing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitInfo")
	TArray<class AMainPlayer*> OverlapingEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitInfo")
	bool bContinueDealing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitInfo")
	FLatentActionInfo LatentInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitInfo")
	TSubclassOf<UDamageType> LazerDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitInfo")
	AController* SpawnController;

	UFUNCTION()
	void OverlapBeginActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OverlapEndActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void Dealing();
};
