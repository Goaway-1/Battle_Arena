#include "SkillFunction.h"
#include "Components/DecalComponent.h"
#include "Enemy.h"
#include "Lazer.h"

USkillFunction::USkillFunction(){
	PrimaryComponentTick.bCanEverTick = true;
	bGround = false;
}

void USkillFunction::BeginPlay(){
	Super::BeginPlay();
	
}

void USkillFunction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/** Targeting On Ground */
	SetSkillLocation();
}

void USkillFunction::SetInitial(APawn* P, USkeletalMeshComponent* S, AController* C,AActor* A) {
	OwnerInstigator = P;
	OwnerSkeletal = S;
	OwnerController = C;
	OwnerActor = A;
}

void USkillFunction::LazerAttack() {
}
void USkillFunction::LazerEnd() {
}
void USkillFunction::GroundAttack() {
}
void USkillFunction::SetSkillLocation() {
}
void USkillFunction::ConfirmTargetAndContinue() {
}