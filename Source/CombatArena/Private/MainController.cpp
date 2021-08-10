#include "MainController.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayer.h"

AMainController::AMainController() {

}

void AMainController::BeginPlay() {
	if (WPlayerMainHealth) {
		PlayerWidget = CreateWidget<UUserWidget>(this, WPlayerMainHealth);
		if (PlayerWidget) {
			PlayerWidget->AddToViewport();
			PlayerWidget->SetVisibility(ESlateVisibility::Visible);	
		}
	}
}
