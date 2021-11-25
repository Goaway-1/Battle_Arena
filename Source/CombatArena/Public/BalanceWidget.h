#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BalanceWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class COMBATARENA_API UBalanceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetEnemyOwner(AEnemy* OtherActor);

	UFUNCTION()
	void SetPlayerOwner(AController* OtherActor);

	UFUNCTION()
	void SetOwnerBalance(float Ratio, float Max, float Current);
protected:
	UPROPERTY()
	TWeakObjectPtr<AEnemy> Enemy;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BalanceBar;
};
