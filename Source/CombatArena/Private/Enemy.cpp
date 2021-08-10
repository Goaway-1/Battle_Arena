#include "Enemy.h"
#include "EnemyController.h"
#include "EnemyAnim.h"
#include "MainPlayer.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));	//콜리전 설정

#pragma region ATTACK
	AttackBox_Left = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox_Left"));
	AttackBox_Left->SetupAttachment(GetMesh(), FName("Left_Weapon"));

	AttackBox_Right = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox_Right"));
	AttackBox_Right->SetupAttachment(GetMesh(), FName("Right_Weapon"));

	//SetCollision_AttackBoxs
	AttackBox_Left->SetCollisionProfileName(TEXT("EnemyWeapon"));
	AttackBox_Right->SetCollisionProfileName(TEXT("EnemyWeapon"));
#pragma endregion

#pragma region HUD
	//Health
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	//HUD
	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);	//위치 고정
	HealthWidget->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
#pragma endregion

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
	AttackBox_Left->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAttackBoxOverlapBegin);
	AttackBox_Left->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAttackBoxOverlapEnd);
	AttackBox_Right->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAttackBoxOverlapBegin);
	AttackBox_Right->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAttackBoxOverlapEnd);

	AttackBox_Left->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackBox_Right->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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
	HealthBar->SetOwner(this, 0);
	HealthBar->SetOwnerHealth();
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
	if (CurrentHealth <= 0) return 0.f;

	SetHealthRatio();
	CurrentHealth -= DamageAmount;
	if(CurrentHealth <= 0) {
		CurrentHealth = 0;
		DeathEnd();
	}
	HealthBar->SetOwnerHealth();

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

void AEnemy::OnAttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if (Player) {
			UE_LOG(LogTemp, Warning, TEXT("ENEMYATTACK"));
			UGameplayStatics::ApplyDamage(Player, 10.f, EnemyController,this, EnemyDamageType);
		}
	}
}

void AEnemy::OnAttackBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}
void AEnemy::ActiveOnCollision() {
	AttackBox_Left->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackBox_Right->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AEnemy::DeActiveOnCollision() {
	AttackBox_Left->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackBox_Right->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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