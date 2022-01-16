#include "AttackWeapon.h"
#include "Boss_Enemy.h"

AAttackWeapon::AAttackWeapon() {
	WeaponPos = EWeaponPos::EWP_Melee;
	Damage = 20.f;
	AttackRange = 200.f;	

	AttackCollision = CreateDefaultSubobject<UCapsuleComponent>("AttackCollision");
	AttackCollision->SetupAttachment(SkeletalMesh);
	AttackCollision->SetCollisionProfileName(FName("PlayerWeapon"));
}

void AAttackWeapon::BeginPlay()
{
	Super::BeginPlay();

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AAttackWeapon::OnAttackOverlap);
	SetAttackCollision(false);
}

void AAttackWeapon::SetAttackCollision(bool value) {	
	if(!value) AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	else AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAttackWeapon::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		ABoss_Enemy* Enemy = Cast<ABoss_Enemy>(OtherActor);
		if (Enemy) {
			/** Set Enemy Balance */
			AMainPlayer* Player = Cast<AMainPlayer>(AtOwner);
			Player->PlayerController->SetBalanceTarget(Enemy);	

			/** Attack */
			Enemy->SetCurrentAttack(AtOwner->GetName() + this->GetName() + FString::FromInt(Player->GetAttackCnt()));	
			UGameplayStatics::ApplyDamage(Enemy, Damage, AtController, AtOwner, AtDamageType);
		}
	}
}

void AAttackWeapon::SetAttackInit(AController* CauserController, AActor* Causer, TSubclassOf<UDamageType> Type) {
	AtController = CauserController;
	AtOwner = Causer;
	AtDamageType = Type;
}

void AAttackWeapon::Equip(class AMainPlayer* Player) {
	Super::Equip(Player);

	if (Player) {
		if ((GetWeaponPos() == EWeaponPos::EWP_Shield && Player->GetShieldCurrentWeapon() != nullptr) || (GetWeaponPos() == EWeaponPos::EWP_Melee && Player->GetAttackCurrentWeapon() != nullptr)) {
			Player->ItemDrop();
		}

		/** ���� ���� */
		const USkeletalMeshSocket* HandSocket = nullptr;
		if (GetWeaponPos() == EWeaponPos::EWP_Melee) HandSocket = Player->GetMesh()->GetSocketByName("MeleeWeapon");
		else if (GetWeaponPos() == EWeaponPos::EWP_Shield) HandSocket = Player->GetMesh()->GetSocketByName("ShieldWeapon");
		else if (GetWeaponPos() == EWeaponPos::EWP_Bow) HandSocket = Player->GetMesh()->GetSocketByName("BowWeapon");

		if (HandSocket) {
			HandSocket->AttachActor(this, Player->GetMesh());
			Player->SetWeaponStatus(EWeaponStatus::EWS_Melee);

			Player->SetAttackRange(GetAttackRange());		//������ ���⸸ �Ÿ� ����
			Player->SetAttackCurrentWeapon(this);
			Player->SetAttackDamage(Damage);	

			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}