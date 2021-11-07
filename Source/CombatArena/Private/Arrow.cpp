#include "Arrow.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Enemy.h"

AArrow::AArrow()
{
 	PrimaryActorTick.bCanEverTick = true;

	/** ArrowCollision */
	ArrowCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ArrowCollision"));
	RootComponent = ArrowCollision;
	ArrowCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBegin);
	SetArrowStatus(EArrowStatus::EAS_InBow);

	/** Mesh */
	ArrowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(GetRootComponent());
	ArrowMesh->SetRelativeScale3D(FVector(1.f,5.f,5.f));
	ArrowMesh->SetSimulatePhysics(false);
	ArrowMesh->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBegin);

	FirePower = 0;
	bisFire = false;

	/** For Destructible Mesh */
	RadiaForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadiaForce"));
	RadiaForce->SetupAttachment(GetRootComponent());

	RadiaForce->ImpulseStrength = 5000.f;		//충돌힘
	RadiaForce->ForceStrength = 1000.f;			//힘의 강도
	RadiaForce->DestructibleDamage = 1000.f;	//디스트럭티블 메쉬에 손상을 입히는 데미지량.

	Damage = 20.f;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnStateBegin();
}

void AArrow::InitalArrow(AActor* AOwner, AController* AController) {
	ArrowOwner = AOwner;
	ArrowController = AController;
}

void AArrow::OnStateBegin() {
	switch (ArrowStatus)
	{
	case EArrowStatus::EAS_InBow:
		ArrowCollision->SetCollisionProfileName(FName("NoCollision"));
		ArrowCollision->SetSimulatePhysics(false);
		break;
	case EArrowStatus::EAS_InArrow:
		ArrowCollision->SetCollisionProfileName(FName("Arrow"));
		ArrowCollision->SetSimulatePhysics(true);
		if(!bisFire){
			ArrowCollision->AddImpulse(GetActorForwardVector() * UKismetMathLibrary::Lerp(50000, 1000000, FirePower));
			bisFire = true;
		}
		break;
	case EArrowStatus::EAS_Destroy:
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

void AArrow::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		SetArrowStatus(EArrowStatus::EAS_Destroy);

		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy) {
			UGameplayStatics::ApplyDamage(Enemy, 10.f, ArrowController, ArrowOwner, DamageType);
			ArrowCollision->SetSimulatePhysics(false);
			SetActorLocation(GetActorLocation());
			OtherActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}