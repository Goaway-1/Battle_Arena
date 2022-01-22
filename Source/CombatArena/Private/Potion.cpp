#include "Potion.h"

APotion::APotion() {
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
}

void APotion::UseItem(float &Health) {
	Health += 10.f;
	if (Health >= 100.f) Health = 100.f;
	if (UsedSound != nullptr) UGameplayStatics::PlaySound2D(this, UsedSound);
	if (OverlapParticle) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator(0.f), true);
	Destroy();
}