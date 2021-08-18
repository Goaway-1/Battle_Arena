#include "ShieldWeapon.h"

AShieldWeapon::AShieldWeapon() {
	ShieldBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldBox"));

	ShieldBox->OnComponentBeginOverlap.AddDynamic(this, &AShieldWeapon::OnShieldOverlapStart);
	ShieldBox->OnComponentEndOverlap.AddDynamic(this, &AShieldWeapon::OnShieldOverlapEnd);
	ShieldBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShieldBox->SetRelativeLocation(FVector(0.f));
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

void AShieldWeapon::OnShieldOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	UE_LOG(LogTemp, Warning, TEXT("Overlap"));
}
void AShieldWeapon::OnShieldOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("OverlapEnd"));
}