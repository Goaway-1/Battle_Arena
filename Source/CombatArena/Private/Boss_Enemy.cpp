#include "Boss_Enemy.h"
#include "EnemyAttackFunction.h"

ABoss_Enemy::ABoss_Enemy() {
	/** INIT*/
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	/** Attacl & Skill */
	AttackDamage = 10.f;
	AttackRange = 280.f;
	KnockBackPower = 800.f;

	ESkillFunction = CreateDefaultSubobject<UEnemySkillFunction>("ESkillFunction");	

	/** Balance */
	Balance = CreateDefaultSubobject<UBalance>("Balance");
	DecreaseBalanceTime = 1.0f;
	bIsFainted = false;
}
#pragma region INIT
void ABoss_Enemy::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	ESkillFunction->SetInitial(GetController()->GetPawn(), GetMesh(), GetController(), this);
}
void ABoss_Enemy::BeginPlay() {
	Super::BeginPlay();
}
void ABoss_Enemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
void ABoss_Enemy::PostInitializeComponents() {
	Super::PostInitializeComponents();
}
void ABoss_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
#pragma endregion
void ABoss_Enemy::Attack(FString type) {
	Super::Attack(type);

	if (type == "Melee") {
		Anim->Montage_Play(AttackMontage);
		Anim->Montage_JumpToSection(GetAttackMontageSection("Attack"), AttackMontage);
	}
	else if (type == "Skill" && !bisSkill) {
		Anim->Montage_Play(SkillAttackMontage);
		Anim->Montage_JumpToSection(GetAttackMontageSection("Skill"), SkillAttackMontage);
	}
}
float ABoss_Enemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	/** Balance Test */
	Balance->SetCurrentBalance(20.f);
	Balance->SetDecreaseBalance(false);
	if (Balance->GetCurrentBalance() >= 100.f) BrokenBalance();
	else GetWorldTimerManager().SetTimer(BalanceHandle, FTimerDelegate::CreateLambda([&] { Balance->SetDecreaseBalance(true); }), DecreaseBalanceTime, false);

	return DamageAmount;
}
void ABoss_Enemy::SkillAttack() {
	//Skill �ҷ�����
	if (bisSkill) return;
	bisSkill = true;

	/** Random */
	if (SkillType == "Meteor") ESkillFunction->GroundAttack();
	else if (SkillType == "Lazer") ESkillFunction->LazerAttack();
	else if (SkillType == "Rush") {
		float dis = GetDistanceTo(EnemyController->GetCurrentTarget()) / 1500.f;
		GetWorldTimerManager().SetTimer(SKillCoolTimer, this, &ABoss_Enemy::DashSkill, dis, false);
		return;
	}
	else if (SkillType == "Magic") ESkillFunction->MagicAttack();

	GetWorldTimerManager().SetTimer(SKillCoolTimer, this, &ABoss_Enemy::SkillAttackEnd, 1.0f, false);
}
void ABoss_Enemy::SkillAttackEnd() {
	bisSkill = false;

	Anim->StopAllMontages(0.f);
	if (SkillType == "Meteor") ESkillFunction->GroundAttack();
	else if (SkillType == "Lazer") ESkillFunction->LazerEnd();
	else if (SkillType == "Magic") ESkillFunction->MagicEnd();
}
void ABoss_Enemy::DashSkill() {
	Anim->Montage_JumpToSection("Attack5", SkillAttackMontage);
	GetWorldTimerManager().SetTimer(SKillCoolTimer, this, &ABoss_Enemy::SkillAttackEnd, 0.79f, false);
}
void ABoss_Enemy::StartLookAround(bool isLeft) {
	/** Look At the Target */
	AMainPlayer* Target = Cast<AMainPlayer>(EnemyController->GetBrainComponent()->GetBlackboardComponent()->GetValueAsObject(AEnemyController::TargetActor));
	if (Target == nullptr) return;
	FVector LookVec = Target->GetActorLocation() - GetActorLocation();
	LookVec.Z = 0;
	FRotator LookRot = FRotationMatrix::MakeFromX(LookVec).Rotator();
	SetActorRotation(LookRot);

	/** Animation */
	Anim->Montage_Play(LookAroundMontage);
	if (isLeft) Anim->Montage_JumpToSection("Left", LookAroundMontage);
	else  Anim->Montage_JumpToSection("Right", LookAroundMontage);
}
void ABoss_Enemy::BrokenBalance() {
	Balance->SetCurrentBalance(-100.f);
	bIsFainted = true;
	EnemyController->SetIsFaint(true);
}
void ABoss_Enemy::ActiveFaint() {
	if (!FaintMontage) return;
	Anim->Montage_Stop(0.f);
	Anim->Montage_Play(FaintMontage);
	Anim->Montage_JumpToSection("Faint", FaintMontage);
}
void ABoss_Enemy::DeactiveFaint() {		
	bIsFainted = false;
	EnemyController->SetIsFaint(false);
}
void ABoss_Enemy::SpecialHitMontage() {
	if (!FaintMontage) return;
	bIsFainted = false;
	Anim->Montage_Stop(0.f);
	Anim->Montage_Play(FaintMontage);
	Anim->Montage_JumpToSection("SpecialHited", FaintMontage);
}