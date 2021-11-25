#include "BalanceWidget.h"
#include "Enemy.h"
#include "MainPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBalanceWidget::SetOwnerBalance(float Ratio, float Max, float Current) {
	if (!Enemy.IsValid() && !PlayerController.IsValid()) return;

	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);

	if (Enemy.IsValid()) BalanceBar->SetPercent(Ratio);
	else if (PlayerController.IsValid()) BalanceBar->SetPercent(Ratio);
}

void UBalanceWidget::SetEnemyOwner(AEnemy* OtherActor) {
	Enemy = OtherActor;
}
void UBalanceWidget::SetPlayerOwner(AController* OtherActor) {
	PlayerController = Cast<APlayerController>(OtherActor);
}
