#include "MainPlayer.h"
#include "Kismet/KismetMathLibrary.h"

AMainPlayer::AMainPlayer()
{
 	PrimaryActorTick.bCanEverTick = true;

#pragma region CAMERA
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;	//플레이어가 컨트롤 할 수 있게 만들어줌
	SpringArm->SocketOffset = FVector(0.f, 35.f, 0.f);
	
	//카메라 회전 On
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;

	SpringArm->TargetArmLength = 350.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	//회전해도 플레이어에게 영향 X
	bUseControllerRotationYaw = true; 
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

#pragma endregion

#pragma	region	MOVEMENT
	GetCharacterMovement()->bOrientRotationToMovement = true;	//이동방향 회전
	GetCharacterMovement()->RotationRate = FRotator(0.f,540.f,0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;	//변하지 않아!
	GetCharacterMovement()->AirControl = 0.5f;

	MovementStatus = EMovementStatus::EMS_Normal;
	MoveSpeed = 500.f;
	SprintingSpeed = 750.f;

#pragma endregion

}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetMovementStatus(EMovementStatus::EMS_Normal);
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckIdle();
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Camera
	PlayerInputComponent->BindAxis(FName("Lookup"), this, &AMainPlayer::Lookup);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AMainPlayer::Turn);

	//Movement
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AMainPlayer::MoveRight);

	//Jump
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Shift", EInputEvent::IE_Pressed, this, &AMainPlayer::Switch_Sprinting);
	PlayerInputComponent->BindAction("Shift", EInputEvent::IE_Released, this, &AMainPlayer::Switch_Sprinting);
}

#pragma region CAMERA
void AMainPlayer::Lookup(float value) {
	AddControllerYawInput(value * CameraSpeed * GetWorld()->GetDeltaSeconds());
}

void AMainPlayer::Turn(float value) {
	AddControllerPitchInput(value * CameraSpeed * GetWorld()->GetDeltaSeconds());
}
#pragma endregion

#pragma region MOVEMENT
void AMainPlayer::MoveForward(float Value) {
	AddMovementInput(Camera->GetForwardVector(), Value);
}
void AMainPlayer::MoveRight(float Value) {
	AddMovementInput(Camera->GetRightVector(), Value);
}
void AMainPlayer::Jump() {
	Super::Jump();
}
void  AMainPlayer::SetMovementStatus(EMovementStatus Status) {
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting) {
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}
void AMainPlayer::Switch_Sprinting() {
	if (MovementStatus != EMovementStatus::EMS_Sprinting) SetMovementStatus(EMovementStatus::EMS_Sprinting);
	else {
		if(GetVelocity().Size() == 0) SetMovementStatus(EMovementStatus::EMS_Normal);
		else  SetMovementStatus(EMovementStatus::EMS_Walk);
	}
}
void AMainPlayer::CheckIdle() {
	float CurrentVelocity = GetVelocity().Size();

	if (CurrentVelocity == 0) {
		SetMovementStatus(EMovementStatus::EMS_Normal);
		bUseControllerRotationYaw = false;
	}
	else if (MovementStatus != EMovementStatus::EMS_Sprinting) {
		SetMovementStatus(EMovementStatus::EMS_Walk);
		bUseControllerRotationYaw = true;
	}
}
#pragma endregion
