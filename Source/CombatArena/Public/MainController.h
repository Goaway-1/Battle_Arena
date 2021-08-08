#pragma once

#include "EngineMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainController.generated.h"

UCLASS()
class COMBATARENA_API AMainController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainController();

	virtual void BeginPlay() override;

#pragma region WIDGET
	//Health Bar
	UPROPERTY(VisibleAnywhere, Category = "Widget | EnemyWidget")
	class UUserWidget* PlayerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | EnemyWidget")
	TSubclassOf<class UUserWidget> WPlayerMainHealth;
#pragma endregion
};
