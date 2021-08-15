#include "AttackFunction.h"

UAttackFunction::UAttackFunction()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackFunction::BeginPlay()
{
	Super::BeginPlay();
}

void UAttackFunction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttackFunction::SetOwner(USkeletalMeshComponent* TakeMesh,AController* TakeController ) {
	Owner = GetOwner();
	Mesh = TakeMesh;
	Controller = TakeController;
}

void UAttackFunction::AttackStart(FVector Location, FVector Forward, TSubclassOf<UDamageType> DamageType,FString Type, UParticleSystem* HitParticle,float AttackRange)
{
	ECollisionChannel AttackChanel;

	if (Type == "Player") {
		AttackChanel = ECollisionChannel::ECC_GameTraceChannel5;
	}
	else if (Type == "Enemy") {
		AttackChanel = ECollisionChannel::ECC_GameTraceChannel4;
	}

	FHitResult HitResult; //맞은 정보를 저장
	FCollisionQueryParams Params(NAME_None, false, Owner);

	bool bResult = GetWorld()->SweepSingleByChannel(HitResult, Location, Location + Forward * AttackRange,
		FQuat::Identity, AttackChanel, FCollisionShape::MakeSphere(AttackRange), Params);

	//구의 정보 (생략가능)
	FVector TraceVec = Forward * AttackRange;
	FVector Center = Location + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + 30.f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 0.5f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, 30.f, CapsuleRot, DrawColor, false, DebugLifeTime);

	if (bResult) {
		if (HitResult.Actor.IsValid()) {
			if (Type == "Player") Hited = Cast<AEnemy>(HitResult.Actor);
			else if (Type == "Enemy") Hited = Cast<AMainPlayer>(HitResult.Actor);
			if (Hited) {
				UGameplayStatics::ApplyDamage(Hited, 10.f, Controller, Owner, DamageType);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.GetActor()->GetActorLocation(), FRotator(0.f));
			}
		}
	}
}