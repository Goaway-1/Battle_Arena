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

void ABowWeapon::Equip(class AMainPlayer* Player) {
	Super::Equip(Player);

	if (Player) {
		while (true) {
			if (Player->GetShieldCurrentWeapon() == nullptr && Player->GetAttackCurrentWeapon() == nullptr) break;
			Player->ItemDrop();
		}

		/** ���� ���� */
		const USkeletalMeshSocket* HandSocket = nullptr;
		if (GetWeaponPos() == EWeaponPos::EWP_Melee) HandSocket = Player->GetMesh()->GetSocketByName("MeleeWeapon");
		else if(GetWeaponPos() == EWeaponPos::EWP_Shield) HandSocket = Player->GetMesh()->GetSocketByName("ShieldWeapon");
		else if(GetWeaponPos() == EWeaponPos::EWP_Bow) HandSocket = Player->GetMesh()->GetSocketByName("BowWeapon");

		if (HandSocket) {
			HandSocket->AttachActor(this, Player->GetMesh());
			Player->SetWeaponStatus(EWeaponStatus::EWS_Weapon);

			Player->AttackRange = GetAttackRange();		//������ ���⸸ �Ÿ� ����
			Player->SetAttackCurrentWeapon(this);

			Player->SetAttackDamage(Damage);
			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ABowWeapon::BeginCharge_Implementation() {
	if(!Arow) return;
}

void ABowWeapon::StopCharge_Implementation() {
	
}

void ABowWeapon::EndCharge_Implementation() {
	if (!Arow) return;
}

void ABowWeapon::Fire() {
	if(!Arow) return;

	Arow->Fire(ChargeAmount);
	StopCharge();
}

void ABowWeapon::Reload() {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	Arow = GetWorld()->SpawnActor<AArrow>(ArrowClass, FVector(0.f), FRotator(0.f), SpawnParams);
	Arow->AttachToComponent(SkeletalMesh,FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("arrow_attach_socket"));
}