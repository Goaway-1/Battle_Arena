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
#include "DamageTextWidget.h"	
#include "Components/PrimitiveComponent.h"
#include "EnemySkillFunction.h"
#include "Engine/World.h"

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
	TargetingDecal->DecalSize = FVector(10.f, 10.f, 90.f);
	TargetingDecal->SetVisibility(false);
#pragma endregion

#pragma region ATTCK
	AttackFunction = CreateDefaultSubobject<UEnemyAttackFunction>(TEXT("AttackFunction"));
	AttackDamage = 10.f;
#pragma endregion
#pragma region SKILL
	SkillFunction = CreateDefaultSubobject<UEnemySkillFunction>(TEXT("SkillFunction"));
#pragma endregion

}
void AEnemy::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	EnemyController = NewController;
	AttackFunction->SetOwner(GetMesh(),EnemyController);

#pragma region SKILL
	SkillFunction->SetInitial(GetController()->GetPawn(), GetMesh(), GetController(), this);
#pragma endregion
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
void AEnemy::Attack(FString type) {
	if(!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());

	if (AttackMontage && Anim) {
		IsAttacking = true;
		if (type == "Melee") {
			Anim->Montage_Play(AttackMontage);
			Anim->Montage_JumpToSection(GetAttackMontageSection("Attack"), AttackMontage);
		}
		else if(type == "Skill" && !bisSkill) {
			Anim->Montage_Play(SkillAttackMontage);
			Anim->Montage_JumpToSection("Attack", SkillAttackMontage);
		}
	}
}
void AEnemy::AttackReady() {
	LaunchCharacter(GetActorForwardVector() * 700.f, true, true);
}
void AEnemy::SkillAttack() {
	//Skill 불러오기
	if(bisSkill) return;
	bisSkill = true;
	SkillFunction->GroundAttack();
	
	GetWorldTimerManager().SetTimer(SKillCoolTimer,this,&AEnemy::SkillAttackEnd,0.4f,false);
}
void AEnemy::SkillAttackEnd() {
	bisSkill = false;
	SkillFunction->GroundAttack();
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

	/** ShowDamageText */
	AttackFunction->SpawnDamageText(GetActorLocation(), DamageAmount, DamageTextWidget, EventInstigator);

	return DamageAmount;
}
void AEnemy::DeathEnd() {
	if (!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	if (DeathMontage && Anim) {
		Anim->Montage_Play(DeathMontage);
		Anim->Montage_JumpToSection("Death", DeathMontage);
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
void AEnemy::LaunchSky(FVector Pos) {
	LaunchCharacter(Pos,false,false);
}
FName AEnemy::GetAttackMontageSection(FString Type) {
	if (Type == "Attack") {
		int range = FMath::RandRange(1,2);
		AttackDamage = (range == 1) ? 10.f : 20.f;
		return FName(*FString::Printf(TEXT("Attack%d"), range));
	}
	else return "Error";
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