#include "MainController.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayer.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetTree.h"
#include "MainPlayer.h"
#include "HealthWidget.h"
#include "StaminaWidget.h"
#include "BalanceWidget.h"

AMainController::AMainController() {
	bPauseMenuVisible = false;
}

void AMainController::BeginPlay() {
	if (WPlayerMainHealth) {
		PlayerWidget = CreateWidget<UUserWidget>(this, WPlayerMainHealth);
		MainPlayer = Cast<AMainPlayer>(GetCharacter());
		if (PlayerWidget) {
			PlayerWidget->AddToViewport();
			PlayerWidget->SetVisibility(ESlateVisibility::Visible);	

			//Health, Stamina, Balance의 Progress를 찾는 부분
			HealthBarOutLine = PlayerWidget->WidgetTree->FindWidget<UHealthWidget>("PlayerHealth_BP");
			StaminaBarOutLine = PlayerWidget->WidgetTree->FindWidget<UStaminaWidget>("PlayerStanima_BP");
			BalanceBarOutLine = PlayerWidget->WidgetTree->FindWidget<UBalanceWidget>("PlayerBalance_BP"); 
			EnemyBalanceBarOutLine = PlayerWidget->WidgetTree->FindWidget<UBalanceWidget>("EnemyBalance_BP");

			HealthBarOutLine->SetPlayerOwner(this);
			StaminaBarOutLine->SetOwner(this);
			BalanceBarOutLine->SetPlayerOwner(this);

			SetPlayerHealth();
			SetPlayerStamina();
			SetPlayerBalance();
			SetEnemyBalance();
		}
	}
	if (WPauseMenu) {	
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu) {
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (WFogSplatter) {
		FogSplatter = CreateWidget<UUserWidget>(this, WFogSplatter);
		if (FogSplatter) {
			FogSplatter->AddToViewport();
			FogSplatter->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMainController::SetPlayerHealth() {
	HealthBarOutLine->SetOwnerHealth(MainPlayer->GetHealthRatio(), MainPlayer->MaxHealth, MainPlayer->CurrentHealth);
}

void AMainController::SetPlayerStamina() {
	StaminaBarOutLine->SetOwnerStamina(MainPlayer->GetStaminaRatio(), MainPlayer->MaxStamina, MainPlayer->CurrentStamina);
}

void AMainController::SetPlayerBalance() {	
	BalanceBarOutLine->SetOwnerBalance(MainPlayer->GetBalance()->GetBalanceRatio(), MainPlayer->GetBalance()->GetMaxBalance(), MainPlayer->GetBalance()->GetCurrentBalance());
}

void AMainController::SetBalanceTarget(ABoss_Enemy* value) {
	BalanceTargetEnemy = value;
	EnemyBalanceBarOutLine->SetEnemyOwner(value);
}
void AMainController::SetEnemyBalance() {
	if(!BalanceTargetEnemy || BalanceTargetEnemy->GetBalance()->GetCurrentBalance() <= 0.f) return;
	EnemyBalanceBarOutLine->SetOwnerBalance(BalanceTargetEnemy->GetBalance()->GetBalanceRatio(), BalanceTargetEnemy->GetBalance()->GetMaxBalance(), BalanceTargetEnemy->GetBalance()->GetCurrentBalance());
}

void AMainController::DisplayPauseMenu_Implementation() {
	if (PauseMenu) {
		/** Show Display */
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		/** InputMode */
		FInputModeUIOnly InputModeUIOnly;
		SetInputMode(InputModeUIOnly);
		bShowMouseCursor = true;
	}
}
void AMainController::RemovePauseMenu_Implementation() {
	if (PauseMenu) {
		/** InputMode */
		FInputModeGameOnly InputModeGame;
		SetInputMode(InputModeGame);
		bShowMouseCursor = false;

		/** Remove Display */
		bPauseMenuVisible = false;
	}
}
void AMainController::TogglePauseMenu() {
	return bPauseMenuVisible ? RemovePauseMenu() : DisplayPauseMenu();
}
void AMainController::DisplayFogSplatter_Implementation() {
	if (FogSplatter) {
		bFogSplatterVisible = true;
		FogSplatter->SetVisibility(ESlateVisibility::Visible);
	}
}
void AMainController::RemoveFogSplatter_Implementation() {
	if (FogSplatter) {
		bFogSplatterVisible = false;
		FogSplatter->SetVisibility(ESlateVisibility::Hidden);
	}
}
void AMainController::ToggleFogSplatter() {
	return bFogSplatterVisible ? RemoveFogSplatter() : DisplayFogSplatter();
}