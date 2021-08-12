#include "Enemy.h"
#include "EnemyController.h"
#include "EnemyAnim.h"
#include "MainPlayer.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.h"
#include "Engine/SkeletalMeshSocket.h"

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

	AttackBox_Left->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackBox_Right->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//KnockBack
	KnockBackPower = -1000.f;
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
	DeActiveOnCollision();
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
	DeActiveOnCollision();
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
			UGameplayStatics::ApplyDamage(Player, 10.f, EnemyController,this, EnemyDamageType);

			const USkeletalMeshSocket* R_HitSocket = GetMesh()->GetSocketByName("Right_Weapon");
			const USkeletalMeshSocket* L_HitSocket = GetMesh()->GetSocketByName("Left_Weapon");
			if (R_HitSocket && L_HitSocket && Player->GetHitParticle()) {
				FVector R_ParticleSpawnLocation = R_HitSocket->GetSocketLocation(GetMesh());
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Player->GetHitParticle(), R_ParticleSpawnLocation, FRotator(0.f));

				FVector L_ParticleSpawnLocation = L_HitSocket->GetSocketLocation(GetMesh());
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Player->GetHitParticle(), L_ParticleSpawnLocation, FRotator(0.f));
			}
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
void AEnemy::KnockBack() {
	LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0.f) * KnockBackPower, true, true);	//입력 방향대로
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