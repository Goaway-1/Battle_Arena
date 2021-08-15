#include "PlayerAttackFunction.h"

UPlayerAttackFunction::UPlayerAttackFunction() {

	KickRange = 120.f;
	KickRadius = 30.f;
	bKicking = false;
	bCanKick = true;
}

void UPlayerAttackFunction::BeginPlay() {
	Super::BeginPlay();
}

void UPlayerAttackFunction::Kick(UAnimInstance* Anim, UAnimMontage* Montage) {
	if (!bCanKick) return;

	bCanKick = false;
	bKicking = true;
	if (Anim && Montage) {
		Anim->Montage_Play(Montage);
		Anim->Montage_JumpToSection("Kick", Montage);
	}
}

void UPlayerAttackFunction::KickStart(FVector Location, FVector Forward) {
	FHitResult HitResult; //맞은 정보를 저장

	//탐색방법에 대한 설정 값을 모은 구조체
	//이름, 추적 복잡성 여부,
	FCollisionQueryParams Params(NAME_None, false, Owner);
	bool bReslut = GetWorld()->SweepSingleByChannel(
		HitResult,
		Location,
		Location + Forward * KickRange,
		FQuat::Identity,		//회전없음.
		ECollisionChannel::ECC_GameTraceChannel5,	//Kick의 채널 번호
		FCollisionShape::MakeSphere(KickRadius),
		Params);

	//구의 정보 (생략가능)
	FVector TraceVec = Forward * KickRange;
	FVector Center = Location + TraceVec * 0.5f;
	float HalfHeight = KickRange * 0.5f + KickRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bReslut ? FColor::Green : FColor::Red;
	float DebugLifeTime = 0.5f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, KickRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

	/// <summary>
	/// 넉백을 시키는 실질적인 부분.
	/// Player의 시점으로 넉백.
	/// </summary>
	if (bReslut) {
		if (HitResult.Actor.IsValid()) {
			AEnemy* KnockBackEnemy = Cast<AEnemy>(HitResult.Actor);
			if (KnockBackEnemy) {
				FVector VectorToBack = FVector(Forward.X, Forward.Y, 0);
				KnockBackEnemy->KnockBack(VectorToBack);
			}
		}
	}
}

void UPlayerAttackFunction::KickEnd() {
	bKicking = false;
	bCanKick = true;
}