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
	SpringArm->SocketOffset = FVector(0.f, 0.f, 0.f);

	//LagSpeed
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 9.0f;				//이동
	SpringArm->CameraRotationLagSpeed = 9.5f;		//회전
	
	//카메라 회전 On
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;

	SpringArm->TargetArmLength = 350.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	//회전해도 플레이어에게 영향 X
	bUseControllerRotationYaw = false; 
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
	DodgeSpeed = 5000.f;
	bCanDodge = true;
	DodgeCoolDownTime = 1.f;
	DodgeStopTime = 0.1f;
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

#pragma region HEALTH
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
#pragma endregion

#pragma region ACTIVE
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));	//콜리전 설정

	ActiveOverlappingItem = nullptr;
	CurrentLeftWeapon = nullptr;
	CurrentRightWeapon = nullptr;
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
void AMainPlayer::OnSprinting() {
	if (!IsCanMove()) return;
	if (MovementStatus != EMovementStatus::EMS_Sprinting && DirX != 0 || DirY != 0) {
		SetMovementStatus(EMovementStatus::EMS_Sprinting);
		ZoomInCam(FVector(150.f, 0.f, 0.f));
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
	if (!bCanDodge) return;
	if (bCanDodge && DirX !=0 || DirY != 0) {
		//GetCharacterMovement()->BrakingFrictionFactor = 0.f;	//마찰계수
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
		if(bTargeting)AnimInstance->Montage_JumpToSection(GetAttackMontageSection("Dodge", Value), DodgeMontage);
	}
}
bool AMainPlayer::IsCanMove() {
	if (AttackFunction->bKicking || bAttacking || GetMovementStatus() == EMovementStatus::EMS_Death) return false;
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
void AMainPlayer::ZoomInCam(FVector Pos,FRotator Rot) {
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(Camera, Pos, Rot,false,false,0.3f, true,EMoveComponentAction::Type::Move,LatentInfo);
}
void AMainPlayer::ZoomOutCam() {
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

	if (!bAttacking) Attack();
	else bIsAttackCheck = true;
}
void AMainPlayer::Attack() {
	UAnimMontage* PlayMontage = nullptr;
	
	if (GetRightCurrentWeapon() == nullptr) PlayMontage = AttackMontage;
	else PlayMontage = WeaponAttackMontage;

	bAttacking = true;
	if (!AnimInstance) AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && PlayMontage) {
		if (!AnimInstance->Montage_IsPlaying(PlayMontage)) {	//공격중이 아닐때 (처음 공격)
			ComboCnt = 0;
			AnimInstance->Montage_Play(PlayMontage);
		}
		else {													//공격중일때 2타 3타
			AnimInstance->Montage_Play(PlayMontage);
			AnimInstance->Montage_JumpToSection(GetAttackMontageSection("Attack", ComboCnt), PlayMontage);

			/** Combo Event */
			if (ComboCnt == 2) {
				GetController()->SetInitialLocationAndRotation(FVector(0.f),GetActorRotation());
				ZoomInCam(FVector(300.f, 120.f, -30.f), FRotator(0.f, -40.f, 0.f));
			}
		}
	}
}
void AMainPlayer::StartAttack() {
	FString Type = "Player";
	AttackFunction->AttackStart(GetActorLocation(), GetActorForwardVector(), PlayerDamageType, Type, GetHitParticle(), GetAttackRange(), AttackDamage);
}
void AMainPlayer::EndAttack() {
	bAttacking = false;
	ZoomOutCam();
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

	// 방어 성공시 
	if (GetCombatStatus() == ECombatStatus::ECS_Blocking) {
		if (IsBlockingSuccess(DamageCauser)) return 0;
	}

	if (CurrentHealth <= 0) return 0.f;
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0) {
		CurrentHealth = 0;
		Death();
	}
	SetHealthRatio();

	/** KnockBack */
	FVector Loc = GetActorForwardVector();
	Loc.Z = 0;
	LaunchCharacter(GetActorForwardVector() * -2000.f, true, true);
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
	if (CurrentLeftWeapon != nullptr) {
		SetCombatStatus(ECombatStatus::ECS_Blocking);
	}
}
void AMainPlayer::UnBlocking() {
	if (CurrentLeftWeapon != nullptr) {
		SetCombatStatus(ECombatStatus::ECS_Normal);
	}
}
bool AMainPlayer::IsBlockingSuccess(AActor* DamageCauser) {
	FRotator BetweenRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageCauser->GetActorLocation());
	BetweenRot = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), BetweenRot);
	bool isShieldRange = UKismetMathLibrary::InRange_FloatFloat(BetweenRot.Yaw, CurrentLeftWeapon->ShiledMinAngle, CurrentLeftWeapon->ShiledMaxAngle);

	if (isShieldRange && CurrentLeftWeapon->HitedParticle) {
		FVector Loc = GetActorForwardVector();
		Loc.Z = 0;
		LaunchCharacter(Loc * -500.f, true, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentLeftWeapon->HitedParticle, GetActorLocation(), FRotator(0.f));
		return true;
	}
	return false;
}

#pragma endregion

#pragma region ACTIVE
void AMainPlayer::ItemEquip() {	
	if (ActiveOverlappingItem != nullptr) {
		if (ActiveOverlappingItem->GetItemType() == EItemType::EIT_Weapon) {
			AWeapon* CurWeapon = Cast<AWeapon>(ActiveOverlappingItem);
			CurWeapon->Equip(this);
		}
		else if(ActiveOverlappingItem->GetItemType() == EItemType::EIT_Item) {
			APotion* Potion = Cast<APotion>(ActiveOverlappingItem);
			CurrentHealth = Potion->UseItem(CurrentHealth);
			SetHealthRatio();
		}
		SetActiveOverlappingItem(nullptr);
	}
}
void AMainPlayer::ItemDrop() {
	if (GetWeaponStatus() == EWeaponStatus::EWS_Weapon) {
		if (CurrentRightWeapon != nullptr) {
			CurrentRightWeapon->UnEquip();
			CurrentRightWeapon = nullptr;		
			AttackDamage = DefaultDamage;	
			AttackRange = DefaultAttackRange;				//공격 범위 조절
			if (CurrentLeftWeapon == nullptr) SetWeaponStatus(EWeaponStatus::EWS_Normal);
		}
		else if (CurrentLeftWeapon != nullptr) {
			CurrentLeftWeapon->UnEquip();
			CurrentLeftWeapon = nullptr;
			if(CurrentRightWeapon == nullptr) SetWeaponStatus(EWeaponStatus::EWS_Normal);
		}
	}
}
void AMainPlayer::SetLeftCurrentWeapon(AShieldWeapon* Weapon) {
	CurrentLeftWeapon = Weapon;
}
void AMainPlayer::SetRightCurrentWeapon(AAttackWeapon* Weapon) {
	CurrentRightWeapon = Weapon;
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
//void AMainPlayer::SpawnDamageText(FVector WorldLocation, float Damage) {
//	WorldLocation.X += UKismetMathLibrary::RandomFloatInRange(-50.f,50.f);
//	WorldLocation.Y += UKismetMathLibrary::RandomFloatInRange(-50.f,50.f);
//	UGameplayStatics::ProjectWorldToScreen(PlayerController, WorldLocation, DamageTextVec);
//	DamageWidget = CreateWidget<UDamageTextWidget>(PlayerController, DamageTextWidget);
//	DamageWidget->InintialScreenLocation = DamageTextVec;
//	DamageWidget->DamageToDisplay = Damage;
//	DamageWidget->AddToViewport();
//}
#pragma endregion
