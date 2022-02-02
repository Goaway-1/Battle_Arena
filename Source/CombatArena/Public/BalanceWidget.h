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

protected:
	TWeakObjectPtr<class AEnemy> Enemy;

	TWeakObjectPtr<class APlayerController> PlayerController;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BalanceBar;
public:
	void SetEnemyOwner(AEnemy* OtherActor);
	void SetPlayerOwner(AController* OtherActor);
	void SetOwnerBalance(float Ratio, float Max, float Current);
};
