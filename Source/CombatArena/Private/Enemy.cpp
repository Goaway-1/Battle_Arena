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

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));	//�ݸ��� ����


#pragma region HUD
	//Health
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	//HUD
	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);	//��ġ ����
	HealthWidget->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
#pragma endregion

	AttackFunction = CreateDefaultSubobject<UEnemyAttackFunction>(TEXT("AttackFunction"));
}

void AEnemy::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	EnemyController = NewController;
}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	if (!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());

#pragma region ATTACK

	//KnockBack
	KnockBackPower = 1000.f;
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
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());

	//�ൿ�� ������ �ٸ� �Լ����� �˷��ش�. ->OnMontageEnded�� ��������Ʈ 
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
		Anim->Montage_Play(AttackMontage);
		Anim->Montage_JumpToSection("Attack1", AttackMontage);
	}
}

void AEnemy::AttackStart() {
	AttackFunction->AttackStart();
}

void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsAttacking) return;
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

	Cast<AEnemyController>(GetController())->StopBeTree();	//�����̺�� Ʈ�� ���� (���� ����)
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::DestroyEnemy() {
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	Destroy();
}

void AEnemy::KnockBack(FVector Backward) {
	LaunchCharacter(Backward * KnockBackPower, true, true);	//�Է� ������
}

#pragma endregion

#pragma region HUD

void AEnemy::ShowEnemyHealth() {
	HealthWidget->SetVisibility(true);
}

void AEnemy::HideEnemyHealth() {

	HealthWidget->SetVisibility(false);
}

void AEnemy::SetHealthRatio() {
	HealthRatio = CurrentHealth / MaxHealth;
}
#pragma endregion