#include "MainPlayerAnim.h"
#include "MainPlayer.h"
#include "Kismet/KismetMathLibrary.h"

void UMainPlayerAnim::NativeInitializeAnimation() {
	if (!MainPlayer) MainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());
} 


void UMainPlayerAnim::NativeUpdateAnimation(float DeltaSeconds){
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!MainPlayer) MainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());

	if (MainPlayer) {
		FVector Speed = MainPlayer->GetVelocity();
		FRotator Rotator = MainPlayer->GetActorRotation();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
		DirectionSpeed = CalculateDirection(Speed, Rotator);

		bIsInAir = MainPlayer->GetMovementComponent()->IsFalling();
		bTargetOn = MainPlayer->GetTargeting();		

		/** Rotation */
		FRotator ViewRotator = UKismetMathLibrary::NormalizedDeltaRotator(MainPlayer->GetBaseAimRotation(), MainPlayer->GetActorRotation());
		ViewYaw = ViewRotator.Yaw;
		ViewPitch = ViewRotator.Pitch;

		/** Turn In Place*/
		TurnAxis = MainPlayer->TurnAxis;
	}
}