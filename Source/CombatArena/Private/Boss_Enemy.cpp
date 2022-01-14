#include "Boss_Enemy.h"
#include "EnemySkillFunction.h"
#include "EnemyAnim.h"
#include "EnemyController.h"


ABoss_Enemy::ABoss_Enemy() {
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	//health
	MaxHealth = 30.f;
	CurrentHealth = MaxHealth;

	//attack
	AttackDamage = 10.f;
	AttackRange = 430.f;
	KnockBackPower = 800.f;

	ESkillFunction = CreateDefaultSubobject<UEnemySkillFunction>("ESkillFunction");	//단독
}

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
	//if (!Anim) Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());

	//Anim->OnMontageEnded.AddDynamic(this, &ABoss_Enemy::OnAttackMontageEnded);
}
void ABoss_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

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
void ABoss_Enemy::SkillAttack() {
	//Skill 불러오기
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
void ABoss_Enemy::DashSkill() {
	Anim->Montage_JumpToSection("Attack5", SkillAttackMontage);
	GetWorldTimerManager().SetTimer(SKillCoolTimer, this, &ABoss_Enemy::SkillAttackEnd, 0.79f, false);
}
void ABoss_Enemy::SkillAttackEnd() {
	bisSkill = false;

	Anim->StopAllMontages(0.f);
	if (SkillType == "Meteor") ESkillFunction->GroundAttack();
	else if (SkillType == "Lazer") ESkillFunction->LazerEnd();
	else if (SkillType == "Magic") ESkillFunction->MagicEnd();
}

//void ABoss_Enemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
//	if (!IsAttacking) return;
//	IsAttacking = false;
//	OnAttackEnd.Broadcast();
//}