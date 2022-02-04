#pragma once

#include "EngineMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameStateBase.generated.h"

UCLASS()
class COMBATARENA_API AMainGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMainGameStateBase();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (AllowPrivateAccess = true))
	TArray<class USoundBase*> BattleSound;

	class UAudioComponent* BattleAudio;


	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	int BattleEnemyCnt;
public:
	void StartBattleSound();
	void EndBattleSound();
	void ForceEndBattleSound();
};
