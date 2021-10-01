#include "BowWeapon.h"

ABowWeapon::ABowWeapon() {
	WeaponPos = EWeaponPos::EWP_Bow;
	Damage = 20.f;
	AttackRange = 200.f;
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
			Player->SetWeaponStatus(EWeaponStatus::EWS_Weapon);

			Player->AttackRange = GetAttackRange();		//오른쪽 무기만 거리 지정
			Player->SetAttackCurrentWeapon(this);

			Player->SetAttackDamage(Damage);
			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}