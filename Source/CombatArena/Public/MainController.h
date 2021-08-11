#pragma once

#include "EngineMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainController.generated.h"

UCLASS()
class COMBATARENA_API AMainController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainController();

	virtual void BeginPlay() override;

#pragma region WIDGET
	UPROPERTY(VisibleAnywhere, Category = "Widget | EnemyWidget")
	class UHealthWidget* HealthBarOutLine;

	UPROPERTY(VisibleAnywhere, Category = "Widget | EnemyWidget")
	class AMainPlayer* MainPlayer;

	//Health Bar
	UPROPERTY(VisibleAnywhere, Category = "Widget | EnemyWidget")
	class UUserWidget* PlayerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | EnemyWidget")
	TSubclassOf<class UUserWidget> WPlayerMainHealth;

	UFUNCTION()
	void SetPlayerHealth();		//Player�� ü�� UI ����. MainPlayer���� ȣ��
#pragma endregion
};
