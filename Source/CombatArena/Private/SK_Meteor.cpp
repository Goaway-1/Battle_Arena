#include "SK_Meteor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainPlayer.h"
#include "EnemySkillFunction.h"

ASK_Meteor::ASK_Meteor()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	CollisionBox->SetupAttachment(GetRootComponent());
	SkeletalMesh->SetupAttachment(CollisionBox);

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
}