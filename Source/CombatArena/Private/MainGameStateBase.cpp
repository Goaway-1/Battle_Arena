#include "MainGameStateBase.h"

AMainGameStateBase::AMainGameStateBase() {
	BattleEnemyCnt = 0;
}

void AMainGameStateBase::StartBattleSound() {
	if(!BattleAudio && BattleSound.Num() > 0) {
		int range = FMath::RandRange(0, BattleSound.Num() - 1);
		BattleAudio = UGameplayStatics::SpawnSound2D(this, BattleSound[range]);
	}
	if(BattleEnemyCnt++ == 0) BattleAudio->SetActive(true, true);
}
void AMainGameStateBase::EndBattleSound() {
	if(--BattleEnemyCnt == 0) BattleAudio->ToggleActive();
}
void AMainGameStateBase::ForceEndBattleSound() {
	BattleAudio->ToggleActive();
}