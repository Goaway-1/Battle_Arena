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
	Collision->SetSphereRadius(160.0f);
	Collision->SetRelativeLocation(FVector(0.f));

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->SetUpdatedComponent(GetRootComponent());		//Collision의 Rotation에 영향
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();	

	GetWorldTimerManager().SetTimer(SpawnSmokeHandle, this, &AGrenade::SpawnSmoke, SpawnSmokeTime, false);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AGrenade::OnOverlapBegin);		
	Collision->OnComponentEndOverlap.AddDynamic(this, &AGrenade::OnOverlapEnd);		
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isGrowing) {
		float tmp = Collision->GetUnscaledSphereRadius();
		tmp += (DeltaTime * 100.0f);
		Collision->SetSphereRadius(tmp);
		if(tmp > 1200.f) {	
			isGrowing = false;
			GetWorldTimerManager().ClearTimer(SpawnSmokeHandle);
			GetWorldTimerManager().SetTimer(SpawnSmokeHandle, this, &AGrenade::GrowingSmoke, SpawnSmokeTime, false);
		}
	}
}

void AGrenade::SpawnSmoke() {
	UE_LOG(LogTemp, Warning, TEXT("Active Smoke"));
	if(SmokeParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeParticle, GetActorLocation(), FRotator(0.f));
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
