#include "AttackWeapon.h"

AAttackWeapon::AAttackWeapon() {
	WeaponPos = EWeaponPos::EWP_Melee;
	Damage = 20.f;
	AttackRange = 200.f;
	AttackCnt = 0;		

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
	if(!value){
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
		AttackCnt++;
		if (AttackCnt > 2) AttackCnt = 0;
	}
	else AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAttackWeapon::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy) {
			/** Set Enemy Balance */
			AMainPlayer* Player = Cast<AMainPlayer>(AtOwner);
			Player->PlayerController->SetBalanceTarget(Enemy);	

			/** Attack */
			FString text = AtOwner->GetName() + this->GetName() + FString::FromInt(AttackCnt);
			Enemy->SetCurrentAttack(AtOwner->GetName() + this->GetName() + FString::FromInt(AttackCnt));	
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

		/** 장착 로직 */
		const USkeletalMeshSocket* HandSocket = nullptr;
		if (GetWeaponPos() == EWeaponPos::EWP_Melee) HandSocket = Player->GetMesh()->GetSocketByName("MeleeWeapon");
		else if (GetWeaponPos() == EWeaponPos::EWP_Shield) HandSocket = Player->GetMesh()->GetSocketByName("ShieldWeapon");
		else if (GetWeaponPos() == EWeaponPos::EWP_Bow) HandSocket = Player->GetMesh()->GetSocketByName("BowWeapon");

		if (HandSocket) {
			HandSocket->AttachActor(this, Player->GetMesh());
			Player->SetWeaponStatus(EWeaponStatus::EWS_Melee);

			Player->SetAttackRange(GetAttackRange());		//오른쪽 무기만 거리 지정
			Player->SetAttackCurrentWeapon(this);
			Player->SetAttackDamage(Damage);	

			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}