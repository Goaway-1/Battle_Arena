#include "Lazer.h"
#include "MainPlayer.h"
#include "Enemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerAttackFunction.h"

ALazer::ALazer(){
 	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	LazerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LazerMesh"));
	LazerArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("LazerArm"));
	LazerEndDetector = CreateDefaultSubobject<USphereComponent>(TEXT("LazerEndDetector"));

	SetRootComponent(DefaultSceneRoot);
	LazerMesh->SetupAttachment(GetRootComponent());
	LazerArm->SetupAttachment(GetRootComponent());
	LazerEndDetector->SetupAttachment(LazerArm);

	LazerArm->TargetArmLength = 4500.f;
	LazerEndDetector->SetSphereRadius(52.f);
}
void ALazer::BeginPlay()
{
	Super::BeginPlay();

	/** Dealing */
	LazerEndDetector->OnComponentBeginOverlap.AddDynamic(this, &ALazer::OverlapBeginActor);
	LazerEndDetector->OnComponentEndOverlap.AddDynamic(this, &ALazer::OverlapEndActor);

	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "Dealing";
	LatentInfo.UUID = 123;
	LatentInfo.Linkage = 1;
}
void ALazer::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}
void ALazer::OverlapBeginActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		bContinueDealing = true;
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);

		if (Player) {
			HitedController = Player->GetController();
			OverlapingEnemies.Add(Player);
			Dealing();
		}
	}
}
void ALazer::OverlapEndActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		OverlapingEnemies.Remove(Player);

		bContinueDealing = false;
	}
}
void ALazer::Dealing() {
	if (bContinueDealing) {
		for (auto hited : OverlapingEnemies) {
			AMainPlayer* Player = Cast<AMainPlayer>(hited);
			if(!Player) return;
			Player->SetCurrentAttack(GetName() + "AttackLazer" + FString::FromInt(HitCnt));
			UGameplayStatics::ApplyDamage(Player, Damage, HitedController,this, LazerDamageType);
			if (++HitCnt > 2) HitCnt = 0;
		}
		UKismetSystemLibrary::Delay(this, 1.0f, LatentInfo);
	}
}