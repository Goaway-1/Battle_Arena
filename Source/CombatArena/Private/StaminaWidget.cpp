#include "StaminaWidget.h"
#include "MainPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UStaminaWidget::SetOwnerStamina(float Ratio, float Max, float Current) {
	if (!PlayerController.IsValid()) return;

	FLinearColor BarColor;
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);

	/** Set StaminaBar Color */
	if (Ratio <= 0.3f) BarColor = FLinearColor::Green;
	else BarColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.25f);

	StaminaBar->SetFillColorAndOpacity(BarColor);
	StaminaBar->SetPercent(Ratio);
	CurrentStaminaLabel->SetText(FText::AsNumber(Current, &Opts));
	MaxStaminaLabel->SetText(FText::AsNumber(Max, &Opts));
}

void UStaminaWidget::SetOwner(AController* OtherActor) {
	PlayerController = Cast<APlayerController>(OtherActor);
}