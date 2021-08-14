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
