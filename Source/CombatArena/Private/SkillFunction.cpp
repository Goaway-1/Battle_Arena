#include "SkillFunction.h"
#include "Components/DecalComponent.h"
#include "Enemy.h"

USkillFunction::USkillFunction()
{
	PrimaryComponentTick.bCanEverTick = true;

#pragma region SKILL
	SkillDecal = CreateDefaultSubobject<UDecalComponent>("SkillDecal");
	//SkillDecal->SetupAttachment(GetRootComponent());
	SkillDecal->DecalSize = FVector(10.f, 200.f, 200.f);
	SkillDecal->SetVisibility(false);

	bGround = false;
#pragma endregion
}

void USkillFunction::BeginPlay()
{
	Super::BeginPlay();
	
}

void USkillFunction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/** Targeting On Ground */
	SetSkillLocation(out);
}

void USkillFunction::SetInitial(APawn* P, USkeletalMeshComponent* S, AController* C,AActor* A) {
	OwnerInstigator = P;
	OwnerSkeletal = S;
	OwnerController = C;
	OwnerActor = A;
}
void USkillFunction::SkillBegin() {
	//LazerAttack();	//Lazer
//	GroundAttack();
}

void USkillFunction::SkillEnd() {
	//LazerEnd();		//Lazer
	//GroundAttack();
}
void USkillFunction::LazerAttack() {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = OwnerInstigator;
	Lazer = GetWorld()->SpawnActor<AActor>(LazerClass, FVector(0.f), FRotator(0.f), SpawnParams);

	Lazer->AttachToComponent(OwnerSkeletal, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName("LazerPos"));
}
void USkillFunction::LazerEnd() {
	if (Lazer) {
		Lazer->Destroy();
	}
}
void USkillFunction::GroundAttack(FRotator Rot) {
	if (!bGround) {
		bGround = true;

		//Camera Pos (수정 필요.)
		OwnerController->SetInitialLocationAndRotation(FVector(0.f), Rot);
		//ZoomInCam(FVector(-200.f, 0.f, 400.f), FRotator(-30.f, 0.f, 0.f));
		SkillDecal->SetVisibility(true);
	}
	else {
		bGround = false;

		//Camera Pos (수정 필요.)
		OwnerController->SetControlRotation(Rot);
		//ZoomOutCam();
		SkillDecal->SetVisibility(false);
	}
}
void USkillFunction::SetSkillLocation(FVector& OutViewPoint) {
	if (!bGround) return;

	FVector ViewPoint;
	FRotator ViewRotation;
	OwnerController->GetPlayerViewPoint(ViewPoint, ViewRotation);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams(NAME_None, false, OwnerActor);
	QueryParams.bTraceComplex = true;
	APawn* OwnerPawn = OwnerController->GetPawn();

	if (OwnerPawn) QueryParams.AddIgnoredActor(OwnerPawn->GetUniqueID());	//주인은 무시

	//플레이어의 시점에 있는 곳
	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPoint, ViewPoint + ViewRotation.Vector() * 10000.f, ECC_Visibility, QueryParams);
	if (TryTrace) {
		out = HitResult.ImpactPoint;
		SkillDecal->SetWorldLocation(out);
	}
	else out = FVector();
}
void USkillFunction::ConfirmTargetAndContinue() {
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
	//GroundAttack();
}