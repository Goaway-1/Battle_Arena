#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "MainPlayer.h"
#include "Enemy.h"

AWeapon::AWeapon() {
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());
	SetActorRotation(FRotator(180.f, 0.f, 0.f));

	SetItemType(EItemType::EIT_Weapon);
}
void AWeapon::BeginPlay() {
	Super::BeginPlay();
}
void AWeapon::Equip(class AMainPlayer* Player) {
	SetItemState(EItemState::EIS_Equip);
	SetActorRotation(FRotator(0.f));
}
void AWeapon::UnEquip() {
	SetItemState(EItemState::EIS_Ground);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CollisionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetActorLocationAndRotation(GetActorLocation(), FRotator(0.f,0.f, 0.f));
}