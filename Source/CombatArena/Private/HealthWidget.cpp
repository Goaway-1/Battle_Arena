#include "HealthWidget.h"
#include "Enemy.h"
#include "MainPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthWidget::SetOwnerHealth() {
	if (!Enemy && !Player) return;

	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);

	if (Enemy) {
		HealthBar->SetPercent(Enemy->GetHealthRatio());
		CurrentHealthLabel->SetText(FText::AsNumber(Enemy->CurrentHealth, &Opts));
		CurrentHealthLabel->SetText(FText::AsNumber(Enemy->MaxHealth, &Opts));
	}
	else if (Player) {
		HealthBar->SetPercent(Player->GetHealthRatio());
		CurrentHealthLabel->SetText(FText::AsNumber(Player->CurrentHealth, &Opts));
		CurrentHealthLabel->SetText(FText::AsNumber(Player->MaxHealth, &Opts));
	}
}

void UHealthWidget::SetOwner(ACharacter* OtherActor,bool IsPlayer) {
	Owner = OtherActor;
	if (IsPlayer == 0) Enemy = Cast<AEnemy>(Owner);
	else if (IsPlayer == 1) Player = Cast<AMainPlayer>(Owner);
	else UE_LOG(LogTemp, Warning, TEXT("HealthWidget SetOwner Error!"));
}