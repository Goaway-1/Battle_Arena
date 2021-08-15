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
	FHitResult HitResult; //���� ������ ����

	//Ž������� ���� ���� ���� ���� ����ü
	//�̸�, ���� ���⼺ ����,
	FCollisionQueryParams Params(NAME_None, false, Owner);
	bool bReslut = GetWorld()->SweepSingleByChannel(
		HitResult,
		Location,
		Location + Forward * KickRange,
		FQuat::Identity,		//ȸ������.
		ECollisionChannel::ECC_GameTraceChannel5,	//Kick�� ä�� ��ȣ
		FCollisionShape::MakeSphere(KickRadius),
		Params);

	//���� ���� (��������)
	FVector TraceVec = Forward * KickRange;
	FVector Center = Location + TraceVec * 0.5f;
	float HalfHeight = KickRange * 0.5f + KickRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bReslut ? FColor::Green : FColor::Red;
	float DebugLifeTime = 0.5f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, KickRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

	/// <summary>
	/// �˹��� ��Ű�� �������� �κ�.
	/// Player�� �������� �˹�.
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