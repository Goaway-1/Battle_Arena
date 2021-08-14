#include "EnemyAttackFunction.h"
#include "Engine/SkeletalMeshSocket.h"

UEnemyAttackFunction::UEnemyAttackFunction() {
}

void UEnemyAttackFunction::BeginPlay() {
	Super::BeginPlay();
	Owner = Cast<AEnemy>(GetOwner());
}

void UEnemyAttackFunction::AttackStart() {
	FHitResult HitResult; //맞은 정보를 저장

	FCollisionQueryParams Params(NAME_None, false, Owner);

	bool bReslut = GetWorld()->SweepSingleByChannel(HitResult, Owner->GetActorLocation(), Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.f,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4,FCollisionShape::MakeSphere(200.f), Params);

	//구의 정보 (생략가능)
	FVector TraceVec = Owner->GetActorForwardVector() * 200.f;
	FVector Center = Owner->GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = 200.f * 0.5f + 30.f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bReslut ? FColor::Green : FColor::Red;
	float DebugLifeTime = 0.5f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, 30.f, CapsuleRot, DrawColor, false, DebugLifeTime);

	if (bReslut) {
		if (HitResult.Actor.IsValid()) {
			AMainPlayer* HitedPlayer = Cast<AMainPlayer>(HitResult.Actor);
			if (HitedPlayer) {
				UGameplayStatics::ApplyDamage(HitedPlayer, 10.f, Owner->EnemyController, Owner, Owner->EnemyDamageType);
				const USkeletalMeshSocket* R_HitSocket = Owner->GetMesh()->GetSocketByName("Right_Weapon");
				const USkeletalMeshSocket* L_HitSocket = Owner->GetMesh()->GetSocketByName("Left_Weapon");
				if (R_HitSocket && L_HitSocket && HitedPlayer->GetHitParticle()) {
					FVector R_ParticleSpawnLocation = R_HitSocket->GetSocketLocation(Owner->GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitedPlayer->GetHitParticle(), R_ParticleSpawnLocation, FRotator(0.f));

					FVector L_ParticleSpawnLocation = L_HitSocket->GetSocketLocation(Owner->GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitedPlayer->GetHitParticle(), L_ParticleSpawnLocation, FRotator(0.f));
				}
			}
		}
	}
}

