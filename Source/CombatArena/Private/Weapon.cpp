#include "Weapon.h"
#include "MainPlayer.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"

AWeapon::AWeapon() {
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	AttackBox->SetupAttachment(GetRootComponent());
	AttackBox->SetCollisionProfileName(TEXT("PlayerWeapon"));	//ÄÝ¸®Àü ¼³Á¤

	Damage = 10.f;
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnAttackBoxOverlapBegin);
	AttackBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnAttackBoxOverlapEnd);
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);	//¿ø·¡ ²¨ÀÖ´Â »óÅÊ
}

#pragma region BASIC

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if(Player) Player->SetActiveOverlappingItem(this);
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if (Player) Player->SetActiveOverlappingItem(nullptr);
	}
}

void AWeapon::Equip(class AMainPlayer* Player) {
	if (Player) {
		const USkeletalMeshSocket* RightHandSocket = Player->GetMesh()->GetSocketByName("RightWeapon");
		SetInstigator(Player->GetController());	//
		if (RightHandSocket) {
			RightHandSocket->AttachActor(this, Player->GetMesh());
			Player->SetWeaponStatus(EWeaponStatus::EWS_Weapon);
			Player->SetCurrentWeapon(this);
			//ÄÝ¸®ÀüÀ» ²¨¾ßµÅ
			CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
#pragma endregion

void AWeapon::OnAttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {	
	if (OtherActor) {
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		
		if (Enemy) {
			UE_LOG(LogTemp, Warning, TEXT("Hit!"));

			UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
		}
	}
}

void AWeapon::OnAttackBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}

void AWeapon::ActiveOnCollision() {
	AttackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UE_LOG(LogTemp, Warning, TEXT("CollisionON"));
}
void AWeapon::DeActiveOnCollision() {
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UE_LOG(LogTemp, Warning, TEXT("CollisionOff"));
}