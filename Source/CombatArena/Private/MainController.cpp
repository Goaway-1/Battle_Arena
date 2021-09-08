#include "MainController.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayer.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetTree.h"
#include "MainPlayer.h"
#include "HealthWidget.h"
#include "StaminaWidget.h"

AMainController::AMainController() {
	
}

void AMainController::BeginPlay() {
	if (WPlayerMainHealth) {
		PlayerWidget = CreateWidget<UUserWidget>(this, WPlayerMainHealth);
		MainPlayer = Cast<AMainPlayer>(GetCharacter());
		if (PlayerWidget) {
			PlayerWidget->AddToViewport();
			PlayerWidget->SetVisibility(ESlateVisibility::Visible);	

			//Health,Stamina의 Progress를 찾는 부분
			HealthBarOutLine = PlayerWidget->WidgetTree->FindWidget<UHealthWidget>("PlayerHealth_BP");
			StaminaBarOutLine = PlayerWidget->WidgetTree->FindWidget<UStaminaWidget>("PlayerStanima_BP");

			HealthBarOutLine->SetPlayerOwner(this);
			StaminaBarOutLine->SetOwner(this);

			SetPlayerHealth();
			SetPlayerStamina();
		}
	}
}

void AMainController::SetPlayerHealth() {
	HealthBarOutLine->SetOwnerHealth(MainPlayer->GetHealthRatio(), MainPlayer->MaxHealth, MainPlayer->CurrentHealth);
}

void AMainController::SetPlayerStamina() {
	StaminaBarOutLine->SetOwnerStamina(MainPlayer->GetStaminaRatio(), MainPlayer->MaxStamina, MainPlayer->CurrentStamina);
}