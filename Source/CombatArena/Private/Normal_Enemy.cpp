#include "Normal_Enemy.h"

ANormal_Enemy::ANormal_Enemy() {
	//movement
	GetCharacterMovement()->MaxWalkSpeed = 350.f;

	//health
	MaxHealth = 30.f;
	CurrentHealth = MaxHealth;

	//attack
	AttackDamage = 10.f;
	AttackRange = 430.f;
	KnockBackPower = 800.f;
}
#pragma region INIT
void ANormal_Enemy::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
}
void ANormal_Enemy::BeginPlay() {
	Super::BeginPlay();

}
void ANormal_Enemy::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}
void ANormal_Enemy::PostInitializeComponents(){
	Super::PostInitializeComponents();
	
}
void ANormal_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
#pragma endregion
float ANormal_Enemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return DamageAmount;
}
void ANormal_Enemy::Attack(FString type) {
	Super::Attack(type);
}
