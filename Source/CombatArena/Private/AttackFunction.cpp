#include "AttackFunction.h"
#include "CollisionQueryParams.h"
#include "DamageTextWidget.h"	
#include "MainController.h"	
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainPlayer.h"
#include "Enemy.h"


UAttackFunction::UAttackFunction(){
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackFunction::BeginPlay(){
	Super::BeginPlay();
}

void UAttackFunction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttackFunction::SetOwner(USkeletalMeshComponent* TakeMesh,AController* TakeController) {
	Owner = GetOwner();
	Mesh = TakeMesh;
	Controller = TakeController;
}

void UAttackFunction::SkillAttackStart(TSubclassOf<UDamageType> DamageType,FString OwnerType, UParticleSystem* HitParticle,float AttackRange,float Damage, int AttackCnt){
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	UClass* SeekClass = nullptr;		//찾아낼 클래스
	TArray<AActor*> OutActors;			//찾아낼 액터들
	TArray<AActor*> IgnoreActors;		//무시할 액터들
	IgnoreActors.Init(Owner, 1);

	/** 찾아낼 클래스와 트레이스 채널 지정 */
	if (OwnerType == "Player") {
		TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
		SeekClass = AEnemy::StaticClass();
	}
	else if (OwnerType == "Enemy") {
		TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
		SeekClass = AMainPlayer::StaticClass();
	}

	/** 원하는 액터가 감지되었다면 True를 반환 */
	bool bResult = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Owner->GetActorLocation(), AttackRange, TraceObjectTypes, SeekClass, IgnoreActors, OutActors);

	FColor DrawColor = FColor::Red;
	if (bResult) {
		for (auto& HitActor : OutActors) {
			
			/** 내적 판단 */
			float Inner = Owner->GetDotProductTo(HitActor);	
			if (Inner > 0.3f) {
				if (OwnerType == "Player") {
					auto EHited = Cast<AEnemy>(HitActor);
					EHited->SetCurrentAttack(Owner->GetName() + EHited->GetName() + FString::FromInt(AttackCnt));
					if (EHited) UGameplayStatics::ApplyDamage(EHited, Damage, Controller, Owner, DamageType);
				}
				else if (OwnerType == "Enemy") {
					auto MHited = Cast<AMainPlayer>(HitActor);
					MHited->SetCurrentAttack(Owner->GetName() + MHited->GetName() + FString::FromInt(AttackCnt));
					if (MHited) UGameplayStatics::ApplyDamage(MHited, Damage, Controller, Owner, DamageType);
				}
				DrawColor = FColor::Green;
			}
		}
	}

	/** 피격 판정 시각화 용도 */
	DrawDebugSphere(GetWorld(), Owner->GetActorLocation(), AttackRange, 12, DrawColor, false, 0.5f);
}
void UAttackFunction::SpawnDamageText(FVector WorldLocation, float Damage, TSubclassOf<class UDamageTextWidget> DamageTextWidget, AController* DisplayController) {
	if(DamageTextWidget == nullptr) return;

	const APlayerController* DamageController = Cast<APlayerController>(DisplayController);
	WorldLocation.X += UKismetMathLibrary::RandomFloatInRange(-50.f, 50.f);
	WorldLocation.Y += UKismetMathLibrary::RandomFloatInRange(-50.f, 50.f);
	UGameplayStatics::ProjectWorldToScreen(DamageController, WorldLocation, DamageTextVec);
	DamageWidget = CreateWidget<UDamageTextWidget>(GetWorld(), DamageTextWidget);
	DamageWidget->SetInitialSetting(DamageTextVec, Damage);
}