#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "MainPlayer.h"
#include "Enemy.h"

AWeapon::AWeapon() {
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());
	SetActorRotation(FRotator(180.f, 0.f, 0.f));
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();
}

#pragma region BASIC

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::Equip(class AMainPlayer* Player) {
	SetItemState(EItemState::EIS_Equip);
	SetActorRotation(FRotator(0.f));
}
void AWeapon::UnEquip() {
	/** 해제 로직 */ 
	SetItemState(EItemState::EIS_Ground);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CollisionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetActorLocationAndRotation(GetActorLocation(), FRotator(180.f,0.f, 0.f));
}
#pragma endregion