#include "Meteor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainPlayer.h"
#include "EnemySkillFunction.h"

AMeteor::AMeteor()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Flying_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Flying_Particle"));
	RootComponent = CollisionBox;
	Flying_Particle->SetupAttachment(CollisionBox);
	Flying_Particle->SetRelativeScale3D(FVector(3.0f));

	CollisionBox->SetSimulatePhysics(true);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

void AMeteor::BeginPlay(){
	Super::BeginPlay();

}

void AMeteor::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	/** Is Actor Detected a Ground */
	IsInGround();
}

void AMeteor::SetInitial(UEnemySkillFunction* Function) {
	SkillFunction = Function;
}

void AMeteor::OnConstruction(const FTransform& Transform) {
	if (!Moving_Particle) return;
	Flying_Particle->SetTemplate(Moving_Particle);
}

void AMeteor::IsInGround() {
	if (bIsEnd) return;

	FVector EndVec = GetActorLocation();
	EndVec.Z -= 20.f;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams(NAME_None, false, this);
	QueryParams.bTraceComplex = true;

	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), EndVec, ECC_Visibility, QueryParams);
	if (TryTrace) {
		bIsEnd = true;
		SkillFunction->SkillDelegate.ExecuteIfBound();		//EnemySkillFunction�� Delegate -> ������ �Ǵ�.
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Impact_Particle, GetActorLocation() + Location_Offset_Impact_Particle, Rotate_Impact_Particle);
	Flying_Particle->Deactivate();
	GetWorldTimerManager().ClearTimer(handle);
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
		if (this) {
			Flying_Particle->DestroyComponent();
			Destroy();
		}
	}, 1.5f, 1);
}