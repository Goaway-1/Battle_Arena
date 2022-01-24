#include "PlayerSkillFunction.h"

UPlayerSkillFunction::UPlayerSkillFunction() {
	PSkillDecal = CreateDefaultSubobject<UDecalComponent>("PSkillDecal");
	PSkillDecal->DecalSize = FVector(10.f, 200.f, 200.f);
	PSkillDecal->SetVisibility(false);
}

void UPlayerSkillFunction::BeginPlay() {
	Super::BeginPlay();
}

void UPlayerSkillFunction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);
}

void UPlayerSkillFunction::SetInitial(APawn* P, USkeletalMeshComponent* S, AController* C, AActor* A) {
	Super::SetInitial(P, S, C, A);

	PSkillDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	PSkillDecal->SetDecalMaterial(DecalMaterial);
}
void UPlayerSkillFunction::LazerAttack() {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = OwnerInstigator;

	FVector Loc = OwnerActor->GetActorLocation();
	Loc.X += 50.f;
	for (int i = 0; i < 2; i++) {
		Lazer.Emplace(GetWorld()->SpawnActor<AActor>(LazerClass, Loc, OwnerActor->GetActorRotation() * (10 * i), SpawnParams));
	}
}
void UPlayerSkillFunction::LazerEnd() {
	for (auto& index : Lazer)
	{
		index->Destroy();
	}
	Lazer.Empty();
}

void UPlayerSkillFunction::GroundAttack() {
	if (!bGround) {
		bGround = true;
		PSkillDecal->SetVisibility(true);
	}
	else {
		bGround = false;
		PSkillDecal->SetVisibility(false);
	}
}
void UPlayerSkillFunction::SetSkillLocation() {
	if (!bGround) return;

	FVector ViewPoint;
	FRotator ViewRotation;
	OwnerController->GetPlayerViewPoint(ViewPoint, ViewRotation);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams(NAME_None, false, OwnerActor);
	QueryParams.bTraceComplex = true;
	APawn* OwnerPawn = OwnerController->GetPawn();

	if (OwnerPawn) QueryParams.AddIgnoredActor(OwnerPawn->GetUniqueID());	//������ ����

	//�÷��̾��� ������ �ִ� ��
	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPoint, ViewPoint + ViewRotation.Vector() * 10000.f, ECC_Visibility, QueryParams);
	if (TryTrace) {
		out = HitResult.ImpactPoint;
		PSkillDecal->SetWorldLocation(out);
	}
	else out = FVector();
}
void UPlayerSkillFunction::ConfirmTargetAndContinue() {
	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AEnemy>> OverlapedEnemy;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = false;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	APawn* OwnerPawn = OwnerController->GetPawn();
	if (OwnerPawn) CollisionQueryParams.AddIgnoredActor(OwnerPawn->GetUniqueID());

	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps,
		out, FQuat::Identity, FCollisionObjectQueryParams(ECC_GameTraceChannel2),
		FCollisionShape::MakeSphere(200.f), CollisionQueryParams);

	if (TryOverlap) {
		for (auto i : Overlaps) {
			AEnemy* EnemyOverlaped = Cast<AEnemy>(i.GetActor());
			if (EnemyOverlaped && !OverlapedEnemy.Contains(EnemyOverlaped)) OverlapedEnemy.Add(EnemyOverlaped);
		}
		for (auto i : OverlapedEnemy) {
			AEnemy* EnemyOverlaped = Cast<AEnemy>(i);
			i->LaunchSky(FVector(0.f, 0.f, 700.f));
		}
	}
}