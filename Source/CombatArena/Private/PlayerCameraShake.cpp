#include "PlayerCameraShake.h"


UPlayerCameraShake::UPlayerCameraShake() {
	OscillationDuration = 0.1f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(0.7f, 1.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(12.0f, 20.0f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(0.7f, 1.0f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(12.0f, 20.0f);
}