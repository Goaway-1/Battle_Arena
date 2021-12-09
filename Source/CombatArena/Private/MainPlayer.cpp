#include "MainPlayer.h"
#include "AttackWeapon.h"
#include "ShieldWeapon.h"
#include "Enemy.h"
#include "MainController.h"
#include "HealthWidget.h"
#include "PlayerAttackFunction.h"
#include "Potion.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"	
#include "PlayerSaveGame.h"	
#include "Components/DecalComponent.h"
#include "PlayerSkillFunction.h"
#include "BowWeapon.h"
#include "Grenade.h"
#include "Balance.h"

AMainPlayer::AMainPlayer()
{
 	PrimaryActorTick.bCanEverTick = true;

#pragma region AIPERCETION
	AIPerceptionSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionSource"));
#pragma endregion
#pragma region CAMERA
	/** SpringArms */
	SpringArmSence = CreateDefaultSubobject<USceneComponent>(TEXT("SpringArmSence"));
	SpringArmSence->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SringArm"));
	SetArms(SpringArm);
	SpringArm->TargetArmLength = 350.f;

	SpringArm_Sprinting = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_Sprinting"));
	SetArms(SpringArm_Sprinting);

	SpringArm_Attacking = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_Attacking"));
	SetArms(SpringArm_Attacking);

	SpringArm_Drawing = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_Drawing"));
	SetArms(SpringArm_Drawing);

	SpringArm_Skilling = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_Skilling")); 
	SetArms(SpringArm_Skilling);

	/** Camera Setting */
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	//회전해도 플레이어에게 영향 X
	bUseControllerRotationYaw = false; 
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

#pragma endregion
#pragma	region	MOVEMENT
	GetCharacterMovement()->bUseControllerDesiredRotation = false; // on&off	
	GetCharacterMovement()->bOrientRotationToMovement = false;	//이동방향 회전
	GetCharacterMovement()->RotationRate = FRotator(0.f,150.f,0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;	//변하지 않아!
	GetCharacterMovement()->AirControl = 0.5f;

	MovementStatus = EMovementStatus::EMS_Normal;
	BowSpeed = 200.f;
	MoveSpeed = 500.f;
	SprintingSpeed = 750.f;

	//Dodge
	DodgeSpeed = 5000.f;
	bCanDodge = true;
	DodgeCoolDownTime = 1.f;
	DirX = 0.f;
	DirY = 0.f;

#pragma endregion
#pragma region ATTACK
	AttackFunction = CreateDefaultSubobject<UPlayerAttackFunction>(TEXT("AttackFunction"));
	SetWeaponStatus(EWeaponStatus::EWS_Normal);
	SetCombatStatus(ECombatStatus::ECS_Normal);

	bLMBDown = false;
	bAttacking = false;
	bIsAttackCheck = false;
	ComboCnt = 0;
	ComboMaxCnt = 0;	
	DefaultAttackRange = 80.f;
	AttackRange = DefaultAttackRange;
	bTargeting = false;
	DefaultDamage = 5.0f;
	AttackDamage = DefaultDamage;

#pragma endregion
#pragma region BALANCE
	Balance = CreateDefaultSubobject<UBalance>("Balance");
	DecreaseBalanceTime = 1.0f;
	bCanSpecialAttack = false;

	//test
	EnemyBalanceOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyBalanceOverlap"));
	EnemyBalanceOverlap->SetupAttachment(GetRootComponent());
	EnemyBalanceOverlap->SetSphereRadius(200.f);
	EnemyBalanceOverlap->SetVisibility(true);

	EnemyBalanceOverlap->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::OnEnemyBalance_OverlapBegin);
	EnemyBalanceOverlap->OnComponentEndOverlap.AddDynamic(this, &AMainPlayer::OnEnemyBalance_OverlapEnd);
#pragma endregion
#pragma region SKILL
	SkillFunction = CreateDefaultSubobject<UPlayerSkillFunction>("SkillFunction");
#pragma endregion
#pragma region HEALTH
	//Health
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	//Stamina
	MaxStamina = 100.f;
	CurrentStamina = MaxStamina;
	CoolDownStamina = 30.f;
	CoolUpStamina = 10.f;
#pragma endregion
#pragma region ACTIVE
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));	//콜리전 설정

	ActiveOverlappingItem = nullptr;
	CurrentShieldWeapon = nullptr;
	CurrentAttackWeapon = nullptr;
#pragma endregion
#pragma region HUD
	EnemyHUDOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyHUDOverlap"));
	EnemyHUDOverlap->SetupAttachment(GetRootComponent());
	EnemyHUDOverlap->SetSphereRadius(700.f);

	EnemyHUDOverlap->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::OnEnemyHUD_OverlapBegin);
	EnemyHUDOverlap->OnComponentEndOverlap.AddDynamic(this, &AMainPlayer::OnEnemyHUD_OverlapEnd);

	CombatTarget = nullptr;

	bESCDown = false;
#pragma endregion
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();
	SetMovementStatus(EMovementStatus::EMS_Normal);
	SetHealthRatio();
	
	/** SkillFunction Initial */
	SkillFunction->SetInitial(GetController()->GetPawn(),GetMesh(),GetController(),this);
}

void AMainPlayer::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	PlayerController = Cast<AMainController>(GetController());
	AttackFunction->SetOwner(GetMesh(),PlayerController);
	CameraManager = PlayerController->PlayerCameraManager;	

	//카메라 각도 조정.
	CameraManager->ViewPitchMax = 50.f;
	CameraManager->ViewPitchMin = -70.f;
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** if Run and then Shake Cam*/
	RunCamShake();

	/** Targeting Check */
	Targeting();

	/** Set StaminaRatio Widget */
	SetStaminaRatio();

	/** Anim Charging */
	BowAnimCharge();
	CurrentHealth = 100.f;
	
	/** Player와 Enemy의 Balance Ratio */
	SetBalanceRatio();

	SetEnemyBalanceRatio();	
	
	/** Active Special Attack */
	CanEnemyBalance();
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

	//SetTargeting
	PlayerInputComponent->BindAction("Tab", EInputEvent::IE_Pressed, this, &AMainPlayer::SetTargeting);

	//sprinting
	PlayerInputComponent->BindAction("Shift", EInputEvent::IE_Pressed, this, &AMainPlayer::OnSprinting);
	PlayerInputComponent->BindAction("Shift", EInputEvent::IE_Released, this, &AMainPlayer::OffSprinting);

	//attack
	PlayerInputComponent->BindAction("LMB", EInputEvent::IE_Pressed, this, &AMainPlayer::LMBDown);
	PlayerInputComponent->BindAction("LMB", EInputEvent::IE_Released, this, &AMainPlayer::LMBUp);

	//Block
	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Pressed, this, &AMainPlayer::Blocking);
	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Released, this, &AMainPlayer::UnBlocking);

	//Kick
	PlayerInputComponent->BindAction("Kick", EInputEvent::IE_Pressed, this, &AMainPlayer::Kick);

	//Active
	PlayerInputComponent->BindAction("Active", EInputEvent::IE_Pressed, this, &AMainPlayer::ActiveInteraction);
	PlayerInputComponent->BindAction("DeActive", EInputEvent::IE_Pressed, this, &AMainPlayer::DeactiveInteraction);	

	/** Skill Test Input */
	PlayerInputComponent->BindAction("SkillTest", EInputEvent::IE_Pressed, this, &AMainPlayer::SkillController);

	/** Pause Menu */
	PlayerInputComponent->BindAction("Quit", EInputEvent::IE_Pressed, this, &AMainPlayer::ESCDown);
	PlayerInputComponent->BindAction("Quit", EInputEvent::IE_Released, this, &AMainPlayer::ESCUp);

	/** Charge & Fire */
	PlayerInputComponent->BindAction("Charge",EInputEvent::IE_Pressed,this, &AMainPlayer::BeginCharge);
	PlayerInputComponent->BindAction("Charge",EInputEvent::IE_Released,this, &AMainPlayer::EndCharge);

	/** Throwing */
	PlayerInputComponent->BindAction("Throwing", EInputEvent::IE_Pressed, this, &AMainPlayer::Throw);

	/** Powerful Attack */
	PlayerInputComponent->BindAction("Alt", EInputEvent::IE_Pressed, this, &AMainPlayer::AltDown);
	PlayerInputComponent->BindAction("Alt", EInputEvent::IE_Released, this, &AMainPlayer::AltUp);
}

#pragma region CAMERA
void AMainPlayer::Lookup(float value) {
	if (GetMovementStatus() == EMovementStatus::EMS_Faint) return;
	AddControllerYawInput(value * CameraSpeed * GetWorld()->GetDeltaSeconds());
	TurnInPlace(value);
}
void AMainPlayer::Turn(float value) {
	if (GetMovementStatus() == EMovementStatus::EMS_Faint) return;
	AddControllerPitchInput(value * CameraSpeed * GetWorld()->GetDeltaSeconds());
}
void AMainPlayer::TurnInPlace(float value) {
	FVector ViewPoint;
	FRotator ViewRotation;
	float calculationY;
	
	PlayerController->GetPlayerViewPoint(ViewPoint, ViewRotation);
	calculationY = UKismetMathLibrary::Abs(ViewRotation.Yaw - GetActorRotation().Yaw);

	if (calculationY <= 0.5f) {
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		TurnAxis = 0;
	}
	else if (calculationY >= 45.f || (DirX != 0 || DirY != 0)) {
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		if (GetVelocity().Size() == 0) {
			if (value > 0.1f) TurnAxis = 1;
			else if (value < -0.1f) TurnAxis = -1;
		}
	}
}
void AMainPlayer::SetArms(USpringArmComponent* Arm) {
	Arm->SetupAttachment(SpringArmSence);
	Arm->bUsePawnControlRotation = true;
	Arm->bInheritPitch = true;
	Arm->bInheritRoll = true;
	Arm->bInheritYaw = true;
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
	if (MovementStatus == EMovementStatus::EMS_Sprinting) GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	else if(MovementStatus == EMovementStatus::EMS_Drawing) GetCharacterMovement()->MaxWalkSpeed = BowSpeed;
	else GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}
void AMainPlayer::OnSprinting() {
	if (!IsCanMove() || GetStaminaRatio() <= 0.3f) return;
	if (MovementStatus != EMovementStatus::EMS_Sprinting && DirX != 0 || DirY != 0) {
		SetMovementStatus(EMovementStatus::EMS_Sprinting);
		ZoomInCam(SpringArm_Sprinting);
	}
}
void AMainPlayer::OffSprinting() {
	if (!IsCanMove()) return;
	if (MovementStatus != EMovementStatus::EMS_Walk){
		if (GetVelocity().Size() == 0) SetMovementStatus(EMovementStatus::EMS_Normal);
		else  SetMovementStatus(EMovementStatus::EMS_Walk);
		ZoomOutCam();
	}
}
void AMainPlayer::Dodge() {
	if (!IsCanMove()) return;
	if (DirX !=0 || DirY != 0) {
		AnimDodge();		
		bCanDodge = false;
		GetWorldTimerManager().SetTimer(DodgeHandle, this, &AMainPlayer::DodgeEnd, DodgeCoolDownTime, false);
	}
}
void AMainPlayer::DodgeEnd() {
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
	if (bAttacking || AttackFunction->bKicking || GetMovementStatus() == EMovementStatus::EMS_Death 
	|| GetMovementStatus() == EMovementStatus::EMS_Faint || !bCanDodge || !GetCharacterMovement()->CurrentFloor.IsWalkableFloor()) return false;
	else return true;
}
void AMainPlayer::Targeting() {
	if (bTargeting && CombatTarget != nullptr) {
		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CombatTarget->GetActorLocation());
		TargetRot.Pitch -= 30.f;

		Controller->SetControlRotation(FMath::RInterpTo(GetControlRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 5.0f));
	}
}
void AMainPlayer::SetTargeting() {
	if (bTargeting) OffTargeting();
	else OnTargeting();
}
void AMainPlayer::OnTargeting() {
	if (CombatTarget != nullptr) {
		bTargeting = true;
		bUseControllerRotationYaw = true;
		CombatTarget->ShowEnemyTarget();
	}
}
void AMainPlayer::OffTargeting() {
	if (CombatTarget != nullptr) {
		bTargeting = false;
		bUseControllerRotationYaw = false;
		CombatTarget->HideEnemyTarget();
	}
}
void AMainPlayer::ZoomInCam(USpringArmComponent* Arm,FRotator Rot) {
	Camera->AttachToComponent(Arm, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(Camera, FVector(0.f), Rot,false,false,0.3f, true,EMoveComponentAction::Type::Move,LatentInfo);
}
void AMainPlayer::ZoomOutCam() {
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(Camera, FVector(0.f), FRotator(0.f), false, false, 0.4f, true, EMoveComponentAction::Type::Move, LatentInfo);
}
void AMainPlayer::RunCamShake() {
	if (GetMovementStatus() == EMovementStatus::EMS_Sprinting) {
		CameraManager->StartCameraShake(RunShake, 1.f);
	}
}
#pragma endregion

#pragma region ATTACK
void AMainPlayer::LMBDown() {
	bLMBDown = true;
	
	//Action
	if (GetMovementStatus() == EMovementStatus::EMS_Throwing) {
		Throwing();
	}
	else if (GetAttackCurrentWeapon() != nullptr && GetAttackCurrentWeapon()->GetWeaponPos() == EWeaponPos::EWP_Bow) {
		Bow = Cast<ABowWeapon>(CurrentAttackWeapon);
		if (Bow && ChargeAmount >= 0.7f) {
			Bow->Fire(); 
			bBowCharging = false;
			EndCharge();
		}
	}
	else if (!bAttacking) Attack();
	else bIsAttackCheck = true;
}	
void AMainPlayer::Attack(bool bIsSpecial) {
	UAnimMontage* PlayMontage = nullptr;
	bAttacking = true;
	
	/** Set Montage */
	if(SkillFunction->bGround) PlayMontage = SkillAttackMontage;
	else if (GetAttackCurrentWeapon() == nullptr) PlayMontage = AttackMontage;
	else {	
		AAttackWeapon* Weapon = Cast<AAttackWeapon>(GetAttackCurrentWeapon());
		if (Weapon) {
			switch (Weapon->GetWeaponName()) {
				case EWeaponName::EWN_Sword :
					PlayMontage = SwordAttackMontage;
					break;
				case EWeaponName::EWN_Mace:
					PlayMontage = MaceAttackMontage;
					break;
				case EWeaponName::EWN_Spear:
					PlayMontage = SpearAttackMontage;
					break;
				default:
					break;
			}
		}
	}
	
	if (!AnimInstance) AnimInstance = GetMesh()->GetAnimInstance();

	if (bIsSpecial) {
		AnimInstance->Montage_Play(PlayMontage);
		AnimInstance->Montage_JumpToSection("SpecialAttack", PlayMontage);
		ZoomInCam(SpringArm_Attacking, FRotator(0.f, -60.f, 0.f));
	}
	else if (SkillFunction->bGround && PlayMontage) /** USEING SKILL */
	{
		AnimInstance->Montage_Play(PlayMontage);
		AnimInstance->Montage_JumpToSection("SkillEmpact", PlayMontage);

		SkillFunction->ConfirmTargetAndContinue();
		SkillEnd();
	}
	else if (bAltPressed && AnimInstance && PlayMontage)		/** Powerful ATTACK */
	{
		AnimInstance->Montage_Play(PlayMontage);
		AnimInstance->Montage_JumpToSection("PowerfulAttack", PlayMontage);
	}
	else if (AnimInstance && PlayMontage)		/** NOMAL ATTACK */
	{ 
		if (!AnimInstance->Montage_IsPlaying(PlayMontage)) {	//공격중이 아닐때 (처음 공격)
			AnimInstance->Montage_Play(PlayMontage);
			ComboCnt = 0;
		}
		else {													//공격중일때 2타 3타
			AnimInstance->Montage_Play(PlayMontage);
			AnimInstance->Montage_JumpToSection(GetAttackMontageSection("Attack", ComboCnt), PlayMontage);

			/** Combo Event */
			//if (ComboCnt == 2) {
			//	ZoomInCam(SpringArm_Attacking,FRotator(0.f,-30.f,0.f));
			//}
		}
	}
}
void AMainPlayer::StartAttack() {
	//그 범위 공격으로 빠질듯
	if(GetAttackCurrentWeapon()->GetWeaponPos() == EWeaponPos::EWP_Melee){
	}

	/** Use AttackFunction */
	//FString Type = "Player";
	//AttackFunction->SkillAttackStart(GetActorLocation(), GetActorForwardVector(), PlayerDamageType, Type, GetHitParticle(), GetAttackRange(), AttackDamage);
}
void AMainPlayer::EndAttack() {
	//그 범위 공격으로 빠질듯


	/** Use AttackFunction */
	bAttacking = false;
	ZoomOutCam();
}
void AMainPlayer::OnWeaponCollision() {
	if (GetAttackCurrentWeapon()->GetWeaponPos() == EWeaponPos::EWP_Melee) {
		AAttackWeapon* Weapon = Cast<AAttackWeapon>(GetAttackCurrentWeapon());
		Weapon->SetAttackCollision(true);
	}
}
void AMainPlayer::OffWeaponCollision() {
	if (GetAttackCurrentWeapon()->GetWeaponPos() == EWeaponPos::EWP_Melee) {
		AAttackWeapon* Weapon = Cast<AAttackWeapon>(GetAttackCurrentWeapon());
		Weapon->SetAttackCollision(false);
	}
}
void AMainPlayer::StartPowerfulAttack() {
	FString Type = "Player";
	AttackFunction->SkillAttackStart(GetActorLocation(), GetActorForwardVector(), PlayerDamageType, Type, GetHitParticle(), GetAttackRange(), AttackDamage * 2);
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
float AMainPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (LastAttack != CurrentAttack) LastAttack = CurrentAttack;
	else if (DamageEvent.DamageTypeClass != InternalDamageType) return 0;

	if(GetMovementStatus() == EMovementStatus::EMS_Faint) RecoverBalance();

	/** Active Shield */
	if (GetCombatStatus() == ECombatStatus::ECS_Blocking) {
		if (IsBlockingSuccess(DamageCauser)) {
			Balance->SetCurrentBalance(10.f);
			return 0;
		}
	}

	/** Health */
	if (CurrentHealth <= 0) return 0.f;
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0) {
		CurrentHealth = 0;
		Death();
	}
	SetHealthRatio();

	/** Balance */
	Balance->SetCurrentBalance(20.f);
	Balance->SetDecreaseBalance(false);
	if (Balance->GetCurrentBalance() >= 100.f) BrokenBalance();
	else GetWorldTimerManager().SetTimer(BalanceHandle, FTimerDelegate::CreateLambda([&] { Balance->SetDecreaseBalance(true);}), DecreaseBalanceTime, false);
	SetBalanceRatio();

	/** KnockBack */
	FVector Loc = DamageCauser->GetActorForwardVector();
	Loc.Z = 0;
	LaunchCharacter(Loc * 500.f, true, true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorLocation(), FRotator(0.f));

	/** CameraShake */
	if (PlayerController) CameraManager->StartCameraShake(CamShake, 1.f);

	/** ShowDamageText */
	AttackFunction->SpawnDamageText(GetActorLocation(), DamageAmount, DamageTextWidget,GetController());

	return DamageAmount;
}
void AMainPlayer::SetHealthRatio() {
	HealthRatio = CurrentHealth / MaxHealth;
	PlayerController->SetPlayerHealth();
}
void AMainPlayer::SetStaminaRatio() {
	if (GetMovementStatus() == EMovementStatus::EMS_Sprinting) {
		CurrentStamina -= GetWorld()->GetDeltaSeconds() * CoolDownStamina;
		if (CurrentStamina <= 0.f) {
			CurrentStamina = 0;
			OffSprinting();
		}
	}
	else {
		if (CurrentStamina >= MaxStamina) CurrentStamina = MaxStamina;
		else CurrentStamina += GetWorld()->GetDeltaSeconds() * CoolUpStamina;
	}
	StaminaRatio = CurrentStamina / MaxStamina;
	PlayerController->SetPlayerStamina();
}
void AMainPlayer::Kick() {
	AttackFunction->Kick(AnimInstance, AttackMontage);
}
void AMainPlayer::KickStart() {
	AttackFunction->KickStart(GetActorLocation(),GetActorForwardVector());
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
void AMainPlayer::Blocking() {
	if (!IsCanMove()) return;
	if (CurrentShieldWeapon != nullptr) {
		SetCombatStatus(ECombatStatus::ECS_Blocking);
	}
}
void AMainPlayer::UnBlocking() {
	if (CurrentShieldWeapon != nullptr) {
		SetCombatStatus(ECombatStatus::ECS_Normal);
	}
}
bool AMainPlayer::IsBlockingSuccess(AActor* DamageCauser) {
	FRotator BetweenRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageCauser->GetActorLocation());
	BetweenRot = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), BetweenRot);
	bool isShieldRange = UKismetMathLibrary::InRange_FloatFloat(BetweenRot.Yaw, CurrentShieldWeapon->ShiledMinAngle, CurrentShieldWeapon->ShiledMaxAngle);

	if (isShieldRange && CurrentShieldWeapon->HitedParticle) {
		FVector Loc = GetActorForwardVector();
		Loc.Z = 0;
		LaunchCharacter(DamageCauser->GetActorForwardVector() * 500.f, true, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentShieldWeapon->HitedParticle, GetActorLocation(), FRotator(0.f));
		return true;
	}
	return false;
}
void AMainPlayer::BeginCharge() {
	if (GetAttackCurrentWeapon() != nullptr && GetAttackCurrentWeapon()->GetWeaponPos() == EWeaponPos::EWP_Bow) {
		Bow = Cast<ABowWeapon>(CurrentAttackWeapon);
		if (Bow) {
			Bow->Reload();
			Bow->BeginCharge();
			bBowCharging = true;
			SetMovementStatus(EMovementStatus::EMS_Drawing);
			ZoomInCam(SpringArm_Drawing, FRotator(0.f,-20.f,0.f));
		}
	}
}
void AMainPlayer::EndCharge() {
	if (GetAttackCurrentWeapon() != nullptr && GetAttackCurrentWeapon()->GetWeaponPos() == EWeaponPos::EWP_Bow) {
		Bow = Cast<ABowWeapon>(CurrentAttackWeapon);
		if (Bow) {
			Bow->EndCharge();
			/** 1. 쐇는데 당기고 있을 경우 */
			if(!bBowCharging && (MovementStatus == EMovementStatus::EMS_Drawing)) {
				ChargeAmount = 0;
				BeginCharge();
			}
			/** 2. 당기다가 놨을 경우 */
			else {
				SetMovementStatus(EMovementStatus::EMS_Normal);
				bBowCharging = false;
				ZoomOutCam();
			}
		}
	}
}
void AMainPlayer::BowAnimCharge() {
	if (Bow && bBowCharging) ChargeAmount = Bow->ChargeAmount;
}
#pragma endregion

#pragma region BALANCE
void AMainPlayer::SetBalanceRatio() {
	if (Balance->GetCurrentBalance() > 0.f) {
		PlayerController->SetPlayerBalance();
	}
}
void AMainPlayer::SetEnemyBalanceRatio() {
	PlayerController->SetEnemyBalance();
}
void AMainPlayer::BrokenBalance() {
	Balance->SetCurrentBalance(-100.f);
	SetMovementStatus(EMovementStatus::EMS_Faint); 
	GetWorldTimerManager().ClearTimer(BalanceHandle);
	GetWorldTimerManager().SetTimer(BalanceHandle, this, &AMainPlayer::RecoverBalance , 1.5f, false);
	
	/** Play Animation */
	if(!FaintMontage) return;
	AnimInstance->Montage_Play(FaintMontage);
	AnimInstance->Montage_JumpToSection("Faint", FaintMontage);
}
void AMainPlayer::RecoverBalance() {
	UE_LOG(LogTemp, Warning, TEXT("Player Recover"));
	if(GetMovementStatus() != EMovementStatus::EMS_Faint) return;
	AnimInstance->Montage_Stop(0.1f);
	SetMovementStatus(EMovementStatus::EMS_Normal);
}
void AMainPlayer::OnEnemyBalance_OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		BalanceTarget = Cast<AEnemy>(OtherActor);
	}
}
void AMainPlayer::OnEnemyBalance_OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		AEnemy* EnemyTest = Cast<AEnemy>(OtherActor);
		if (EnemyTest && BalanceTarget) BalanceTarget = nullptr;
	}
}
void AMainPlayer::CanEnemyBalance() {
	if(BalanceTarget == nullptr) bCanSpecialAttack = false;
	if (BalanceTarget) {
		if (BalanceTarget->GetIsFainted() && !bCanSpecialAttack) bCanSpecialAttack = true;
		else if(!BalanceTarget->GetIsFainted()) bCanSpecialAttack = false;
	}
}
#pragma endregion

#pragma region SKILL
void AMainPlayer::SkillController() {
	if(GetCombatStatus() != ECombatStatus::ECS_Skilling) {
		if(bTargeting) OffTargeting();
		SkillBegin();
	}
	else SkillEnd();
}
void AMainPlayer::SkillBegin() {
	SetCombatStatus(ECombatStatus::ECS_Skilling);

	/** Set Player View */
	ZoomInCam(SpringArm_Skilling, FRotator(-20.f, 0.f, 0.f));

	/** Use Skill */
	//SkillFunction->LazerAttack();	
	SkillFunction->GroundAttack();
}
void AMainPlayer::SkillEnd() {
	SetCombatStatus(ECombatStatus::ECS_Normal);

	/** Set Player View */
	ZoomOutCam();

	/** Use Skill */
	//SkillFunction->LazerEnd();		
	SkillFunction->GroundAttack();
}
#pragma endregion

#pragma region THROW
void AMainPlayer::Throw() {
	(GetMovementStatus() == EMovementStatus::EMS_Throwing) ? EndThrow() : StartThrow();
}
void AMainPlayer::StartThrow() {
	if(!GrenadeClass) return;

	SetMovementStatus(EMovementStatus::EMS_Throwing);
	bisThrow = false;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	Grenade = GetWorld()->SpawnActor<AGrenade>(GrenadeClass,FVector(0.f) , FRotator(0.f), SpawnParams);
	Grenade->SetActorScale3D(FVector(0.1f));
	Grenade->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName("Throw"));
}
void AMainPlayer::Throwing() {
	bisThrow = true; 
	GetWorldTimerManager().SetTimer(ThrowTimer, this, &AMainPlayer::EndThrow, 0.3f, false);
}
void AMainPlayer::EndThrow() {
	if (GetMovementStatus() != EMovementStatus::EMS_Throwing) return;
	Grenade->SetFire(GetActorRotation());
	Grenade = nullptr;
	bisThrow = false;
	SetMovementStatus(EMovementStatus::EMS_Normal);
}
#pragma endregion

#pragma region ACTIVE
void AMainPlayer::ActiveInteraction() {
	/** Active SpecialAttack */
	if(bCanSpecialAttack) ActiveSpecialAttack();

	/** Item or Weapon */
	else if (ActiveOverlappingItem != nullptr) ItemEquip();
}

void AMainPlayer::DeactiveInteraction() {

	/** Weapon */
	if (GetWeaponStatus() != EWeaponStatus::EWS_Normal) ItemDrop();
}
void AMainPlayer::ActiveSpecialAttack() {
	UE_LOG(LogTemp, Warning, TEXT("ActiveSpecialAttack"));

	if (!BalanceTarget) {
		UE_LOG(LogTemp, Warning, TEXT("MainPlayer :: ActiveSpecialAttack Error!"));
		return;
	}
	BalanceTarget->SpecialHit();
	Attack(true);
	//Enemy는 그냥 데미지 받도록 설정.
}
void AMainPlayer::ItemEquip() {	
	if (ActiveOverlappingItem->GetItemType() == EItemType::EIT_Weapon) {
		AWeapon* CurWeapon = Cast<AWeapon>(ActiveOverlappingItem);
		CurWeapon->Equip(this);

		AAttackWeapon* ACurWeapon = Cast<AAttackWeapon>(CurWeapon);
		if (ACurWeapon) ACurWeapon->SetAttackInit(PlayerController, this, PlayerDamageType);
	}
	else if (ActiveOverlappingItem->GetItemType() == EItemType::EIT_Item) {
		APotion* Potion = Cast<APotion>(ActiveOverlappingItem);
		CurrentHealth = Potion->UseItem(CurrentHealth);
		SetHealthRatio();
	}
	SetActiveOverlappingItem(nullptr);
}
void AMainPlayer::ItemDrop() {
	if (CurrentAttackWeapon != nullptr) {
		CurrentAttackWeapon->UnEquip();
		CurrentAttackWeapon = nullptr;
		AttackDamage = DefaultDamage;
		AttackRange = DefaultAttackRange;				//공격 범위 조절
		if (CurrentShieldWeapon == nullptr) SetWeaponStatus(EWeaponStatus::EWS_Normal);
	}
	else if (CurrentShieldWeapon != nullptr) {
		CurrentShieldWeapon->UnEquip();
		CurrentShieldWeapon = nullptr;
		if (CurrentAttackWeapon == nullptr) SetWeaponStatus(EWeaponStatus::EWS_Normal);
	}
}
void AMainPlayer::SetShieldCurrentWeapon(AShieldWeapon* Weapon) {
	CurrentShieldWeapon = Weapon;
}
void AMainPlayer::SetAttackCurrentWeapon(AWeapon* Weapon) {
	CurrentAttackWeapon = Weapon;
}
#pragma endregion

#pragma region HUD
void AMainPlayer::OnEnemyHUD_OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		CombatTarget = Cast<AEnemy>(OtherActor);
		if(CombatTarget) CombatTarget->ShowEnemyHUD();
	}
}
void AMainPlayer::OnEnemyHUD_OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		CombatTarget = Cast<AEnemy>(OtherActor);
		if (CombatTarget) {
			CombatTarget->HideEnemyHUD();
			OffTargeting();
			CombatTarget = nullptr;
		}
	}
}
void AMainPlayer::ESCUp() {
	bESCDown = false;
}
void AMainPlayer::ESCDown(){
	bESCDown = true;
	if (PlayerController) {
		PlayerController->TogglePauseMenu();
	}
}
void AMainPlayer::SetFogSplatter() {
	(PlayerController->GetFogSplatterVisible()) ? PlayerController->RemoveFogSplatter() : PlayerController->DisplayFogSplatter();
}
#pragma endregion

#pragma region SAVE&LOAD
void AMainPlayer::SaveData() {
	UPlayerSaveGame* SaveGameInstance = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.Health = CurrentHealth;
	SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
	SaveGameInstance->CharacterStats.Stamina	= CurrentStamina;
	SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
	SaveGameInstance->CharacterStats.Location = GetActorLocation();

	UGameplayStatics::SaveGameToSlot(SaveGameInstance,SaveGameInstance->PlayerName,SaveGameInstance->UserIndex);
}

void AMainPlayer::LoadData() {
	UPlayerSaveGame* LoadGameInstance = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));

	LoadGameInstance = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName,LoadGameInstance->UserIndex));
	
	CurrentHealth = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	CurrentStamina = LoadGameInstance->CharacterStats.Stamina;
	MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
	SetActorLocation(LoadGameInstance->CharacterStats.Location);
}
#pragma endregion
