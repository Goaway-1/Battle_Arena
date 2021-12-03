#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "Balance.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATARENA_API UBalance : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBalance();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "BALANCE")
	float Currentbalance;

	UPROPERTY(VisibleAnywhere, Category = "BALANCE")
	float Maxbalance;

	UPROPERTY(VisibleAnywhere, Category = "BALANCE")
	float BalanceRatio;

	UPROPERTY(VisibleAnywhere, Category = "BALANCE")
	bool bIsDecreaseBalance;

public:
	UFUNCTION()
	void BalancePercent();

	FORCEINLINE float GetMaxBalance() { return Maxbalance; }
	FORCEINLINE float GetCurrentBalance() { return Currentbalance; }
	FORCEINLINE float GetBalanceRatio() { return BalanceRatio; }
	FORCEINLINE void SetBalanceRatio(float value) { BalanceRatio = value; }
	FORCEINLINE void SetCurrentBalance(float value) { Currentbalance += value; }
	FORCEINLINE void SetDecreaseBalance(bool value) { bIsDecreaseBalance = value; }
};
