#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class COMBATARENA_API UDamageTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
private:
	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	class UTextBlock* DamageText;

	UPROPERTY(Meta = (BindWidgetAnim), Meta = (AllowPrivateAccess = true), Transient)	//Á÷·ÄÈ­
	class UWidgetAnimation* Fade;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	FVector2D InintialScreenLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	FVector2D FinalScreenLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float DamageToDisplay;
};
