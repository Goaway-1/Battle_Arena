#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "PlayerCameraShake.generated.h"

/**
 * Camera Shake
 */
UCLASS()
class COMBATARENA_API UPlayerCameraShake : public UMatineeCameraShake
{
	GENERATED_BODY()
public:
	UPlayerCameraShake();
};
