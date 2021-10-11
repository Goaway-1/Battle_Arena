#include "Arrow.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/RadialForceComponent.h"

AArrow::AArrow()
{
 	PrimaryActorTick.bCanEverTick = true;

	ArrowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	RootComponent = ArrowMesh;

	ArrowMesh->SetRelativeScale3D(FVector(1.f,5.f,5.f));
	SetArrowStatus(EArrowStatus::EAS_InBow);

	FirePower = 0;

	/** Destructible Mesh */
	RadiaForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadiaForce"));
	RadiaForce->SetupAttachment(GetRootComponent());

	RadiaForce->ImpulseStrength = 5000.f;		//충돌힘
	RadiaForce->ForceStrength = 1000.f;			//힘의 강도
	RadiaForce->DestructibleDamage = 1000.f;	//디스트럭티블 메쉬에 손상을 입히는 데미지량.

}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnStateBegine();
}

void AArrow::OnStateBegine() {
	switch (ArrowStatus)
	{
	case EArrowStatus::EAS_Normal:
		break;
	case EArrowStatus::EAS_Unobtained:
		ArrowMesh->SetCollisionProfileName(FName("OverlapAllDynamic"));
		ArrowMesh->SetSimulatePhysics(true);
		break;
	case EArrowStatus::EAS_InBow:
		ArrowMesh->SetCollisionProfileName(FName("NoCollision"));
		ArrowMesh->SetSimulatePhysics(false);
		break;
	case EArrowStatus::EAS_InArrow:
		ArrowMesh->SetCollisionProfileName(FName("Arrow"));
		ArrowMesh->SetSimulatePhysics(true);
		if(!bisFire){
			ArrowMesh->AddImpulse(GetActorForwardVector() * UKismetMathLibrary::Lerp(7000, 50000, FirePower));	
		}
		bisFire = true;
		break;
	default:
		break;
	}
}

void AArrow::SetArrowStatus(EArrowStatus Status) {
	ArrowStatus = Status;
}

void AArrow::Fire(float Amount) {
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetArrowStatus(EArrowStatus::EAS_InArrow);
	FirePower = Amount;
}