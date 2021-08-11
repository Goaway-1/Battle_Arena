#include "MainPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Weapon.h"
#include "Enemy.h"
#include "MainController.h"
#include "HealthWidget.h"

AMainPlayer::AMainPlayer()
{
 	PrimaryActorTick.bCanEverTick = true;

#pragma region AIPERCETION
	AIPerceptionSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionSource"));
#pragma endregion


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
	SetWeaponStatus(EWeaponStatus::EWS_Normal);

	bLMBDown = false;
	bAttacking = false;
	bIsAttackCheck = false;
	ComboCnt = 0;
	ComboMaxCnt = 0;
#pragma endregion

#pragma region HEALTH
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
#pragma endregion

#pragma region ACTIVE
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));	//콜리전 설정

	ActiveOverlappingItem = nullptr;
	CurrentWeapon = nullptr;
#pragma endregion

#pragma region HUD
	EnemyHUDOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyHUDOverlap"));
	EnemyHUDOverlap->SetupAttachment(GetRootComponent());
	EnemyHUDOverlap->SetSphereRadius(700.f);

	EnemyHUDOverlap->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::OnEnemyHUD_OverlapBegin);
	EnemyHUDOverlap->OnComponentEndOverlap.AddDynamic(this, &AMainPlayer::OnEnemyHUD_OverlapEnd);

	CombatTarget = nullptr;
#pragma endregion

}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();
	SetMovementStatus(EMovementStatus::EMS_Normal);
	SetHealthRatio();
}

void AMainPlayer::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	PlayerController = Cast<AMainController>(GetController());
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

	//Active
	PlayerInputComponent->BindAction("Equip", EInputEvent::IE_Pressed, this, &AMainPlayer::ItemEquip);
	PlayerInputComponent->BindAction("Drop", EInputEvent::IE_Pressed, this, &AMainPlayer::ItemDrop);

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
	if (!IsCanMove()) return;

	AddMovementInput(Camera->GetForwardVector(), Value);
	DirX = Value;
}
void AMainPlayer::MoveRight(float Value) {
	if (!IsCanMove()) return;

	AddMovementInput(Camera->GetRightVector(), Value);
	DirY = Value;
}
void AMainPlayer::Jump() {
	if (!IsCanMove()) return;
	
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
	if (!IsCanMove()) return;
	if (MovementStatus != EMovementStatus::EMS_Sprinting) SetMovementStatus(EMovementStatus::EMS_Sprinting);
	else {
		if(GetVelocity().Size() == 0) SetMovementStatus(EMovementStatus::EMS_Normal);
		else  SetMovementStatus(EMovementStatus::EMS_Walk);
	}
}
void AMainPlayer::CheckIdle() {
	float CurrentVelocity = GetVelocity().Size();
	if (!IsCanMove()) return;

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
	if (!IsCanMove()) return;
	if (bCanDodge && DirX !=0 || DirY != 0) {
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;	//마찰계수
		AnimDodge();
		LaunchCharacter(FVector(GetLastMovementInputVector().X , GetLastMovementInputVector().Y, 0.f) * DodgeSpeed, true, true);	//입력 방향대로
		GetWorldTimerManager().SetTimer(DodgeHandle, this, &AMainPlayer::DodgeEnd, DodgeStopTime,false);
		bCanDodge = false;
	}
}
void AMainPlayer::DodgeEnd() {
	GetCharacterMovement()->StopMovementImmediately();	//움직임을 정지시킨다.
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

bool AMainPlayer::IsCanMove() {
	if (bAttacking || GetMovementStatus() == EMovementStatus::EMS_Death) return false;
	else return true;
}
#pragma endregion

#pragma region ATTACK

void AMainPlayer::LMBDown() {
	bLMBDown = true;

	if (!bAttacking) Attack();
	else bIsAttackCheck = true;
}

void AMainPlayer::Attack() {
	UAnimMontage* PlayMontage = nullptr;

	if (GetWeaponStatus() == EWeaponStatus::EWS_Normal) PlayMontage = AttackMontage;
	else if (GetWeaponStatus() == EWeaponStatus::EWS_Weapon) PlayMontage = WeaponAttackMontage;

	bAttacking = true;
	if (!AnimInstance) AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && PlayMontage) {
		if (!AnimInstance->Montage_IsPlaying(PlayMontage)) {	//공격중이 아닐때 (처음 공격)
			ComboCnt = 0;
			AnimInstance->Montage_Play(PlayMontage);
		}
		else {													//공격중일때
			AnimInstance->Montage_Play(PlayMontage);
			AnimInstance->Montage_JumpToSection(GetAttackMontageSection("Attack", ComboCnt), PlayMontage);
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

void AMainPlayer::ActiveWeaponCollision() {
	GetCurrentWeapon()->ActiveOnCollision();
}
void AMainPlayer::DeActiveWeaponCollision() {
	GetCurrentWeapon()->DeActiveOnCollision();
}
float AMainPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHealth <= 0) return 0.f;

	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0) {
		CurrentHealth = 0;
		Death();
	}
	SetHealthRatio();

	return DamageAmount;
}
void AMainPlayer::SetHealthRatio() {
	HealthRatio = CurrentHealth / MaxHealth;
	PlayerController->SetPlayerHealth();
}

void AMainPlayer::Death() {
	SetMovementStatus(EMovementStatus::EMS_Death);

	if (DeathMontage && AnimInstance) {
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection("Death_1", DeathMontage);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainPlayer::DeathEnd() {
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	Destroy();
}
#pragma endregion

#pragma region ACTIVE
void AMainPlayer::ItemEquip() {
	if (ActiveOverlappingItem != nullptr) {
		AWeapon* CurWeapon = Cast<AWeapon>(ActiveOverlappingItem);

		if (nullptr != CurrentWeapon) ItemDrop();
		
		CurWeapon->Equip(this);
		SetActiveOverlappingItem(nullptr);
	}
}
void AMainPlayer::ItemDrop() {
	if (GetWeaponStatus() == EWeaponStatus::EWS_Weapon) {
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
		SetWeaponStatus(EWeaponStatus::EWS_Normal);
	}
}
#pragma endregion

#pragma region HUD

void AMainPlayer::OnEnemyHUD_OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		CombatTarget = Cast<AEnemy>(OtherActor);
		if (CombatTarget) {
			CombatTarget->ShowEnemyHealth();
		}
	}
}

void AMainPlayer::OnEnemyHUD_OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		CombatTarget = Cast<AEnemy>(OtherActor);
		if (CombatTarget) {
			CombatTarget->HideEnemyHealth();
			CombatTarget = nullptr;
		}
	}
}
#pragma endregion
