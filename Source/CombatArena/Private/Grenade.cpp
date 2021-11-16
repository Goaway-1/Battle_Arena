#include "Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MainPlayer.h"
#include "Enemy.h"

AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = true;
	SpawnSmokeTime = 1.5f;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(GetRootComponent());
	Collision->SetSphereRadius(30.0f);
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->SetUpdatedComponent(Mesh);		
	Projectile->bAutoActivate = false;

	/** ParticleSystem (Setup Plz) */
	Smoke = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke"));
	Smoke->SetupAttachment(Mesh);
	Smoke->SetVisibility(false);
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();	

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AGrenade::OnOverlapBegin);		
	Collision->OnComponentEndOverlap.AddDynamic(this, &AGrenade::OnOverlapEnd);		
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isGrowing && Smoke) {
		float tmp = Collision->GetUnscaledSphereRadius();
		tmp += (DeltaTime * 100.0f);
		Collision->SetSphereRadius(tmp);

		FVector temp = Smoke->GetRelativeScale3D();
		temp += FVector(DeltaTime * 0.3f);
		Smoke->SetRelativeScale3D(temp);

		if(tmp > 700.f) {	
			isGrowing = false;
			GetWorldTimerManager().ClearTimer(SpawnSmokeHandle);
			GetWorldTimerManager().SetTimer(SpawnSmokeHandle, this, &AGrenade::DestorySmoke, SpawnSmokeTime, false);
		}
	}
}

void AGrenade::SetFire(FRotator Rot) {
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorLocation(GetActorLocation());
	
	/** Set Rotation */
	Mesh->SetRelativeRotation(Rot);
	Projectile->SetVelocityInLocalSpace(FVector::ForwardVector * 8000);
	Projectile->Activate();

	GetWorldTimerManager().SetTimer(SpawnSmokeHandle, this, &AGrenade::SpawnSmoke, SpawnSmokeTime, false);
}

void AGrenade::SpawnSmoke() {
	UE_LOG(LogTemp, Warning, TEXT("Active Smoke"));
	if(Smoke) {
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Smoke->SetVisibility(true);
		GetWorldTimerManager().ClearTimer(SpawnSmokeHandle);
		GetWorldTimerManager().SetTimer(SpawnSmokeHandle, this, &AGrenade::GrowingSmoke, SpawnSmokeTime, false);
	}
}

void AGrenade::GrowingSmoke() {
	UE_LOG(LogTemp, Warning, TEXT("Growing Smoke"));
	isGrowing = true;
}

void AGrenade::DestorySmoke() {
	UE_LOG(LogTemp, Warning, TEXT("Destory Smoke"));
	Smoke->DestroyComponent();
	Destroy();
}

void AGrenade::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Player) {
			UE_LOG(LogTemp, Warning, TEXT("Player : %s Can not See Forward!"), *Player->GetName());
		}
		if (Enemy) {
			UE_LOG(LogTemp, Warning, TEXT("Enemy : %s Can not See Forward!"),*Enemy->GetName());
		}
	}
}

void AGrenade::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Player) {
			UE_LOG(LogTemp, Warning, TEXT("Player : %s Can See"), *Player->GetName());
		}
		if (Enemy) {
			UE_LOG(LogTemp, Warning, TEXT("Enemy : %s Can See"), *Enemy->GetName());
		}
	}
}
