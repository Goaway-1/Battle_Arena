#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

/**
 * HealthBar
 */
UCLASS(Abstract)		//√ﬂªÛ»≠ ∞≥≥‰
class COMBATARENA_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetEnemyOwner(AEnemy* OtherActor);
	
	UFUNCTION()
	void SetPlayerOwner(AController* OtherActor);

	UFUNCTION()
	void SetOwnerHealth(float Ratio, float Max, float Current);
protected:

	UPROPERTY()
	TWeakObjectPtr<AEnemy> Enemy;	
	
	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentHealthLabel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHealthLabel;
};
