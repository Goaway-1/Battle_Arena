#include "EnemySkillFunction.h"
#include "MainPlayer.h"
#include "EnemyController.h"

UEnemySkillFunction::UEnemySkillFunction() {

}

void UEnemySkillFunction::BeginPlay() {
	Super::BeginPlay();
}

void UEnemySkillFunction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
void UEnemySkillFunction::GroundAttack() {
	if (!bGround) {
		bGround = true;
		SkillDecal->SetVisibility(true);
	}
	else {
		bGround = false;
		ConfirmTargetAndContinue();
		SkillDecal->SetVisibility(false);
	}
}

void UEnemySkillFunction::SetSkillLocation() {
	if (!bGround) return;

	AEnemyController* Con = Cast<AEnemyController>(OwnerController);
	out = Con->GetTargetVec();
	SkillDecal->SetWorldLocation(out);	//적의 위치
}

void UEnemySkillFunction::ConfirmTargetAndContinue() {
	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AMainPlayer>> OverlapedEnemy;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = false;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	APawn* OwnerPawn = OwnerController->GetPawn();
	if (OwnerPawn) CollisionQueryParams.AddIgnoredActor(OwnerPawn->GetUniqueID());

	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps,
		out, FQuat::Identity, FCollisionObjectQueryParams(ECC_GameTraceChannel1),
		FCollisionShape::MakeSphere(200.f), CollisionQueryParams);

	if (TryOverlap) {
		for (auto i : Overlaps) {
			AMainPlayer* PlayerOverlaped = Cast<AMainPlayer>(i.GetActor());
			if (PlayerOverlaped && !OverlapedEnemy.Contains(PlayerOverlaped)) OverlapedEnemy.Add(PlayerOverlaped);
		}
		for (auto i : OverlapedEnemy) {
			AMainPlayer* PlayerOverlaped = Cast<AMainPlayer>(i);
			UE_LOG(LogTemp,Warning,TEXT("Overlaped"));
		}
	}
	GroundAttack();
}