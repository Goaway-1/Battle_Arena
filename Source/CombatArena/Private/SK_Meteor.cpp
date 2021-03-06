#include "SK_Meteor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainPlayer.h"
#include "EnemySkillFunction.h"

ASK_Meteor::ASK_Meteor()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Flying_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Flying_Particle"));
	CollisionBox->SetupAttachment(GetRootComponent());
	Flying_Particle->SetupAttachment(CollisionBox);
	Flying_Particle->SetRelativeScale3D(FVector(3.0f));

	CollisionBox->SetSimulatePhysics(true);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

void ASK_Meteor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASK_Meteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** Is Actor Detected a Ground */
	IsInGround();
}

void ASK_Meteor::SetInitial(UEnemySkillFunction* Function) {
	SkillFunction = Function;
}

void ASK_Meteor::OnConstruction(const FTransform& Transform) {
	if (!Moving_Particle) return;
	Flying_Particle->SetTemplate(Moving_Particle);
}

void ASK_Meteor::IsInGround() {
	if(bIsEnd) return;

	FVector EndVec = GetActorLocation();
	EndVec.Z -= 20.f;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams(NAME_None, false, this);
	QueryParams.bTraceComplex = true;

	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), EndVec, ECC_Visibility, QueryParams);
	if(TryTrace){
		bIsEnd = true;
		SkillFunction->SkillDelegate.ExecuteIfBound();		//EnemySkillFunction의 Delegate -> 공격을 판단.
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Impact_Particle, GetActorLocation() + Location_Offset_Impact_Particle, Rotate_Impact_Particle);
	Flying_Particle->Deactivate();
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
		Flying_Particle->DestroyComponent();
		Destroy();
		}, 2.0f, 1);
}