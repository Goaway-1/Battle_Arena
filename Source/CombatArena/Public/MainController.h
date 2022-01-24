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
private:
	class UHealthWidget* HealthBarOutLine;

	class AMainPlayer* MainPlayer;

	//Health Bar
	class UUserWidget* PlayerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> WPlayerMainHealth;

	class UStaminaWidget* StaminaBarOutLine;

	/** Pause Menu */
	UPROPERTY(EditAnywhere, Category = "Widget", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> WPauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = true))
	UUserWidget* PauseMenu;

	bool bPauseMenuVisible;

	/** Fog */
	UPROPERTY(EditAnywhere, Category = "Widget", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> WFogSplatter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = true))
	UUserWidget* FogSplatter;

	bool bFogSplatterVisible;

	/* Balacne **/
	class UBalanceWidget* BalanceBarOutLine;
	class UBalanceWidget* EnemyBalanceBarOutLine;
	class ABoss_Enemy* BalanceTargetEnemy;
public:
	void SetPlayerHealth();		//Player�� ü�� UI ����. MainPlayer���� ȣ��
	void SetPlayerStamina();

	UFUNCTION(BlueprintNativeEvent)
	void DisplayPauseMenu();

	UFUNCTION(BlueprintNativeEvent)
	void RemovePauseMenu();

	void TogglePauseMenu(); 

	/** Fog */
	FORCEINLINE bool GetFogSplatterVisible() { return bFogSplatterVisible; }

	UFUNCTION(BlueprintNativeEvent)
	void DisplayFogSplatter();

	UFUNCTION(BlueprintNativeEvent)
	void RemoveFogSplatter();

	void ToggleFogSplatter();

	/** Balacne */
	void SetPlayerBalance();
	void SetBalanceTarget(ABoss_Enemy* value);
	void SetEnemyBalance();
};
