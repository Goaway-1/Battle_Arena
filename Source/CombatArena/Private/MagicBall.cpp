#include "MagicBall.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainPlayer.h"

AMagicBall::AMagicBall(){
	PrimaryActorTick.bCanEverTick = true;

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->InitialSpeed = 2000;
	Projectile->bRotationFollowsVelocity = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetSphereRadius(100.f);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMagicBall::OnOverlapBegin);

	Flying_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Flying_Particle"));
	Flying_Particle->SetupAttachment(GetRootComponent());
	Flying_Particle->SetRelativeScale3D(FVector(3.0f));
}

void AMagicBall::BeginPlay(){
	Super::BeginPlay();
}

void AMagicBall::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AMagicBall::OnConstruction(const FTransform& Transform) {
	if(!Moving_Particle) return;
	Flying_Particle->SetTemplate(Moving_Particle);
}

void AMagicBall::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Impact_Particle, GetActorLocation() + Location_Offset_Impact_Particle, Rotate_Impact_Particle);

	if (MagicSound) UGameplayStatics::SpawnSoundAtLocation(this, MagicSound, GetActorLocation());

	Flying_Particle->Deactivate();
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
		Flying_Particle->DestroyComponent();
		Destroy();
		}, 2.0f, 1);

	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if (Player) {
			Player->SetCurrentAttack(GetName() + "AttackMagic" + FString::FromInt(HitCnt));
			UGameplayStatics::ApplyDamage(Player, Damage, Player->GetController(), this, MagicDamageType);
		}
	}
}
