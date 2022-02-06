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
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;		
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));

	UClass* SeekClass = AEnemy::StaticClass();
	TArray<AActor*> OutActors;
	TArray<AActor*> IgnoreActors;	
	IgnoreActors.Init(Owner, 1);	

	bool bResult= UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Owner->GetActorLocation(), 200.f, TraceObjectTypes, SeekClass, IgnoreActors, OutActors);

	/** Draw Image */

	/** 실질적인 알고리즘 */
	if (bResult) {
		for (auto& HitActor : OutActors)
		{
			// 내적의 크기
			float Inner = Owner->GetDotProductTo(HitActor);
			if (Inner > 0.5f) {
				AEnemy* KnockBackEnemy = Cast<AEnemy>(HitActor);
				if (KnockBackEnemy) {
					FVector VectorToBack = FVector(Forward.X, Forward.Y, 0);
					KnockBackEnemy->KnockBack(VectorToBack);
				}
			}
		}
	}
}

void UPlayerAttackFunction::KickEnd() {
	bKicking = false;
	bCanKick = true;
}