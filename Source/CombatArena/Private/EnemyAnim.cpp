#include "EnemyAnim.h"
#include "Enemy.h"

void UEnemyAnim::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	if(!Enemy) Enemy = Cast<AEnemy>(TryGetPawnOwner());

}
void UEnemyAnim::PostInitProperties(){
	Super::PostInitProperties();

	OnMontageEnded.AddUniqueDynamic(this, &UEnemyAnim::OnAttackMontageEnded);
}
void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
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