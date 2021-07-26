#include "MainGameMode.h"
#include "Mainplayer.h"
#include "MainController.h"

AMainGameMode::AMainGameMode() {
	PlayerControllerClass = AMainController::StaticClass();
}