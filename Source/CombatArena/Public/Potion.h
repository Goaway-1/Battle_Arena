#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Potion.generated.h"

UCLASS()
class COMBATARENA_API APotion : public AItem
{
	GENERATED_BODY()
protected:
	APotion();	
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (AllowPrivateAccess = true))
	class USoundWave* UsedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion | Mesh", Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Mesh;
public:
	void UseItem(float &Health);
};
