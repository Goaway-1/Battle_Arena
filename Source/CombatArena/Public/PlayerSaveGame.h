#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

/**
 * SaveData
 */
 USTRUCT(BlueprintType)
 struct FCharacterStats
 {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Stamina;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;
 };

UCLASS()
class COMBATARENA_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPlayerSaveGame();

	UPROPERTY(VisibleAnywhere, Category = "DefaultData")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = "DefaultData")
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = "DefaultData")
	FCharacterStats CharacterStats;
};
