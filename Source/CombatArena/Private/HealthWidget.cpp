#include "HealthWidget.h"
#include "Enemy.h"
#include "MainPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthWidget::SetOwnerHealth(float Ratio,float Max,float Current) {
	if (!Enemy.IsValid() && !PlayerController.IsValid()) return;

	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);

	if (Enemy.IsValid()) {
		HealthBar->SetPercent(Ratio);
		CurrentHealthLabel->SetText(FText::AsNumber(Current, &Opts));
		MaxHealthLabel->SetText(FText::AsNumber(Max, &Opts));
	}
	else if (PlayerController.IsValid()) {
		HealthBar->SetPercent(Ratio);
		CurrentHealthLabel->SetText(FText::AsNumber(Current, &Opts));
		MaxHealthLabel->SetText(FText::AsNumber(Max, &Opts));
	}
}

void UHealthWidget::SetEnemyOwner(AEnemy* OtherActor) {
	Enemy = OtherActor;
}
void UHealthWidget::SetPlayerOwner(AController* OtherActor) {
	PlayerController = Cast<APlayerController>(OtherActor);
}