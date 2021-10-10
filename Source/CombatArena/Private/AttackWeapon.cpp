#include "AttackWeapon.h"

AAttackWeapon::AAttackWeapon() {
	WeaponPos = EWeaponPos::EWP_Melee;
	Damage = 20.f;
	AttackRange = 200.f;
}

void AAttackWeapon::Equip(class AMainPlayer* Player) {
	Super::Equip(Player);

	if (Player) {
		if ((GetWeaponPos() == EWeaponPos::EWP_Shield && Player->GetShieldCurrentWeapon() != nullptr) || (GetWeaponPos() == EWeaponPos::EWP_Melee && Player->GetAttackCurrentWeapon() != nullptr)) {
			Player->ItemDrop();
		}

		/** 장착 로직 */
		const USkeletalMeshSocket* HandSocket = nullptr;
		if (GetWeaponPos() == EWeaponPos::EWP_Melee) HandSocket = Player->GetMesh()->GetSocketByName("MeleeWeapon");
		else if (GetWeaponPos() == EWeaponPos::EWP_Shield) HandSocket = Player->GetMesh()->GetSocketByName("ShieldWeapon");
		else if (GetWeaponPos() == EWeaponPos::EWP_Bow) HandSocket = Player->GetMesh()->GetSocketByName("BowWeapon");

		if (HandSocket) {
			HandSocket->AttachActor(this, Player->GetMesh());
			Player->SetWeaponStatus(EWeaponStatus::EWS_Melee);

			Player->AttackRange = GetAttackRange();		//오른쪽 무기만 거리 지정
			Player->SetAttackCurrentWeapon(this);

			Player->SetAttackDamage(Damage);	
			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}