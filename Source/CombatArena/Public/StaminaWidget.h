#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaWidget.generated.h"

/**
 * Stamina
 */
UCLASS(Abstract)
class COMBATARENA_API UStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetOwner(AController* OtherActor);

	UFUNCTION()
	void SetOwnerStamina(float Ratio, float Max, float Current);

protected:
	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentStaminaLabel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxStaminaLabel;
};
