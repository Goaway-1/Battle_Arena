#include "MainPlayer.h"

AMainPlayer::AMainPlayer()
{
 	PrimaryActorTick.bCanEverTick = true;

#pragma region CAMERA
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;	//�÷��̾ ��Ʈ�� �� �� �ְ� �������
	SpringArm->TargetArmLength = 600.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

#pragma endregion

}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Lookup"), this, &AMainPlayer::Lookup);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AMainPlayer::Turn);
}


void AMainPlayer::Lookup(float value) {
	AddControllerYawInput(value * CameraSpeed* GetWorld()->GetDeltaSeconds());
}

void AMainPlayer::Turn(float value) {
	AddControllerPitchInput(value * CameraSpeed*GetWorld()->GetDeltaSeconds());
}
