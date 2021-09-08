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
	UPROPERTY(VisibleAnywhere, Category = "Widget | PlayerWidget")
	class UHealthWidget* HealthBarOutLine;

	UPROPERTY(VisibleAnywhere, Category = "Widget | PlayerWidget")
	class AMainPlayer* MainPlayer;

	//Health Bar
	UPROPERTY(VisibleAnywhere, Category = "Widget | PlayerWidget")
	class UUserWidget* PlayerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | PlayerWidget")
	TSubclassOf<class UUserWidget> WPlayerMainHealth;

	UFUNCTION()
	void SetPlayerHealth();		//Player의 체력 UI 지정. MainPlayer에서 호출

	//Stamina
	UPROPERTY(VisibleAnywhere, Category = "Widget | PlayerWidget")
	class UStaminaWidget* StaminaBarOutLine;

	UFUNCTION()
	void SetPlayerStamina();
#pragma endregion
};
