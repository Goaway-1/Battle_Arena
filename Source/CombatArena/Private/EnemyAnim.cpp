#include "EnemyAnim.h"
#include "Enemy.h"

void UEnemyAnim::NativeInitializeAnimation() {
	if(!Enemy) Enemy = Cast<AEnemy>(TryGetPawnOwner());
}

void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds) {
	if (!Enemy) Enemy = Cast<AEnemy>(TryGetPawnOwner());

	if (Enemy) {
		FVector CurrentSpeed = Enemy->GetVelocity();
		Speed = FVector(CurrentSpeed.X, CurrentSpeed.Y, 0).Size();
	}
}