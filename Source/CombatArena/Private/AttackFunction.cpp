#include "AttackFunction.h"
#include "CollisionQueryParams.h"

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

void UAttackFunction::AttackStart(FVector Location, FVector Forward, TSubclassOf<UDamageType> DamageType,FString Type, UParticleSystem* HitParticle,float AttackRange,float Damage)
{
	//찾아낼 액터의 트레이스 채널
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	//찾아낼 액터의 타입
	UClass* SeekClass = nullptr;

	//찾아낸 액터를 저장할 배열
	TArray<AActor*> OutActors;

	//무시될 액터의 배열
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(Owner, 1);

	if (Type == "Player") {
		TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
		SeekClass = AEnemy::StaticClass();
	}
	else if (Type == "Enemy") {
		TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
		SeekClass = AMainPlayer::StaticClass();
	}

	bool bResult = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Owner->GetActorLocation(), AttackRange, TraceObjectTypes, SeekClass, IgnoreActors, OutActors);

	/** Draw Image */
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), Owner->GetActorLocation(), AttackRange, 12, DrawColor, false, 0.5f);

	/** 실질적인 알고리즘 */
	if (bResult) {
		for (auto& HitActor : OutActors)
		{
			// 내적의 크기
			float Inner = Owner->GetDotProductTo(HitActor);
			if (Inner > 0.3f) {
				if (Type == "Player") Hited = Cast<AEnemy>(HitActor);
				else if (Type == "Enemy") Hited = Cast<AMainPlayer>(HitActor);
				if (Hited) {
					UGameplayStatics::ApplyDamage(Hited, Damage, Controller, Owner, DamageType);
				}
			}
		}
	}
}