#include "BowWeapon.h"
#include "Components/TimelineComponent.h"

ABowWeapon::ABowWeapon() {
	WeaponPos = EWeaponPos::EWP_Bow;
	Damage = 20.f;
	AttackRange = 200.f;

	///** TimeLine */
	//static ConstructorHelpers::FObjectFinder<UCurveFloat> Curvy(TEXT("CurveFloat'/Game/Blueprint/Weapons/ChargeAmount.ChargeAmount'"));
	//if (Curvy.Object) {
	//	fCurve = Curvy.Object;
	//}

	//ScoreTimeline = CreateDefaultSubobject<UTimelineComponent>(this, TEXT("TimelineScore"));

	//InterpFunction.BindUFunction(this, FName{ TEXT("TimelineFloatReturn") });
}

void ABowWeapon::BeginPlay() {
	Super::BeginPlay();

	//ScoreTimeline->AddInterpFloat(fCurve, InterpFunction, FName{ TEXT("Floaty") });
	//ScoreTimeline->Play(); 
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

void ABowWeapon::BeginCharge_Implementation() {

}
void ABowWeapon::StopCharge_Implementation() {

}

void ABowWeapon::EndCharge_Implementation() {

}

void ABowWeapon::Fire_Implementation() {

}

void ABowWeapon::Reload() {
	//GetWorld()->SpawnActor<AArrow>();
}