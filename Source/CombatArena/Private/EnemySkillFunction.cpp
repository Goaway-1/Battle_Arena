#include "EnemySkillFunction.h"
#include "MainPlayer.h"
#include "EnemyController.h"
#include "SK_Meteor.h"
#include "Kismet/KismetMathLibrary.h"
#include "MagicBall.h"
#include "Lazer.h"

UEnemySkillFunction::UEnemySkillFunction() {
	ESkillDecal = CreateDefaultSubobject<UDecalComponent>("ESkillDecal");
	ESkillDecal->DecalSize = FVector(10.f, 200.f, 200.f);
	ESkillDecal->SetVisibility(false);
}
void UEnemySkillFunction::BeginPlay() {
	Super::BeginPlay();
	
	/** Set Delegate */
	SkillDelegate.BindUObject(this, &UEnemySkillFunction::ConfirmTargetAndContinue);
}
void UEnemySkillFunction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
void UEnemySkillFunction::SetInitial(APawn* P, USkeletalMeshComponent* S, AController* C, AActor* A) {
	Super::SetInitial(P, S, C, A);

	ESkillDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	ESkillDecal->SetDecalMaterial(DecalMaterial);
}
void UEnemySkillFunction::LazerAttack() {
	/** Lazer 생성 (존재한다면 무시) */
	if (Lazer.Num() == 0) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerActor;
		SpawnParams.Instigator = OwnerInstigator;
		for (int i = 0; i < LazerCnt; i++) {
			Lazer.Add(GetWorld()->SpawnActor<AActor>(LazerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams));
		}
	}

	/** Lazer들의 위치 및 활성화 */
	for (int32 i = 0; i < Lazer.Num(); i++) {
		RandPos(LazerLoc, LazerRot);
		ALazer* laz = Cast<ALazer>(Lazer[i]);
		laz->SetActorLocationAndRotation(LazerLoc, LazerRot);
		
		laz->SetCnt(HitCnt);
		laz->SetActorHiddenInGame(false);
		laz->SetActorEnableCollision(true);
		laz->SetActorTickEnabled(true);
	}
	SetHitCnt();
}
void UEnemySkillFunction::LazerEnd() {
	for (int32 i = 0; i < Lazer.Num(); i++){
		Lazer[i]->SetActorHiddenInGame(true);
		Lazer[i]->SetActorEnableCollision(false);
		Lazer[i]->SetActorTickEnabled(false);
	}
}
void UEnemySkillFunction::RandPos(FVector& Loc, FRotator& Rot) {
	Loc = OwnerActor->GetActorLocation();
	Rot = OwnerActor->GetActorRotation();
	

	Loc.X += UKismetMathLibrary::RandomFloatInRange(-1000.f, 1000.f);
	Loc.Y += UKismetMathLibrary::RandomFloatInRange(-1000.f, 1000.f);
	Loc.Z += 300.f;
	Rot.Roll += UKismetMathLibrary::RandomFloatInRange(-1000.f, 1000.f);
	Rot.Pitch = -90.f;
	Rot.Yaw += UKismetMathLibrary::RandomFloatInRange(-1000.f, 1000.f);
}
void UEnemySkillFunction::GroundAttack() {
	if (!bGround) {
		bGround = true;
		ESkillDecal->SetVisibility(true);
	}
	else {
		bGround = false;
		ESkillDecal->SetVisibility(false);
		SpawnMeteor();
	}
}
void UEnemySkillFunction::SetSkillLocation() {
	if (!bGround) return;

	AEnemyController* Con = Cast<AEnemyController>(OwnerController);
	out = Con->GetTargetVec();
	ESkillDecal->SetWorldLocation(out);
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
			PlayerOverlaped->SetCurrentAttack(GetName() + "AttackMeteor" + FString::FromInt(HitCnt));
			UGameplayStatics::ApplyDamage(PlayerOverlaped, 10.f, OwnerController,OwnerPawn, MeteorDamageType);
			SetHitCnt();
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
void UEnemySkillFunction::MagicAttack() {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = OwnerInstigator;

	FVector Loc = OwnerActor->GetActorLocation();
	FRotator Rot =  OwnerActor->GetActorRotation();
	Loc.X += 50.f;
	Rot.Pitch += 5.f;		
	Magic = GetWorld()->SpawnActor<AMagicBall>(MagicClass, Loc, Rot, SpawnParams);
	Magic->SetCnt(HitCnt);
	SetHitCnt();
}
void UEnemySkillFunction::MagicEnd() {
	if (Magic) Magic = nullptr;
}
void UEnemySkillFunction::SetHitCnt() {
	if (++HitCnt > 2) HitCnt = 0;
}