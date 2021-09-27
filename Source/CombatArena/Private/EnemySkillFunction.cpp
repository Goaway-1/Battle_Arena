#include "EnemySkillFunction.h"
#include "MainPlayer.h"
#include "EnemyController.h"
#include "SK_Meteor.h"

UEnemySkillFunction::UEnemySkillFunction() {
	
}

void UEnemySkillFunction::BeginPlay() {
	Super::BeginPlay();
	
	/** Set Delegate */
	SkillDelegate.BindUObject(this, &UEnemySkillFunction::ConfirmTargetAndContinue);
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
		SkillDecal->SetVisibility(false);
		SpawnMeteor();
	}
}

void UEnemySkillFunction::SetSkillLocation() {
	if (!bGround) return;

	AEnemyController* Con = Cast<AEnemyController>(OwnerController);
	out = Con->GetTargetVec();
	SkillDecal->SetWorldLocation(out);
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

	/** Apply Damage for Player */
	if (TryOverlap) {
		for (auto i : Overlaps) {
			AMainPlayer* PlayerOverlaped = Cast<AMainPlayer>(i.GetActor());
			if (PlayerOverlaped && !OverlapedEnemy.Contains(PlayerOverlaped)) OverlapedEnemy.Add(PlayerOverlaped);
		}
		for (auto i : OverlapedEnemy) {
			AMainPlayer* PlayerOverlaped = Cast<AMainPlayer>(i);
			UGameplayStatics::ApplyDamage(PlayerOverlaped, 10.f, OwnerController,OwnerPawn, MeteorDamageType); 
		}
	}
}

void UEnemySkillFunction::SpawnMeteor() {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = OwnerInstigator;

	/** Spawn Location */
	FVector tmp = out;
	tmp.Z += 1000.f;

	Meteor = GetWorld()->SpawnActor<ASK_Meteor>(MeteorClass, FVector(tmp), FRotator(0.f), SpawnParams);
	Meteor->SetInitial(this);
}