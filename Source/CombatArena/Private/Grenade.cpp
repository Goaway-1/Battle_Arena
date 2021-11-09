#include "Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = true;
	SpawnSmokeTime = 1.0f;

	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = Root;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Mesh);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AGrenade::OnOverlapBegin);
	Collision->SetSphereRadius(160.0f);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->SetUpdatedComponent(Mesh);		//Collision의 Rotation에 영향
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();	

	GetWorldTimerManager().SetTimer(SpawnSmokeHandle, this, &AGrenade::SpawnSmoke, SpawnSmokeTime, false);
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isGrowing) {
		float tmp = Collision->GetUnscaledSphereRadius();
		tmp += DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("%f"),tmp);
		Collision->SetSphereRadius(tmp);
		if(tmp > 300.f) isGrowing = false;
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
void AGrenade::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("Overlaped Actor. Noe The Actor Can not see Anything"));
	/*if (OtherActor && !isOverlaped) {
		isOverlaped = true;
		UE_LOG(LogTemp, Warning, TEXT("Overlaped! Active Smoke"));
	}*/
}