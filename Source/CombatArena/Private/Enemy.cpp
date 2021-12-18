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
#include "Balance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"

AEnemy::AEnemy()
{
#pragma region INIT
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));	
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

	//Targeting 
	TargetingDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("TargetingDecal"));
	TargetingDecal->SetupAttachment(GetMesh());
	TargetingDecal->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	TargetingDecal->DecalSize = FVector(10.f, 10.f, 90.f);
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

	AttackDamage = 10.f;
	IsAttacking = false;
	AttackRange = 200.f;
	KnockBackPower = 800.f;
	bIsback = false;
#pragma endregion
#pragma region SKILL
	SkillFunction = CreateDefaultSubobject<UEnemySkillFunction>(TEXT("SkillFunction"));
#pragma endregion
#pragma region BALANCE
	Balance = CreateDefaultSubobject<UBalance>("Balance");
	DecreaseBalanceTime = 1.0f;
	bIsFainted = false;
#pragma endregion
}

void AEnemy::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	EnemyController = Cast<AEnemyController>(NewController);
	AttackFunction->SetOwner(GetMesh(),EnemyController);
	SkillFunction->SetInitial(GetController()->GetPawn(), GetMesh(), GetController(), this);

	RightWeapon->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnWeaponOverlap);
	LeftWeapon->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnWeaponOverlap);
	AttackStart_Collision(false);
}
void AEnemy::BeginPlay()
{
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

	//행동이 끝나면 다른 함수에게 알려준다. ->OnMontageEnded는 델리게이트 
	Anim->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);
}
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

#pragma region MOVEMENT
void AEnemy::StartLookAround(bool isLeft) {
	/** Look At the Target */
	AMainPlayer* Target = Cast<AMainPlayer>(EnemyController->GetBrainComponent()->GetBlackboardComponent()->GetValueAsObject(AEnemyController::TargetActor));
	FVector LookVec = Target->GetActorLocation() - GetActorLocation();
	LookVec.Z = 0;
	FRotator LookRot = FRotationMatrix::MakeFromX(LookVec).Rotator();
	SetActorRotation(LookRot);

	/** Animation */
	Anim->Montage_Play(LookAroundMontage);
	if (isLeft) Anim->Montage_JumpToSection("Left", LookAroundMontage);
	else  Anim->Montage_JumpToSection("Right", LookAroundMontage);
}
#pragma endregion
#pragma region ATTACK
void AEnemy::Attack(FString type) {
	if(!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());

	Anim->StopAllMontages(0.f);
	if (AttackMontage && Anim) {
		IsAttacking = true;
		if (type == "Melee") {
			Anim->Montage_Play(AttackMontage);
			Anim->Montage_JumpToSection(GetAttackMontageSection("Attack"), AttackMontage);
		}
		else if(type == "Skill" && !bisSkill) {
			Anim->Montage_Play(SkillAttackMontage);
			Anim->Montage_JumpToSection(GetAttackMontageSection("Skill"), SkillAttackMontage);
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

	/** Random */
	if(SkillType == "Meteor") SkillFunction->GroundAttack();
	else if(SkillType == "Lazer") SkillFunction->LazerAttack();
	else if(SkillType == "Rush") SkillFunction->RushAttack();
	
	GetWorldTimerManager().SetTimer(SKillCoolTimer,this,&AEnemy::SkillAttackEnd,1.0f,false);
}
void AEnemy::SkillAttackEnd() {
	bisSkill = false; 
	
	if (SkillType == "Meteor") SkillFunction->GroundAttack();
	else if (SkillType == "Lazer") SkillFunction->LazerEnd();
	else if (SkillType == "Rush") SkillFunction->RushEnd();
}
void AEnemy::AttackStart_Internal() {
	FString Type = "Enemy";
	AttackFunction->SkillAttackStart(GetActorLocation(),GetActorForwardVector(),InternalDamageType, Type, GetHitParticle(),GetAttackRange(), AttackDamage);
}
void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted){
	if (!IsAttacking) return;
	IsAttacking = false;
	OnAttackEnd.Broadcast();
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

	if (CurrentHealth <= 0) return 0.f;

	SetHealthRatio();
	CurrentHealth -= DamageAmount;
	if(CurrentHealth <= 0) {
		CurrentHealth = 0;
		DeathEnd();
	}
	HealthBar->SetOwnerHealth(GetHealthRatio(),MaxHealth, CurrentHealth);

	/** Balance Test */
	Balance->SetCurrentBalance(20.f);
	Balance->SetDecreaseBalance(false);
	if (Balance->GetCurrentBalance() >= 100.f) BrokenBalance();
	else GetWorldTimerManager().SetTimer(BalanceHandle, FTimerDelegate::CreateLambda([&] { Balance->SetDecreaseBalance(true); }), DecreaseBalanceTime, false);

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
	else if (Type == "Skill") {
		/*int range = FMath::RandRange(1, 3);
		switch (range) {
			case 1:
				AttackDamage = 20.f;
				SkillType = "Meteor";
				break;
			case 2:
				AttackDamage = 30.f;
				SkillType = "Lazer";
				break;
			case 3:
				AttackDamage = 30.f;
				SkillType = "Rush";
				break;
			default:
				SkillType = "Error";
				break;
		}
		return FName(*FString::Printf(TEXT("Attack%d"), range));*/
		AttackDamage = 30.f;
		SkillType = "Rush";
		return FName(*FString::Printf(TEXT("Attack%d"), 3));
	}
	else return "Error";
}
void AEnemy::SetVisibleInFog(bool bisin, int time) {
	if (!EnemyController) return;
	EnemyController->SetVisibleInFog(bisin);
	SmokeTime = time + 2.f;
}
void AEnemy::ActiveFogEvent() {
	UE_LOG(LogTemp, Warning, TEXT("Enemy ActiveFogEvent"));
	if (IsInFogMontage && Anim) Anim->Montage_Play(IsInFogMontage);
	GetWorldTimerManager().SetTimer(FogHandle, this, &AEnemy::DeactiveFogEvent, SmokeTime, false);
}
void AEnemy::DeactiveFogEvent() {
	UE_LOG(LogTemp, Warning, TEXT("Enemy DeactiveFogEvent"));
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
#pragma region BALANCE
void AEnemy::BrokenBalance() {
	Balance->SetCurrentBalance(-100.f);
	bIsFainted = true;
	EnemyController->SetIsFaint(true);
}
void AEnemy::ActiveFaint() {	
	if (!FaintMontage) return;
	Anim->Montage_Stop(0.f);
	Anim->Montage_Play(FaintMontage);
	Anim->Montage_JumpToSection("Faint", FaintMontage);
}
void AEnemy::DeactiveFaint() {		//Animation과 연동 -> 상태 도중 맞을때
	bIsFainted = false;		
	EnemyController->SetIsFaint(false);
}
void AEnemy::SpecialHitMontage() {
	if (!FaintMontage) return;
	bIsFainted = false;
	Anim->Montage_Stop(0.f);
	Anim->Montage_Play(FaintMontage);
	Anim->Montage_JumpToSection("SpecialHited", FaintMontage);
}
#pragma endregion