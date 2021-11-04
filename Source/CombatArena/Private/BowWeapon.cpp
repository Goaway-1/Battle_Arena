#include "BowWeapon.h"
#include "Arrow.h"

ABowWeapon::ABowWeapon() {
	WeaponPos = EWeaponPos::EWP_Bow;
	Damage = 20.f;
	AttackRange = 200.f;
}

void ABowWeapon::BeginPlay() {
	Super::BeginPlay();
}
void ABowWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
void ABowWeapon::Equip(class AMainPlayer* Player) {
	Super::Equip(Player);

	if (Player) {
		while (true) {
			if (Player->GetShieldCurrentWeapon() == nullptr && Player->GetAttackCurrentWeapon() == nullptr) break;
			Player->ItemDrop();
		}

		/** 장착 로직 */
		const USkeletalMeshSocket* HandSocket = nullptr;
		if (GetWeaponPos() == EWeaponPos::EWP_Melee) HandSocket = Player->GetMesh()->GetSocketByName("MeleeWeapon");
		else if(GetWeaponPos() == EWeaponPos::EWP_Shield) HandSocket = Player->GetMesh()->GetSocketByName("ShieldWeapon");
		else if(GetWeaponPos() == EWeaponPos::EWP_Bow) HandSocket = Player->GetMesh()->GetSocketByName("BowWeapon");

		if (HandSocket) {
			HandSocket->AttachActor(this, Player->GetMesh());
			Player->SetWeaponStatus(EWeaponStatus::EWS_Bow);

			Player->AttackRange = GetAttackRange();		//오른쪽 무기만 거리 지정
			Player->SetAttackCurrentWeapon(this);

			Player->SetAttackDamage(Damage);
			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ABowWeapon::BeginCharge_Implementation() {
	if(!Arrow) return;
}
void ABowWeapon::EndCharge_Implementation() {
	if (Arrow) {
		Arrow->Destroy();
		Arrow = nullptr;
	}
}
void ABowWeapon::Fire() {
	if(!Arrow) return;

	Arrow->Fire(ChargeAmount);
	Arrow = nullptr;
	EndCharge();
}
void ABowWeapon::Reload() {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	if (!Arrow) {
		Arrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, FVector(0.f), FRotator(0.f), SpawnParams);
		Arrow->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("arrow_attach_socket"));
	}
}