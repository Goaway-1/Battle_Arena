#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextWidget.generated.h"

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

	UPROPERTY(EditAnywhere, Category = "DamageText")
	FVector2D InintialScreenLocation;
	
	UPROPERTY(EditAnywhere, Category = "DamageText")
	FVector2D FinalScreenLocation;

	UPROPERTY(EditAnywhere, Category = "DamageText")
	float DamageToDisplay;
public:
	void SetInitialSetting (FVector2D &vec, float &name);
};
