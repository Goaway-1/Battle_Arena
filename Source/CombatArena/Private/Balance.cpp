#include "Balance.h"

UBalance::UBalance(){
	PrimaryComponentTick.bCanEverTick = true;

	Maxbalance = 100.f;
	Currentbalance = 0.f;
	bIsDecreaseBalance = false;
}

void UBalance::BeginPlay()
{
	Super::BeginPlay();
}

void UBalance::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	BalancePercent();
}

void UBalance::BalancePercent() {
	if (bIsDecreaseBalance && Currentbalance > 0.f) {
		Currentbalance -= 0.1f;
		if (Currentbalance < 0.f) Currentbalance = 0.f;
	}
	BalanceRatio = Currentbalance / Maxbalance;
}