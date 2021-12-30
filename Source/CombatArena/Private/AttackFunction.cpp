#include "AttackFunction.h"
#include "CollisionQueryParams.h"
#include "DamageTextWidget.h"	
#include "MainController.h"	
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UAttackFunction::UAttackFunction(){
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackFunction::BeginPlay(){
	Super::BeginPlay();
}

void UAttackFunction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttackFunction::SetOwner(USkeletalMeshComponent* TakeMesh,AController* TakeController ) {
	Owner = GetOwner();
	Mesh = TakeMesh;
	Controller = TakeController;
}

void UAttackFunction::SkillAttackStart(FVector Location, FVector Forward, TSubclassOf<UDamageType> DamageType,FString Type, UParticleSystem* HitParticle,float AttackRange,float Damage)
{
	//ã�Ƴ� ������ Ʈ���̽� ä��
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	//ã�Ƴ� ������ Ÿ��
	UClass* SeekClass = nullptr;

	//ã�Ƴ� ���͸� ������ �迭
	TArray<AActor*> OutActors;

	//���õ� ������ �迭
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(Owner, 1);

	if (Type == "Player") {
		TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
		SeekClass = AEnemy::StaticClass();
	}
	else if (Type == "Enemy") {
		TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
		SeekClass = AMainPlayer::StaticClass();
	}

	bool bResult = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Owner->GetActorLocation(), AttackRange, TraceObjectTypes, SeekClass, IgnoreActors, OutActors);

	/** Draw Image */
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), Owner->GetActorLocation(), AttackRange, 12, DrawColor, false, 0.5f);

	/** �������� �˰��� */
	if (bResult) {
		for (auto& HitActor : OutActors)
		{
			// ������ ũ��
			float Inner = Owner->GetDotProductTo(HitActor);
			if (Inner > 0.3f) {
				if (Type == "Player") Hited = Cast<AEnemy>(HitActor);
				else if (Type == "Enemy") Hited = Cast<AMainPlayer>(HitActor);
				if (Hited) {
					UGameplayStatics::ApplyDamage(Hited, Damage, Controller, Owner, DamageType);
				}
			}
		}
	}
}
void UAttackFunction::SpawnDamageText(FVector WorldLocation, float Damage, TSubclassOf<class UDamageTextWidget> DamageTextWidget, AController* DisplayController) {
	if(DamageTextWidget == nullptr) return;

	const APlayerController* DamageController = Cast<APlayerController>(DisplayController);
	WorldLocation.X += UKismetMathLibrary::RandomFloatInRange(-50.f, 50.f);
	WorldLocation.Y += UKismetMathLibrary::RandomFloatInRange(-50.f, 50.f);
	UGameplayStatics::ProjectWorldToScreen(DamageController, WorldLocation, DamageTextVec);
	DamageWidget = CreateWidget<UDamageTextWidget>(GetWorld(), DamageTextWidget);
	DamageWidget->InintialScreenLocation = DamageTextVec;
	DamageWidget->DamageToDisplay = Damage;
	DamageWidget->AddToViewport();
}