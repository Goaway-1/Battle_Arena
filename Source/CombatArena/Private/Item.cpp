#include "Item.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayer.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);

	IdleParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleComponent"));
	IdleParticleComponent->SetupAttachment(GetRootComponent());

	//Spin Item
	SpinSpeed = 45.f;
	SetItemState(EItemState::EIS_Ground);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetSpinItem();
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OverlapParticle) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator(0.f), true);
	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if (Player) Player->SetActiveOverlappingItem(this);
	}
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if (Player) Player->SetActiveOverlappingItem(nullptr);
	}
}

void AItem::SetSpinItem() {
	if (GetItemState() == EItemState::EIS_Ground) {
		FRotator Rot = GetActorRotation();
		Rot.Yaw += GetWorld()->GetDeltaSeconds() * SpinSpeed;
		SetActorRotation(Rot);
	}
}