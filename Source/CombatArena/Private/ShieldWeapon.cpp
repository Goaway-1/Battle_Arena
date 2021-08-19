#include "ShieldWeapon.h"

AShieldWeapon::AShieldWeapon() {
	WeaponPos = EWeaponPos::EWP_Left;
	ShiledMinAngle = -40.f;
	ShiledMaxAngle = 40.f;
}

void AShieldWeapon::BeginPlay() {
	Super::BeginPlay();
}

void AShieldWeapon::Equip(class AMainPlayer* Player) {
	Super::Equip(Player);

	if (Player) {
		if ((GetWeaponPos() == EWeaponPos::EWP_Left && Player->GetLeftCurrentWeapon() != nullptr)) {
			Player->ItemDrop();
		}

		/** ÀåÂø ·ÎÁ÷ */
		const USkeletalMeshSocket* HandSocket = nullptr;
		HandSocket = Player->GetMesh()->GetSocketByName("LeftWeapon");

		if (HandSocket) {
			HandSocket->AttachActor(this, Player->GetMesh());
			Player->SetWeaponStatus(EWeaponStatus::EWS_Weapon);
			Player->SetLeftCurrentWeapon(this);

			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}