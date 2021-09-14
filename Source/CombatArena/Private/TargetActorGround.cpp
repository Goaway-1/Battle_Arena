#include "TargetActorGround.h"
#include "CollisionQueryParams.h"

ATargetActorGround::ATargetActorGround()
{
 	PrimaryActorTick.bCanEverTick = true;

	Radius = 10.f;

}

void ATargetActorGround::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATargetActorGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATargetActorGround::StartTargeting(APlayerController* Con) {
	Controller = Cast<APlayerController>(Con);
}

void ATargetActorGround::ConfrimTargetingAndContinue() {
	FVector ViewLocation;
	GetPlayerLookingPoint(ViewLocation);

	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AActor>> OverlapedActors;
	bool TraceComplex = false;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = TraceComplex;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	APawn* OwnerPawn = Controller->GetPawn();
	if(OwnerPawn) CollisionQueryParams.AddIgnoredActor(OwnerPawn->GetUniqueID());

	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps,ViewLocation,FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(Radius),
		CollisionQueryParams);
	if (TryOverlap) {
		for (int32 i = 0; i < Overlaps.Num(); i++) {
			APawn* PawnOverlaped = Cast<APawn>(Overlaps[i].GetActor());
			if(PawnOverlaped && !OverlapedActors.Contains(PawnOverlaped)) OverlapedActors.Add(PawnOverlaped);
		}
	}	
}

bool ATargetActorGround::GetPlayerLookingPoint(OUT FVector& OutViewPoint) {
	FVector ViewPoint;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewPoint, ViewRotation);		//플레이어의 관점 획득
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	APawn* OwnerPawn = Controller->GetPawn();

	if (OwnerPawn) QueryParams.AddIgnoredActor(OwnerPawn->GetUniqueID());	//주인은 무시

	//플레이어의 시점에 있는 곳!!
	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPoint, ViewPoint + ViewRotation.Vector() * 10000.f, ECC_Visibility, QueryParams);
	if (TryTrace) OutViewPoint = HitResult.ImpactPoint;
	else OutViewPoint = FVector();

	return TryTrace;
}