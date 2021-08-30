#include "Enemy.h"
#include "EnemyController.h"
#include "EnemyAnim.h"
#include "MainPlayer.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.h"
#include "Engine/SkeletalMeshSocket.h"
#include "EnemyAttackFunction.h"
#include "CollisionQueryParams.h"	
#include "TimerManager.h"
#include "Components/DecalComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));	//콜리전 설정

#pragma region HUD
	//Health
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	//HUD
	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);	//위치 고정
	HealthWidget->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//Targeting 
	TargetingDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("TargetingDecal"));
	TargetingDecal->SetupAttachment(GetMesh());
	TargetingDecal->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	TargetingDecal->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.05f));
	TargetingDecal->DecalSize = FVector(128.f, 128.f, 256.f);
	TargetingDecal->SetVisibility(false);
#pragma endregion

#pragma region ATTCK
	AttackFunction = CreateDefaultSubobject<UEnemyAttackFunction>(TEXT("AttackFunction"));
	AttackDamage = 10.f;
#pragma endregion
}

void AEnemy::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	EnemyController = NewController;
	AttackFunction->SetOwner(GetMesh(),EnemyController);
}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	if (!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());

#pragma region ATTACK

	//KnockBack
	IsAttacking = false;
	AttackRange = 200.f;
	KnockBackPower = 800.f;
	bIsback = false;
#pragma endregion
	
#pragma region HUD
	//HUD
	if (WEnemyHealth) {
		HealthWidget->SetWidgetClass(WEnemyHealth);
		HealthWidget->SetDrawSize(FVector2D(150.f, 20.f));
		HealthWidget->SetVisibility(false);
	}

	//HealthBar
	HealthBar = Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject());
	HealthBar->SetEnemyOwner(this);
	HealthBar->SetOwnerHealth(GetHealthRatio(), MaxHealth, CurrentHealth);
#pragma endregion
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** KnockBack의 조건이 만족하면 뒤로 밀림. */
	IsKnockBack();
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());

	//행동이 끝나면 다른 함수에게 알려준다. ->OnMontageEnded는 델리게이트 
	Anim->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
#pragma region ATTACK
void AEnemy::Attack() {
	if(!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());

	if (AttackMontage && Anim) {
		IsAttacking = true;
		Anim->Montage_Play(AttackMontage);
		Anim->Montage_JumpToSection("Attack1", AttackMontage);
	}
}

void AEnemy::AttackStart() {
	FString Type = "Enemy";
	AttackFunction->AttackStart(GetActorLocation(),GetActorForwardVector(),EnemyDamageType, Type, GetHitParticle(),GetAttackRange(), AttackDamage);
}

void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking) return;
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	if (CurrentHealth <= 0) return 0.f;

	SetHealthRatio();
	CurrentHealth -= DamageAmount;
	if(CurrentHealth <= 0) {
		CurrentHealth = 0;
		DeathEnd();
	}
	HealthBar->SetOwnerHealth(GetHealthRatio(),MaxHealth, CurrentHealth);

	return DamageAmount;
}

void AEnemy::DeathEnd() {
	if (!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	if (AttackMontage && Anim) {
		Anim->Montage_Play(AttackMontage);
		Anim->Montage_JumpToSection("Death", AttackMontage);
	}

	Cast<AEnemyController>(GetController())->StopBeTree();	//비헤이비어 트리 정지 (내가 만듬)
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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