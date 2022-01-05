#include "BowWeapon.h"
#include "Arrow.h"

ABowWeapon::ABowWeapon() {
	WeaponPos = EWeaponPos::EWP_Bow;
}
void ABowWeapon::BeginPlay() {
	Super::BeginPlay();
}
void ABowWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABowWeapon::InitalBow(AActor* BOwner, AController* BController) {
	BowOwner = BOwner;
	BowController = BController;
}
void ABowWeapon::Equip(class AMainPlayer* Player) {
	Super::Equip(Player);

	if (Player) {
		while (true) {
			if (Player->GetShieldCurrentWeapon() == nullptr && Player->GetAttackCurrentWeapon() == nullptr) break;
			Player->ItemDrop();
		}

		/** ÀåÂø ·ÎÁ÷ */
		const USkeletalMeshSocket* HandSocket = nullptr;
		if (GetWeaponPos() == EWeaponPos::EWP_Melee) HandSocket = Player->GetMesh()->GetSocketByName("MeleeWeapon");
		else if(GetWeaponPos() == EWeaponPos::EWP_Shield) HandSocket = Player->GetMesh()->GetSocketByName("ShieldWeapon");
		else if(GetWeaponPos() == EWeaponPos::EWP_Bow) HandSocket = Player->GetMesh()->GetSocketByName("BowWeapon");

		if (HandSocket) {
			HandSocket->AttachActor(this, Player->GetMesh());
			Player->SetWeaponStatus(EWeaponStatus::EWS_Bow);
			Player->SetAttackCurrentWeapon(this);
			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			InitalBow(Player,Player->Controller);
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
void ABowWeapon::Fire(int Cnt) {
	if(!Arrow) return;

	Arrow->Fire(ChargeAmount, Cnt);
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
		Arrow->InitalArrow(BowOwner,BowController);
	}
}