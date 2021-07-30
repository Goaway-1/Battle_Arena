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

	//Dodge
	DodgeSpeed = 7000.f;
	bCanDodge = true;
	DodgeCoolDownTime = 1.f;
	DodgeStopTime = 0.1f;
	DirX = 0.f;
	DirY = 0.f;

#pragma endregion

#pragma region ATTACK
	bLMBDown = false;
	bAttacking = false;
	bIsAttackCheck = false;
	ComboCnt = 0;
	ComboMaxCnt = 0;
#pragma endregion
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();
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

	//Dodge
	PlayerInputComponent->BindAction("Dodge", EInputEvent::IE_Pressed, this, &AMainPlayer::Dodge);

	//sprinting
	PlayerInputComponent->BindAction("Shift", EInputEvent::IE_Pressed, this, &AMainPlayer::Switch_Sprinting);
	PlayerInputComponent->BindAction("Shift", EInputEvent::IE_Released, this, &AMainPlayer::Switch_Sprinting);

	//attack
	PlayerInputComponent->BindAction("LMB", EInputEvent::IE_Pressed, this, &AMainPlayer::LMBDown);
	PlayerInputComponent->BindAction("LMB", EInputEvent::IE_Released, this, &AMainPlayer::LMBUp);

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
	DirX = Value;
}
void AMainPlayer::MoveRight(float Value) {
	AddMovementInput(Camera->GetRightVector(), Value);
	DirY = Value;
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

void AMainPlayer::Dodge() {
	if (bCanDodge && DirX !=0 || DirY != 0) {
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;	//뭐에 닿아도 안느려짐
		AnimDodge();
		LaunchCharacter(FVector(GetLastMovementInputVector().X , GetLastMovementInputVector().Y, 0.f) * DodgeSpeed, true, true);	//입력 방향대로
		GetWorldTimerManager().SetTimer(DodgeHandle, this, &AMainPlayer::DodgeEnd, DodgeStopTime,false);
		bCanDodge = false;
	}
}
void AMainPlayer::DodgeEnd() {
	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(DodgeHandle, this, &AMainPlayer::ResetDodge, DodgeCoolDownTime, false);
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
}
void AMainPlayer::ResetDodge() {
	bCanDodge = true;
}
void AMainPlayer::AnimDodge() {
	int Value = 0;

	if (DirX > 0) Value = 1;
	else if (DirX < 0)  Value = 4;
	else if (DirY < 0)  Value = 2;
	else if (DirY > 0)  Value = 3;

	if(!AnimInstance) AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DodgeMontage) {
		AnimInstance->Montage_Play(DodgeMontage);
		AnimInstance->Montage_JumpToSection(GetAttackMontageSection("Dodge", Value), DodgeMontage);
	}
}
#pragma endregion

#pragma region ATTACK

void AMainPlayer::LMBDown() {
	bLMBDown = true;

	if (!bAttacking) Attack();
	else bIsAttackCheck = true;
}

void AMainPlayer::Attack() {

	bAttacking = true;
	if (!AnimInstance) AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage) {
		if (!AnimInstance->Montage_IsPlaying(AttackMontage)) {	//공격중이 아닐때 (처음 공격)
			ComboCnt = 0;
			AnimInstance->Montage_Play(AttackMontage);
		}
		else {													//공격중일때
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(GetAttackMontageSection("Attack", ComboCnt), AttackMontage);
		}
	}
}

void AMainPlayer::EndAttack() {
	bAttacking = false;
}

void AMainPlayer::AttackInputCheck() {
	if (bIsAttackCheck) {
		ComboCnt++;
		if (ComboCnt >= ComboMaxCnt) ComboCnt = 0;
		bIsAttackCheck = false;
		Attack();
	}
}

FName AMainPlayer::GetAttackMontageSection(FString Type, int32 Section) {
	if (Type == "Attack") return FName(*FString::Printf(TEXT("Attack%d"), Section));
	else if (Type == "Dodge") return FName(*FString::Printf(TEXT("Dodge%d"), Section));
	else return "Error";
}

#pragma endregion
