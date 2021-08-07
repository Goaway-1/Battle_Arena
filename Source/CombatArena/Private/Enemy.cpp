#include "Enemy.h"
#include "EnemyController.h"
#include "EnemyAnim.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));	//콜리전 설정

	//Health
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;


	//Widget
	//EnemyWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyWidget"));
	//EnemyWidget->SetupAttachment(GetMesh());
	//EnemyWidget->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	//EnemyWidget->SetWidgetSpace(EWidgetSpace::Screen);		//Screen Mode 

	////UUserWidget* UUU = CreateWidget<UUserWidget>(this, HealthWidget, FName("UUU"));
	////UUU->AddToViewport();
	////UUU->SetVisibility(ESlateVisibility::Visible);
	//static ConstructorHelpers::FClassFinder<UUserWidget>
	//UI_HUD(TEXT("/Game/Enemy/Enemy_Health_Widget.Enemy_Health_Widget"));
	//
	//if (UI_HUD.Succeeded()) {
	//	EnemyWidget->SetWidgetClass(UI_HUD.Class);				//UUSerWidget of class 
	//	EnemyWidget->SetDrawSize(FVector2D(150.f, 50.f));
	//}
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

	//행동이 끝나면 다른 함수에게 알려준다. ->OnMontageEnded는 델리게이트 
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
	if (CurrentHealth <= 0) return 0.f;


	CurrentHealth -= DamageAmount;
	if(CurrentHealth <= 0) {
		CurrentHealth = 0;
		DeathEnd();
	}
	UE_LOG(LogTemp, Warning, TEXT("Health : %f"), CurrentHealth);

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