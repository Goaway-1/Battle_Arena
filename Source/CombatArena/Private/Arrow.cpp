#include "Arrow.h"

AArrow::AArrow()
{
 	PrimaryActorTick.bCanEverTick = true;

	ArrowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	RootComponent = ArrowMesh;

	ArrowMesh->SetRelativeScale3D(FVector(1.f,5.f,5.f));
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

