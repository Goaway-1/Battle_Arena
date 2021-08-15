#include "Weapon.h"
#include "MainPlayer.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"

AWeapon::AWeapon() {
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	Damage = 10.f;
	AttackRange = 200.f;
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();
}

#pragma region BASIC

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if(Player) Player->SetActiveOverlappingItem(this);
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if (Player) Player->SetActiveOverlappingItem(nullptr);
	}
}

void AWeapon::Equip(class AMainPlayer* Player) {
	if (Player) {
		const USkeletalMeshSocket* RightHandSocket = Player->GetMesh()->GetSocketByName("RightWeapon");
		SetInstigator(Player->GetController());	//
		if (RightHandSocket) {
			RightHandSocket->AttachActor(this, Player->GetMesh());
			Player->SetWeaponStatus(EWeaponStatus::EWS_Weapon);
			Player->SetCurrentWeapon(this);
			//ÄÝ¸®ÀüÀ» ²¨¾ßµÅ
			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
void AWeapon::UnEquip() {
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CollisionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetActorLocation(GetActorLocation());
}
#pragma endregion