#include "Enemy.h"
#include "MainPlayer.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.h"
#include "Engine/SkeletalMeshSocket.h"
#include "EnemyAttackFunction.h"
#include "CollisionQueryParams.h"	
#include "TimerManager.h"
#include "Components/DecalComponent.h"
#include "DamageTextWidget.h"	
#include "Components/PrimitiveComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"

AEnemy::AEnemy()
{
#pragma region INIT
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));	
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
#pragma endregion
#pragma region HUD
	//HUD
	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);	//위치 고정
	HealthWidget->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//Targeting 
	TargetingDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("TargetingDecal"));
	TargetingDecal->SetupAttachment(GetMesh());
	TargetingDecal->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	TargetingDecal->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	TargetingDecal->DecalSize = FVector(500.f, 1500.f, 1500.f);
	TargetingDecal->SetVisibility(false);
#pragma endregion
#pragma region ATTCK
	AttackFunction = CreateDefaultSubobject<UEnemyAttackFunction>(TEXT("AttackFunction"));

	LeftWeapon = CreateDefaultSubobject<UCapsuleComponent>("LeftWeapon");
	LeftWeapon->SetupAttachment(GetMesh(), FName("weapon_l"));
	LeftWeapon->SetCollisionProfileName(FName("EnemyWeapon"));

	RightWeapon = CreateDefaultSubobject<UCapsuleComponent>("RightWeapon");
	RightWeapon->SetupAttachment(GetMesh(), FName("weapon_r"));
	RightWeapon->SetCollisionProfileName(FName("EnemyWeapon"));

	IsAttacking = false;
	bIsback = false;
	AttackDamage = 10.f;
	AttackRange = 430.f;	
	KnockBackPower = 800.f;
#pragma endregion
}

void AEnemy::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	EnemyController = Cast<AEnemyController>(NewController);
	AttackFunction->SetOwner(GetMesh(),EnemyController);

	RightWeapon->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnWeaponOverlap);
	LeftWeapon->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnWeaponOverlap);
	AttackEnd_Collision();
}
void AEnemy::BeginPlay(){
	Super::BeginPlay();

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	if (!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	
#pragma region HUD
	if (WEnemyHealth) {
		HealthWidget->SetWidgetClass(WEnemyHealth);
		HealthWidget->SetDrawSize(FVector2D(150.f, 20.f));
		HealthWidget->SetVisibility(false);
	}

	HealthBar = Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject());
	HealthBar->SetEnemyOwner(this);
	HealthBar->SetOwnerHealth(GetHealthRatio(), MaxHealth, CurrentHealth);
#pragma endregion
}
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	IsKnockBack();
}
void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
}
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

#pragma region ATTACK
void AEnemy::Attack(FString type) {
	if(!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
    Anim->StopAllMontages(0.f);
	
	if (!AttackMontage && !Anim) return;
	
	IsAttacking = true;
	if (type == "Melee") {
		Anim->Montage_Play(AttackMontage);
		Anim->Montage_JumpToSection(GetAttackMontageSection("Attack"), AttackMontage);
	}
}
void AEnemy::AttackReady() {
	LaunchCharacter(GetActorForwardVector() * 700.f, true, true);
}
void AEnemy::AttackStart_Internal() {
	FString Type = "Enemy";
	AttackFunction->SkillAttackStart(GetActorLocation(),GetActorForwardVector(),InternalDamageType, Type, GetHitParticle(),GetAttackRange(), AttackDamage, AttackCnt);

	AttackCnt++;
	if (AttackCnt > 2) AttackCnt = 0;
}
void AEnemy::AttackStart_Collision(bool value) {
	if (value) RightWeapon->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else LeftWeapon->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	AttackCnt++; 
	if (AttackCnt > 2) AttackCnt = 0;
}
void AEnemy::AttackEnd_Collision() {
	LeftWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	if (LastAttack != CurrentAttack) LastAttack = CurrentAttack;
	else if (DamageEvent.DamageTypeClass != InternalDamageType) return 0;
	else return 0;

	CurrentHealth -= DamageAmount;
	SetHealthRatio();
	HealthBar->SetOwnerHealth(GetHealthRatio(), MaxHealth, CurrentHealth);
	if(CurrentHealth <= 0) {
		CurrentHealth = 0;
		DeathEnd();
		return DamageAmount;
	}

	if (HitedMontage && Anim && !IsAttacking) {
		Anim->Montage_Play(HitedMontage);
		Anim->Montage_JumpToSection("Hited_F", DeathMontage);
	}
	GetAttackFuntion()->SpawnDamageText(GetActorLocation(), DamageAmount, DamageTextWidget, EventInstigator);
	return DamageAmount;
}
void AEnemy::DeathEnd() {
	Cast<AEnemyController>(GetController())->StopBeTree();	//비헤이비어 트리 정지 (내가 만듬)
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	if (DeathMontage && Anim) {
		Anim->Montage_Play(DeathMontage);
		Anim->Montage_JumpToSection("Death", DeathMontage);
	}
}
void AEnemy::DestroyEnemy() {
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	Destroy();
}
void AEnemy::KnockBack(FVector Backward) {
	if (!bIsback) {
		if (HitedMontage != nullptr) Anim->Montage_Play(HitedMontage);
		bIsback = true;
		BackVector = Backward;
		GetWorldTimerManager().SetTimer(backHandle, this, &AEnemy::knockBackEnd, 0.2f);
	}
}
void AEnemy::knockBackEnd() {
	bIsback = false;
}
void AEnemy::IsKnockBack() {
	if (bIsback) {
		AddActorWorldOffset(BackVector * KnockBackPower * GetWorld()->GetDeltaSeconds(), false);
	}
}
void AEnemy::LaunchSky(FVector Pos) {
	LaunchCharacter(Pos,false,false);
}
FName AEnemy::GetAttackMontageSection(FString Type) {
	if (Type == "Attack") {
		int range = FMath::RandRange(1, GetAttackTypeCnt());
		AttackDamage = (range == 1) ? 10.f : 20.f;
		return FName(*FString::Printf(TEXT("Attack%d"), range));
	}
	else if (Type == "Skill") {
		int range = FMath::RandRange(1, 4);
		switch (range) {
		case 1:
			SkillType = "Meteor";
			break;
		case 2:
			SkillType = "Lazer";
			break;
		case 3:
			SkillType = "Magic";
			break;
		case 4:
			SkillType = "Rush";
			break;
		default:
			SkillType = "Error";
			break;
		}
		return FName(*FString::Printf(TEXT("Attack%d"), range));
	}
	else return "Error";
}
void AEnemy::SetVisibleInFog(bool bisin, int time) {
	if (!EnemyController) return;
	EnemyController->SetVisibleInFog(bisin);
	SmokeTime = time + 2.f;
}
void AEnemy::ActiveFogEvent() {
	if (IsInFogMontage && Anim) Anim->Montage_Play(IsInFogMontage);
	GetWorldTimerManager().SetTimer(FogHandle, this, &AEnemy::DeactiveFogEvent, SmokeTime, false);
}
void AEnemy::DeactiveFogEvent() {
	Anim->Montage_Stop(0.1f);
	SetVisibleInFog(false);
}
void AEnemy::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if (Player) {
			Player->SetCurrentAttack(GetName() + FString::FromInt(AttackCnt));
			UGameplayStatics::ApplyDamage(Player, AttackDamage, EnemyController, GetController(), CollisionDamageType);
		}
	}
}
#pragma endregion
#pragma region HUD
void AEnemy::ShowEnemyTarget() {
	if (!TargetingDecal) return;
	TargetingDecal->SetVisibility(true);
}
void AEnemy::HideEnemyTarget() {
	if (!TargetingDecal) return;
	TargetingDecal->SetVisibility(false);
}
void AEnemy::ShowEnemyHUD() {
	if (!HealthWidget) return;
	HealthWidget->SetVisibility(true);
}
void AEnemy::HideEnemyHUD() {
	if (!HealthWidget) return;
	HealthWidget->SetVisibility(false);
}
void AEnemy::SetHealthRatio() {
	HealthRatio = CurrentHealth / MaxHealth;
}
#pragma endregion
#pragma region SOUND
void AEnemy::PlaySwingSound() {
	if (SwingSound) UGameplayStatics::SpawnSoundAtLocation(this, SwingSound, GetActorLocation());
}
void AEnemy::PlayHitedSound() { 
	if (HitedSound) UGameplayStatics::SpawnSoundAtLocation(this, HitedSound, GetActorLocation());
}
void AEnemy::PlayDeathSound() {
	if (DeathSound) UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
}
#pragma endregion