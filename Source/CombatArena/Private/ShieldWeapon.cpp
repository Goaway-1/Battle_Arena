#include "ShieldWeapon.h"

AShieldWeapon::AShieldWeapon() {
	WeaponPos = EWeaponPos::EWP_Shield;
	ShiledMinAngle = -40.f;
	ShiledMaxAngle = 40.f;
}

void AShieldWeapon::BeginPlay() {
	Super::BeginPlay();
}

void AShieldWeapon::Equip(class AMainPlayer* Player) {
	Super::Equip(Player);

	if (Player) {
		if ((GetWeaponPos() == EWeaponPos::EWP_Shield && Player->GetShieldCurrentWeapon() != nullptr)) {
			Player->ItemDrop();
		}

		/** ÀåÂø ·ÎÁ÷ */
		const USkeletalMeshSocket* HandSocket = nullptr;
		if (GetWeaponPos() == EWeaponPos::EWP_Melee) HandSocket = Player->GetMesh()->GetSocketByName("MeleeWeapon");
		else if (GetWeaponPos() == EWeaponPos::EWP_Shield) HandSocket = Player->GetMesh()->GetSocketByName("ShieldWeapon");
		else if (GetWeaponPos() == EWeaponPos::EWP_Bow) HandSocket = Player->GetMesh()->GetSocketByName("BowWeapon");

		if (HandSocket) {
			HandSocket->AttachActor(this, Player->GetMesh());
			Player->SetWeaponStatus(EWeaponStatus::EWS_Weapon);
			Player->SetShieldCurrentWeapon(this);

			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}