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
	void SetOwner(ACharacter* OtherActor,bool IsPlayer);

	UFUNCTION()
	void SetOwnerHealth();
protected:

	UPROPERTY()
	TWeakObjectPtr<ACharacter> Owner;

	class AEnemy* Enemy;
	class AMainPlayer* Player;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentHealthLabel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHealthLabel;
};
