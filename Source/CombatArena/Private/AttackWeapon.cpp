#include "AttackWeapon.h"

AAttackWeapon::AAttackWeapon() {
	Damage = 10.f;
	AttackRange = 200.f;
}

void AAttackWeapon::Equip(class AMainPlayer* Player) {
	Super::Equip(Player);

	if (Player) {
		if ((GetWeaponPos() == EWeaponPos::EWP_Left && Player->GetLeftCurrentWeapon() != nullptr) || (GetWeaponPos() == EWeaponPos::EWP_Right && Player->GetRightCurrentWeapon() != nullptr)) {
			Player->ItemDrop();
		}

		/** ���� ���� */
		const USkeletalMeshSocket* HandSocket = nullptr;
		if (GetWeaponPos() == EWeaponPos::EWP_Right) HandSocket = Player->GetMesh()->GetSocketByName("RightWeapon");
		else HandSocket = Player->GetMesh()->GetSocketByName("LeftWeapon");

		if (HandSocket) {
			HandSocket->AttachActor(this, Player->GetMesh());
			Player->SetWeaponStatus(EWeaponStatus::EWS_Weapon);

			Player->AttackRange = GetAttackRange();		//������ ���⸸ �Ÿ� ����
			Player->SetRightCurrentWeapon(this);

			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}