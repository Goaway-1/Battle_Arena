#include "EnemyAnim.h"
#include "Enemy.h"

void UEnemyAnim::NativeInitializeAnimation() {
	if(!Enemy) Enemy = Cast<AEnemy>(TryGetPawnOwner());

	OnMontageEnded.AddDynamic(this, &UEnemyAnim::OnAttackMontageEnded);
}

void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds) {
	if (!Enemy) Enemy = Cast<AEnemy>(TryGetPawnOwner());

	if (Enemy) {
		FVector CurrentSpeed = Enemy->GetVelocity();
		Speed = FVector(CurrentSpeed.X, CurrentSpeed.Y, 0).Size();
	}
}
void UEnemyAnim::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	if (!Enemy->GetIsAttacking()) return;
	Enemy->SetIsAttacking(false);
	OnAttackEnd.Broadcast();
}