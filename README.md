# Battle_Arena
- 포트폴리오 용도의 언리얼 프로젝트 

> **<h3>Input Keys</h3>**
|key|info|
|:--:|:--:|
|||

## **07.27**
> **<h3>Today Dev Story</h3>**
- ## <span style = "color:yellow;">기초적인 설정</span>
  - ### __GameMode & Character & PlayerController 제작__
    1. ### Character에 Camera,SpringArm 부착 및 설정 
    - <img src="Image/CameraMovement.gif" height="300" title="Movement">
    - GameMode에 PlayerController를 기본값을 지정 
      ```c++
        PlayerControllerClass = AMainController::StaticClass();
      ```
    - Character에 UCameraComponent, USpringArmComponent를 지정
      1. Camera는 SpringArm의 소켓에 Attach
      2. SpringArm은 bUsePawnControllRotaion 기능을 키고 플레이어가 조정이 가능하도록 지정. (Camera는 false)
      3. 카메라의 회전을 키기 위해서 SpringArm->bInheritPitch... = true;
      4. 카메라의 회전이 플레이어에게 영향을 받지 않도록 bUseControllerRotationPitch... = false;
      5. AddControllerYawInput를 사용하여 카메라의 회전
      <details><summary>C++ 코드</summary> 

        ```c++
        //MainPlayer.cpp
        AMainPlayer::AMainPlayer()
        {
          ...
          SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SringArm"));
          SpringArm->SetupAttachment(GetRootComponent());
          SpringArm->bUsePawnControlRotation = true;	//플레이어가 컨트롤 할 수 있게 만들어줌
          
          //카메라 회전 On
          SpringArm->bInheritPitch = true;
          SpringArm->bInheritRoll = true;
          SpringArm->bInheritYaw = true;

          SpringArm->TargetArmLength = 450.f;

          Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
          Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
          Camera->bUsePawnControlRotation = false;

          //회전해도 플레이어에게 영향 X
          bUseControllerRotationYaw = false; 
          bUseControllerRotationRoll = false; 
          bUseControllerRotationPitch = false; 
        }
        ...
        ...

        void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
        {
          Super::SetupPlayerInputComponent(PlayerInputComponent);

          PlayerInputComponent->BindAxis(FName("Lookup"), this, &AMainPlayer::Lookup);
          PlayerInputComponent->BindAxis(FName("Turn"), this, &AMainPlayer::Turn);
        }
        ...
        ...
        void AMainPlayer::Lookup(float value) {
	        AddControllerYawInput(value * CameraSpeed* GetWorld()->GetDeltaSeconds());  //CameraSpeed 지정
        }

        void AMainPlayer::Turn(float value) {
          AddControllerPitchInput(value * CameraSpeed*GetWorld()->GetDeltaSeconds());
        }
        ```
      </details>
      <details><summary>h 코드</summary> 

        ```h
        //MainPlayer.h
        #pragma region CAMERA

          UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
          class USpringArmComponent* SpringArm;

          UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
          class UCameraComponent* Camera;

        private:

          UPROPERTY(EditDefaultsOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
          float CameraSpeed = 12.f;

        public:
          void Lookup(float value);

          void Turn(float value);

        #pragma endregion
        ```
      </details>

    2. ### 캐릭터의 이동을 위한 구현
      - <img src="Image/Movement.gif" height="300" title="Movement">
      - AddMovementInput함수를 사용해 구현하며 __Camera의 시점을__ 기준으로 움직이도록 구현 (이동, 점프)
      - GetCharacterMovement함수를 사용해 각종 컨트롤 수정 (이동방향으로 캐릭터를 자동 회전, 회전 속도, 공중 컨트롤 퍼센트)  
        <details><summary>c++ 코드</summary> 

          ```c++
          AMainPlayer::AMainPlayer()
          {
            ...
            GetCharacterMovement()->bOrientRotationToMovement = true;	//이동방향 자동회전
            GetCharacterMovement()->RotationRate = FRotator(0.f,540.f,0.f);
            GetCharacterMovement()->JumpZVelocity = 600.f;	//점프 크기   
            GetCharacterMovement()->AirControl = 0.5f;      //공중 컨트롤
          }
          void AMainPlayer::MoveForward(float Value) {
            AddMovementInput(Camera->GetForwardVector(), Value);  //액터의 방향이 아닌 카메라의 방향으로
          }
          void AMainPlayer::MoveRight(float Value) {
            AddMovementInput(Camera->GetRightVector(), Value);
          }
          void AMainPlayer::Jump() {
            Super::Jump();
          }
          ```
        </details>

        <details><summary>h 코드</summary> 
        
        ```h
        #pragma region MOVEMENT
        public:
          void MoveForward(float Value);

          void MoveRight(float Value);

        private:
          UPROPERTY(EditDefaultsOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
          float MoveSpeed;

        #pragma endregion
        
        ```
        </details>

  - #### __달리기 (Sprinting)__ 
    - <img src="Image/Sprinting.gif" height="300" title="Sprinting">
    - MainPlayer에 enum 클래스로 상태를 제작하여 뛰는 것과 걷는 것 구별 (속도에 차이를 두기 위함)
    - Shift키에 바인딩했으며 Switch_Sprinting 이라는 함수 하나만을 만들어 상태와 속도 전환
    - 실직적으로는 SetMovementStatus() 함수에서 들어오는 값에 따라 MaxWalkSpeed에 속도를 지정 (추후 추가예정)
      <details><summary>c++ 코드</summary> 

        ```c++
        void AMainPlayer::BeginPlay()
        {
          Super::BeginPlay();
          SetMovementStatus(EMovementStatus::EMS_Normal);
        }

        void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
        {
          PlayerInputComponent->BindAction("Shift", EInputEvent::IE_Pressed, this, &AMainPlayer::Switch_Sprinting);
          PlayerInputComponent->BindAction("Shift", EInputEvent::IE_Released, this, &AMainPlayer::Switch_Sprinting);
        }
        ...
        void  AMainPlayer::SetMovementStatus(EMovementStatus Status) {
          MovementStatus = Status;
          if (MovementStatus == EMovementStatus::EMS_Sprinting) GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
          else GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
        }
        void AMainPlayer::Switch_Sprinting() {
          if(MovementStatus != EMovementStatus::EMS_Sprinting) SetMovementStatus(EMovementStatus::EMS_Sprinting);
          else SetMovementStatus(EMovementStatus::EMS_Normal);
        }
        ```
      </details>

      <details><summary>h 코드</summary> 

        ```h
        UENUM(BlueprintType)
        enum class EMovementStatus : uint8 {
          EMS_Normal		UMETA(DisplayName = "Normal"),
          EMS_Sprinting	UMETA(DisplayName = "Sprinting"),
          
          EMS_Default		UMETA(DisplayName = "Default")
        };

        ...
          void Switch_Sprinting();
        private:
          UPROPERTY(EditDefaultsOnly, Category = "Movement", Meta = (AllowPrivateAccess = true))
          float SprintingSpeed;

          //현재 캐릭터의 상태를 표현 (스턴,러닝,정지,공격 등등)
          UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = true))
          EMovementStatus MovementStatus;	

          void SetMovementStatus(EMovementStatus Status);
        ```
      </details>

  - #### __Character Animation 제작__ 
    - <img src="Image/Way_Directional_Movement.gif" height="300" title="Way_Directional_Movement">
    - <img src="Image/Main_Blend.png" height="300" title="Main_Blend">
    - AnimInstance 클래스(MainPlayerAnim.cpp) 제작후 BP만들고 위 그림과 같은 Blend 제작
    - bUseControllerRotationYaw 만을 true로 전환하여 카메라 회전시 플레이어도 회전하도록 지정 (위와 같은 애니메이션 제작을 원하기에)
      - MainPalyer의 정보를 얻어와야 하기 때문에 헤더에 선언
      - Anim에서 'Update Animation 이벤트'에 내가 만든 UpdateAnimationProperties()함수를 연결하여 애니메이션에 필요한 데이터를 갱신한다. 
      - 필요한 데이터는 점프여부, 속도, 방향.
      <details><summary>c++ 코드</summary> 

        ```c++
        #include "MainPlayer.h"

        void UMainPlayerAnim::NativeInitializeAnimation() {
          if (!MainPlayer) MainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());
        } 

        void UMainPlayerAnim::UpdateAnimationProperties() {
          if (!MainPlayer) MainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());

          if (MainPlayer) {
            FVector Speed = MainPlayer->GetVelocity();
            FRotator Rotator = MainPlayer->GetActorRotation();
            FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
            MovementSpeed = LateralSpeed.Size();
            DirectionSpeed = CalculateDirection(Speed, Rotator);

            bIsInAir = MainPlayer->GetMovementComponent()->IsFalling();
          }

        }
        ```
      </details>

      <details><summary>h 코드</summary> 

        ```h
        public:
          virtual void NativeInitializeAnimation() override;	//생성시 동작

          UFUNCTION(BlueprintCallable, Category = AnimationProperties)
          void UpdateAnimationProperties();					//틱마다 동작

          UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
          class AMainPlayer* MainPlayer;

          UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
          bool bIsInAir;

          UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
          float MovementSpeed;

          UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
          float DirectionSpeed;
        ```
      </details>

> **<h3>Realization</h3>**
- GameMode->Character->Controller->AnimInstance->Blend
