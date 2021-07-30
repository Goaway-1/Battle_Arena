#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8 {
	EMS_Normal		UMETA(DisplayName = "Normal"),
	EMS_Walk		UMETA(DisplayName = "Walk"),
	EMS_Sprinting	UMETA(DisplayName = "Sprinting"),
	
	EMS_Default		UMETA(DisplayName = "Default")
};

UCLASS()
class COMBATARENA_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AMainPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region CAMERA

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	float CameraSpeed = 20.f;

public:
	void Lookup(float value);

	void Turn(float value);

#pragma endregion

#pragma region MOVEMENT
public:
	void MoveForward(float Value);

	void MoveRight(float Value);

	virtual void Jump() override;

	void Switch_Sprinting();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float SprintingSpeed;

	//현재 캐릭터의 상태를 표현 (스턴,러닝,정지,공격 등등)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = true))
	EMovementStatus MovementStatus;	

	void SetMovementStatus(EMovementStatus Status);

	//현재 속도를 측정하여 Idle과 walk를 구분 (카메라의 회전 변경)
	void CheckIdle();

	/** Dodge */
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Dodge")
	float DodgeSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Dodge")
	float DodgeCoolDownTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Dodge")
	float DodgeStopTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Dodge")
	bool bCanDodge;

	//방향
	UPROPERTY(VisibleAnywhere, Category = "Movement | Dodge")
	float DirX;

	UPROPERTY(VisibleAnywhere ,Category = "Movement | Dodge")
	float DirY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Dodge")
	FTimerHandle DodgeHandle;

	UFUNCTION()
	void Dodge();

	UFUNCTION()
	void DodgeEnd();

	UFUNCTION()
	void ResetDodge();

	UFUNCTION()
	void AnimDodge();

#pragma endregion

#pragma region ATTACK
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	class UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* AttackMontage;

	bool bLMBDown;			//공격 키가 눌렸는지 여부
	bool bAttacking;		//공격중인지 여부
	bool bIsAttackCheck;	//또 공격할 건지에 대한 여부
	int ComboCnt;			//현재 공격 횟수
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	int ComboMaxCnt;		//최대 공격 횟수

	void Attack();

	UFUNCTION(BlueprintCallable)
	void EndAttack();

	UFUNCTION(BlueprintCallable)
	void AttackInputCheck();

	void LMBDown();
	FORCEINLINE void LMBUp() { bLMBDown = false; }

	FName GetAttackMontageSection(FString Type,int32 Section);

#pragma endregion

};
