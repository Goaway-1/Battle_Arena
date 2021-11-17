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

	/** Pause Menu */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> WPauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* PauseMenu;

	bool bPauseMenuVisible;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category= "HUD")
	void DisplayPauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable,Category = "HUD")
	void RemovePauseMenu();

	void TogglePauseMenu(); 

	/** FogSplatter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> WFogSplatter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* FogSplatter;

	UPROPERTY()
	bool bFogSplatterVisible;

	UFUNCTION()
	FORCEINLINE bool GetFogSplatterVisible() { return bFogSplatterVisible; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayFogSplatter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveFogSplatter();

	void ToggleFogSplatter();
#pragma endregion
};
