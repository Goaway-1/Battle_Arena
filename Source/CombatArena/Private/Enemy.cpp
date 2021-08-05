#include "Enemy.h"
#include "EnemyController.h"
#include "EnemyAnim.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));	//�ݸ��� ����

	//Health
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	if (!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
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

void AEnemy::Attack() {
	if(!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());

	if (AttackMontage && Anim) {
		Anim->Montage_Play(AttackMontage);
		Anim->Montage_JumpToSection("Attack1", AttackMontage);
	}
}

void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsAttacking) return;
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	
	CurrentHealth -= DamageAmount;
	if(CurrentHealth < 0.f) {
		CurrentHealth = 0;
		Die();
	}
	UE_LOG(LogTemp, Warning, TEXT("Health : %f"), CurrentHealth);

	return DamageAmount;
}

void AEnemy::Die() {
	

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}