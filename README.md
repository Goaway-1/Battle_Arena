# Battle_Arena (미완)

|목록|내용|
|:--:|:--:|
|작업 기간|2021.07.26 ~ 2022.01.30 (실기간 2달)|
|인력 구성|1인 개발|
|프로젝트 목적|포트폴리오 & 기술 향상|
|프로젝트 내용|MMORPG 형식의 게임을 구상하고 시작했으나, 거대한 틀은 잡지 못하고 기능들만 구현|
|주요 업무|기획 및 개발|
|사용언어|C++|
|개발 환경|Visual Studio Code, Visual Studio 2019, Unreal Engine 4.26.2|
|참고 자료|Unreal Docs, Google|
|개발일지|[날짜별 기록이 남긴 일지](https://github.com/Goaway-1/Battle_Arena/blob/master/DevLog.md)|


> ### **<h3>목차</h3>**
1. [동영상 및 개발 일지](#동영상-및-개발-일지)
2. [프로젝트 설명](#프로젝트-설명)
3. [프로젝트 시스템 구조](#프로젝트-시스템-구조)
4. [주요 기능](#주요-기능)
5. [개선점](#개선점)

> ### **<h3>동영상 및 개발 일지</h3>**
- [<img src="Image/Docs/MainImage.png" height="300" title="클릭 시 이동">](https://youtube.com)

- ## [개발일기](https://github.com/Goaway-1/Battle_Arena/blob/master/DevLog.md)

> ### **<h3>프로젝트 설명</h3>**
- MMORPG형식의 게임으로 초기 다크소울 느낌의 소울라이크 게임을 원했으나, 밸런스 등은 해결하지 않고 기능을 중심적으로 개발하는데 노력
- 게임의 재미와 기능 모두 완벽하게 하고 싶었지만 기능, 전투 시스템에만 중점으로 개발
- 무기를 구하고 몬스터들을 잡아 최종 보스를 처치하면 종료

> ### **<h3>프로젝트 시스템 구조</h3>**
- ※ 화살표는 __상속을__ 뜻하며, 점선은 클래스를 해당 클래스에 내에서 __인스턴스화__ 했다는 것을 의미
- <img src="Image/Docs/WeaponAndSkill.png" height="200" title="구조 이미지">
  
  - Item클래스는 플레이어와 상호작용하며, Potion은 효과를 부여하고 Weapon은 장착하여 사용
  - Meteor, Grenade등과 같은 클래스들은 플레이어나 적이 SkillFunction클래스에서 인스턴스화하여 사용
- <img src="Image/Docs/AttackAndSkillFunction.png" height="200" title="AttackAndSkillFunction">
  
  - 플레이어와 적이 사용하는 Attack/SkillFunction클래스로 컴포넌트로 생성하여 사용
  - AttackFunction클래스는 공격하는 2가지 방식 중 내적을 사용하여 공격 하는 방식
  - SkillFunction클래스는 Meteor, Lazer등과 같이 클래스들을 인스턴스화하여 사용
- <img src="Image/Docs/AIBT.png" height="400" title="AIBT">

  - 비헤이비어트리(BehaviorTree)를 구성할때 사용하는 노드들로 UBTDecorator는 조건을 판단하고, UBTaskNode는 실제로 행동하는 노드
- <img src="Image/Docs/MainPlayer.png" height="400" title="MainPlayer">
  
  - Character를 상속받은 클래스로 플레이어를 뜻하며 열거형(ENUM)을 사용하여 상태를 나타냄
  - Controller는 화면상에 표시하는 위젯들을 관리하고, Anim은 조건(속도, 방향)에 따른 애니메이션을 관리
- <img src="Image/Docs/Enemy.png" height="400" title="Enemy">

  - Character를 상속받은 클래스로 적을 뜻하며 이는 다시 Normal_Enemy와 Boss_Enemy로 구분
    - 가장 큰 차이점은 SkillFunction, Balance의 유무
  - 적의 EnemyController에서는 비헤이비어트리(BehaviorTree)와 BlackBoard, AIPerception..등을 사용하여 로직 구성
  - 나머지는 플레이어와 비슷하게 구성

TurnInPlace, 무기장착 로직, 공격 판정2가지, 콤보, 방패, SkillFunction..
> ### **<h3>주요기능</h3>**
※ 내용에 적혀있는 코드는 일부만을 작성한 것으로 자세한 내용이나 구조는 개발일기나 소스파일을 참조
## __1. TurnInPlace__
  - <img src="Image/Docs/TurnInPlace.gif" height="300" title="TurnInPlace">
  - <img src="Image/Docs/TurnInPlaceAnim.png" height="300" title="TurnInPlaceAnim">
  - 설명 : 이동 속도와 카메라 회전 방향에 따른 플레이어의 회전으로
### __작업 내용__
- __메서드명__ : TurnInPlace(float value)
  - 설명 : 움직임이 없이 제자리에서 카메라를 이동했을때 플레이어가 자연스럽게 회전하는 애니메이션을 사용하는 메서드
  - 플레이어 컨트롤러에서 GetPlayerViewPoint()메서드를 사용하여 카메라의 Location, Rotation을 받아와 캐릭터의 Rotation과의 차이 값을 도출
  - 그 값이 0.5도 이내라면 회전하지 않고, 45도 이상 차이가 나고 속도가 없다면 TurnAxis의 값을 value에 따라 1, -1로 지정
  - 이때 value는 회전 방향 구분하며 Left(-1)/Idle(0)/Right(1)로 지정
  - TurnAxis의 값이 변경됨에 따라 MainPlayerAnim에서 애니메이션 실행 (위 그래프에서 확인)

- __메서드명__ : TurnMove()
  - 설명 : 플레이어가 움직일때 즉시 이동방향으로 회전하도록 하는 메서드
  - CharacterMovementComponent의 OrientRotationToMovement를 거짓으로 하여 캐릭터의 회전을 움직임과 동일하게 구현

### __호출 방식__
  - 마우스 좌우 이동 시 Turn()메서드가 실행되는데, 캐릭터 이동속도가 0.1이하면 TurnInPlace()메서드가 그렇지 않고 이동 중이라면 TurnMove()메서드 실행

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  //MainPlayer.cpp
  void AMainPlayer::Turn(float value) {
    if (GetMovementStatus() == EMovementStatus::EMS_Faint) return;
    AddControllerYawInput(value * CameraSpeed * GetWorld()->GetDeltaSeconds());

    /** 이동속도에 따른 메서드 실행 */
    if (GetVelocity().Size() <= 0.1f) TurnInPlace(value);
    else TurnMove();
  }
  void AMainPlayer::TurnInPlace(float value) {
    FVector ViewPoint;
    FRotator ViewRotation;
    float calculationY;
    
    PlayerController->GetPlayerViewPoint(ViewPoint, ViewRotation);
    calculationY = UKismetMathLibrary::Abs(ViewRotation.Yaw - GetActorRotation().Yaw);

    /** 카메라와 액터 사이의 회전 차이에 따른 구분 */
    if (calculationY <= 0.5f) {
      GetCharacterMovement()->bUseControllerDesiredRotation = false;
      TurnAxis = 0;
    }
    else if (calculationY >= 45.f || (DirX != 0 || DirY != 0)) {
      GetCharacterMovement()->bUseControllerDesiredRotation = true;
      if (GetVelocity().Size() == 0) {
        if (value > 0.1f) TurnAxis = 1;
        else if (value < -0.1f) TurnAxis = -1;
      }
    }
  }
  void AMainPlayer::TurnMove() {
    GetCharacterMovement()->bOrientRotationToMovement = false;	
    FRotator ViewRotation = CameraManager->GetCameraRotation();
    ViewRotation.Pitch = ViewRotation.Roll = 0;
    SetActorRotation(ViewRotation);
  }
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  //MainPlayer.h
  private:
    //애니메이션 실행을 위한 값
  	int TurnAxis = 0;	
  public:
    void Turn(float value);
    void TurnInPlace(float value);
    void TurnMove();
    FORCEINLINE int GetTurnAxis() { return TurnAxis; }
  ```
  </details>

---
## __2. 플레이어의 이동__
  - <img src="Image/Docs/MainPlayerAnim.png" height="300" title="MainPlayerAnim">
  - <img src="Image/Docs/MainBlendSpace.png" height="300" title="MainBlendSpace">
  - 설명 : 애니메이션을 관리하는 MainPlayerAnim클래스의 구조와 상하체를 구분하는 블랜딩
### __작업 내용__
  - BlendSpace를 사용하여 Axis Settings의 가로, 세로축을 각각 Dircection(방향)과 - Speed(속도)로 지정하여 Axis에 따라 애니메이션를 자연스럽게 변화
  - 이외의 점프나 Combat/Weapon/MovementStatus에 따른 애니메이션은 블랜드(Blennd Poses by bool : bool에 따른 구분)와 스테이트머신을 사용하여 위 그림과 같이 구현.
  - 블랜드는 상체와 하체를 구분하여 다양한 연출을 가능하도록 도와줌

---
## __3. 아이템 (무기, 포션)과의 상호 작용__
  - <img src="Image/Docs/Interactive.gif" height="300" title="Interactive">
  - 설명 : 플레이어가 아이템과 상호작용하는 로직으로 Item클래스에 Overlap로직 구현
### __작업 내용__
- __클래스명__ : MainPlayer클래스의 De/ActiveInteraction(), ItemDrop/Equip()메서드
  - 설명 : 무기나 포션에 상호작용하는 로직
  - 아이템이 겹쳐진다면 MainPlayer에 __ActiveOverlappingItem을__ 해당 아이템으로 설정
  - 'E'키를 눌러 ActiveOverlappingItem이 null이 아니라면 ActiveInteraction(), ItemEquip()를 호출하며 해당 아이템의 ItemType이 EIT_Weapon(무기)일 경우 CurrentWeapon에 Weapon을 어태치하여 장착하고, EIT_Item(아이템)일 경우 사용
  - 'Q'키를 눌러 ItemDrop()을 호출하며 CurrentWeapon을 DetachFromActor()를 사용하여 사용자로 부터 제거, 기존에 무기가 존재할때 장착 시 자동으로 ItemDrop()호출

- __클래스명__ : Weapon클래스
  - 설명 : 플레이어에게 장착되어 사용
  - Weapon클래스에서는 EWeaponPos라는 UENUM클래스로 무기의 종류를 구분하며, Weapon을 상속받은 AttackWeapon클래스에서는 EWeaponName라는 ENUM클래스로 근접 무기의 종류를 구분
  - Weapon에 CollisionVolume을 설정하여 Overlap 된다면 MainPlayer에 ActiveOverlappingItem을 해당 Weapon으로 설정
  - Equip()메서드에서는 기존 무기가 있다면 해제하고 WeaponPos에 따라서 Attach하고 무기의 특성인 AttackRange,Damage...등과 같은 것을 플레이어에게 설정

- __클래스명__ : Potion클래스
  - 설명 : 플레이어와 상호작용 시 효과를 부여하는데, UseItem(float &Health)메서드를 통해 CurrentHealth를 증가 (주소로 받아 처리)

### __호출 방식__
  - 사용자와 Overlap되면 사용자가 E,Q키를 사용하여 상호작용

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  //MainPlayer.cpp
  void AMainPlayer::ActiveInteraction() {
    if(!IsCanMove()) return;

    /** Active SpecialAttack || Active Item, Weapon */
    float Inner = this->GetDotProductTo(BalanceTarget);
    if (Inner > 0.3f && bCanSpecialAttack && !bAttacking) ActiveSpecialAttack();
    else if (ActiveOverlappingItem != nullptr) ItemEquip();
  }
  void AMainPlayer::DeactiveInteraction() {
    if (GetWeaponStatus() != EWeaponStatus::EWS_Normal) ItemDrop();
  }
  void AMainPlayer::ItemEquip() {	
    /** 겹처진 아이템의 타입이 무기인 경우 무기 장착 */
    if (ActiveOverlappingItem->GetItemType() == EItemType::EIT_Weapon) {
      AWeapon* CurWeapon = Cast<AWeapon>(ActiveOverlappingItem);
      CurWeapon->Equip(this);

      AAttackWeapon* ACurWeapon = Cast<AAttackWeapon>(CurWeapon);
      if (ACurWeapon) ACurWeapon->SetAttackInit(PlayerController, this, PlayerDamageType);
    }
    /** 아이템인 경우 사용 */
    else if (ActiveOverlappingItem->GetItemType() == EItemType::EIT_Item) {
      APotion* Potion = Cast<APotion>(ActiveOverlappingItem);
      Potion->UseItem(CurrentHealth);
      SetHealthRatio();
    }
    SetActiveOverlappingItem(nullptr);
  }
  void AMainPlayer::ItemDrop() {
    /** 무기가 존재한다면 해제 */
    if (CurrentAttackWeapon != nullptr) {	
      CurrentAttackWeapon->UnEquip();
      CurrentAttackWeapon = nullptr;
      AttackDamage = DefaultDamage;
      AttackRange = DefaultAttackRange;
    }
    /** 방패가 존재한다면 해제 */
    else if (CurrentShieldWeapon != nullptr) {	
      CurrentShieldWeapon->UnEquip();
      CurrentShieldWeapon = nullptr;
    }
    if (CurrentAttackWeapon == nullptr) SetWeaponStatus(EWeaponStatus::EWS_Normal);
  }
  ```
  ```c++
  //Item.cpp
  void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    if (OverlapParticle) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator(0.f), true);
    if (OtherActor) {
      AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);

      /** MainPlayer의 ActiveOverlappingItem을 this로 지정 */
      if (Player) Player->SetActiveOverlappingItem(this);
    }
  }
  void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (OtherActor) {
      AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
      if (Player) Player->SetActiveOverlappingItem(nullptr);
    }
  }
  ```
  ```c++
  //AttackWeapon.cpp
  void AAttackWeapon::Equip(class AMainPlayer* Player) {
    Super::Equip(Player);

    if (Player) {
      if ((GetWeaponPos() == EWeaponPos::EWP_Shield && Player->GetShieldCurrentWeapon() != nullptr) || (GetWeaponPos() == EWeaponPos::EWP_Melee && Player->GetAttackCurrentWeapon() != nullptr)) {
        Player->ItemDrop();
      }

      /** WeaponPos에 따른 Attach */
      const USkeletalMeshSocket* HandSocket = nullptr;
      if (GetWeaponPos() == EWeaponPos::EWP_Melee) HandSocket = Player->GetMesh()->GetSocketByName("MeleeWeapon");
      else if (GetWeaponPos() == EWeaponPos::EWP_Shield) HandSocket = Player->GetMesh()->GetSocketByName("ShieldWeapon");
      else if (GetWeaponPos() == EWeaponPos::EWP_Bow) HandSocket = Player->GetMesh()->GetSocketByName("BowWeapon");

      if (HandSocket) {
        HandSocket->AttachActor(this, Player->GetMesh());
        Player->SetWeaponStatus(EWeaponStatus::EWS_Melee);

        /** InitalSetting */
        Player->SetAttackRange(GetAttackRange());		
        Player->SetAttackCurrentWeapon(this);
        Player->SetAttackDamage(GetDamage());	

        CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
      }
    }
  }
  ```
  ```c++
  //Potion.cpp
  void APotion::UseItem(float &Health) {
    Health += 10.f;
    if (Health >= 100.f) Health = 100.f;
    if (UsedSound != nullptr) UGameplayStatics::PlaySound2D(this, UsedSound);
    if (OverlapParticle) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator(0.f), true);
    Destroy();
  }
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  //MainPlayer.h
  private:
  	UPROPERTY(VisibleAnywhere, Category = "Active | Weapon")
    class AItem* ActiveOverlappingItem;   /** 현재 겹쳐진 아이템 */

    UPROPERTY(VisibleAnywhere, Category = "Active | Weapon")
    class AShieldWeapon* CurrentShieldWeapon;

    UPROPERTY(VisibleAnywhere, Category = "Active | Weapon")
    class AWeapon* CurrentAttackWeapon;
  public:
	  FORCEINLINE void SetActiveOverlappingItem(AItem* item) { ActiveOverlappingItem = item; }
  	FORCEINLINE AShieldWeapon* GetShieldCurrentWeapon() { return CurrentShieldWeapon; }
	  FORCEINLINE AWeapon* GetAttackCurrentWeapon() { return CurrentAttackWeapon; }
    void ItemEquip();
	  void ItemDrop();
  ```
  ```c++
  //Item.h
  /** 아이템의 타입 */
  UENUM(BlueprintType) 
  enum class EItemType : uint8 {
    EIT_Normal		UMETA(DisplayName = "Normal"),
    EIT_Weapon		UMETA(DisplayName = "Weapon"),
    EIT_Item		UMETA(DisplayName = "Item"),

    EIT_Default		UMETA(DisplayName = "Default")
  };

  /** 아이템의 장착 여부 */
  UENUM(BlueprintType)
  enum class EItemState : uint8
  {
    EIS_Normal	UMETA(DisplayName = "Normal"),
    EIS_Ground  UMETA(DisplayName = "Ground"),
    EIS_Equip	UMETA(DisplayName = "Equip"),

    EIS_Default	UMETA(DisplayName = "Default")
  };
  public:
  	UFUNCTION()
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
  ```
  ```c++
  //AttackWeapon.h
  /** 무기의 종류 */
  UENUM(BlueprintType)
  enum class EWeaponPos : uint8 {
    EWP_Empty		UMETA(DIsplayName = "Empty"),
    EWP_Shield		UMETA(DisplayName = "Shield"),
    EWP_Melee		UMETA(DisplayName = "Melee"),
    EWP_Bow			UMETA(DisplayName = "Bow"),
    EWP_Full		UMETA(DisplayName = "Full"),

    EWP_Default		UMETA(DisplayName = "Default")
  };

  private:
    UPROPERTY(EditAnywhere, Category = "Weapon", Meta = (AllowPrivateAccess = true))
    float AttackRange;

    UPROPERTY(EditAnyWhere, Category = "Weapon", Meta = (AllowPrivateAccess = true))
    float Damage;
  public:
	  virtual void Equip(class AMainPlayer* Player) override;
    FORCEINLINE float GetAttackRange() { return AttackRange; }
	  FORCEINLINE float GetDamage() { return Damage; }
  ```
  ```c++
  //Potion.h
  public:
	  void UseItem(float &Health);
  ```
  </details>

---
## __4. 공격판정 (무기)__
  - <img src="Image/Docs/Attack_1.gif" height="300" title="Attack_1">
  - <img src="Image/Docs/Attack_1_Notify.png" height="300" title="Attack_1_Notify">
  - 설명 : 플레이어가 무기를 장착하고 하는 무기의 콜리전을 활용한 공격 판정 방법
### __작업 내용__
- __클래스명__ : AttackWeapon클래스
  - 설명 : AttackWeapon의 CapsuleComponent 콜리전을 On/Off하는 방식으로 Overlap되면 피격 판정
  - On/OffWeaponCollision()메서드를 애니메이션 노티파이에서 호출하여 Weapon의 콜리전을 비/활성화 
  - 필요한 정보를 무기를 착용할때 ItemEquip()메서드에서 SetAttackInit()메서드에서 넘겨 사용
  - 무기를 한번 휘두를 때 중첩되어 2번 이상 피해를 받는 오류가 발생할 수 있기에 공격 시 “공격자의 이름 + 무기의 이름 + 카운트 번호”로 CurrentAttack을 설정. 
  - AttakCnt를 매 차례 증가하여 위 로직에 도움이 되도록 설정 (MainPlayer의 Get/SetAttackCnt()메서드)
    ```c++
    void AMainPlayer::SetAttackCnt() {
      AttackCnt++;
      if(AttackCnt > 10) AttackCnt = 0;
    }
    ```
  - Enemy클래스의 CurrentAttack이름과 LastAttack이름을 비교하여 다를 시 TakeDamage()메서드 실행

### __호출 방식__
  - 사용자가 무기를 장착하고 마우스 왼쪽 버튼 클릭 시 호출 시 LMBDown()메서드 호출

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  //AttackWeapon.cpp
  /** AttackCollision의 Active여부 */
  void AAttackWeapon::SetAttackCollision(bool value) {	
    if(!value) AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    else AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  }
  void AAttackWeapon::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    if (OtherActor) {
      AEnemy* Enemy = Cast<AEnemy>(OtherActor);
      AMainPlayer* Player = Cast<AMainPlayer>(AtOwner);
      ABoss_Enemy* BEnemy = Cast<ABoss_Enemy>(Enemy);
      if (BEnemy) Player->GetPlayerController()->SetBalanceTarget(BEnemy);  //보스인 경우 Balance를 띄움
      if (Enemy) {  /** 적의 CurrentAttack정보를 갱신하고 ApplyDamage()메서드로 피격 처리 */
        Enemy->SetCurrentAttack(AtOwner->GetName() + this->GetName() + FString::FromInt(Player->GetAttackCnt()));
        UGameplayStatics::ApplyDamage(Enemy, Damage, AtController, AtOwner, AtDamageType);
      }
    }
  }
  ```
  ```c++
  //Enemy.cpp
  float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
    if(CurrentHealth <= 0) return 0;
    else if (LastAttack != CurrentAttack ) LastAttack = CurrentAttack;      /** 마지막 피격과 최근 피격정보가 다른 경우 LastAttack을 갱신하고 피격처리 */
    else if (DamageEvent.DamageTypeClass != InternalDamageType) return 0;
    else return 0;
    ....
  }
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  //AttackWeapon.h
  private:
  	UPROPERTY(EditAnywhere, Category = "Weapon", Meta = (AllowPrivateAccess = true))
	  UCapsuleComponent* AttackCollision;
  public:
  	UFUNCTION()
    void OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    void SetAttackCollision(bool value);
  ```
  ```c++
  //Enemy.h
  private:
    UPROPERTY(VisibleAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
    FString LastAttack = "";    /** 마지막 피격 정보 */

    UPROPERTY(VisibleAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
    FString CurrentAttack = "";  /** 지금 피격 정보 */
  public:
	  FORCEINLINE void SetCurrentAttack(FString Value) { CurrentAttack = Value; }
  ```
  </details>

---
## __5. 공격판정 (내적)__
  - <img src="Image/Docs/Attack_2.gif" height="300" title="Attack_2">
  - <img src="Image/Docs/Attack_2_Notify.png" height="300" title="Attack_2_Notify">
  - <img src="Image/Docs/TraceChannel.png" height="300" title="TraceChannel">
  - 설명 : 기존 무기의 콜리전 방식이 아닌 트레이스 채널과 내적을 사용한 공격 방식
### __작업 내용__
- __클래스명__ : AttackFunction클래스
  - 설명 : SkillAttackStart()메서드에서 공격 판정을 진행
  - AttackFunction클래스에 공격메서드를 추상화하여 구상
  - SetOwner()메서드를 통해서 계산에 필요한 정보를 미리 넘긴다. 이는 클래스를 사용할 모든 객체에서 PossessedBy()에서 반드시 호출해야만함.
  - 트레이스채널, ECollisionChannel의 타입 Player일때는 2번(Enemy), Enemy일때는 1번(Player) 채널로 선언하고 __UKismetSystemLibrary::SphereOverlapActors()메서드를__ 사용하여 소유자 주변을 채널과 일치하는 타입의 액터를 찾아 두 액터 간 내적의 크기가 일정 범위내라면 피격처리
  - 이때 찾아낼 액터를 저장할 배열(OutActors), 트레이스 채널번호(TraceObjectTypes), 클래스(SeekClass)이 필요하고 무시할 액터를 저장한 배열(IgnoreActors)이 필요하며, 배열은 모두 Array를 사용
  - 위의 그림처럼 내적의 크기를 기준으로 피격을 판단하며 __장점은 공격타입에 따라 범위를 지정할 수 있다는 점이다.__

### __호출 방식__
  - BTTask_Attack()메서드에 의해 Attack()이 호출되며 이는 애니메이션 노티파이를 실행하고, 특정 타이밍에 AttackStart_Internal()메서드가 실행.
  - 적과 플레이어 모두 사용가능 하지만 적의 공격 방식은 이 내적 방식으로만 구현

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  //AttackFunction.cpp
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
    if (Type == "Player") {
      TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
      SeekClass = AEnemy::StaticClass();
    }
    else if (Type == "Enemy") {
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
          if (Type == "Player") {
            auto EHited = Cast<AEnemy>(HitActor);
            EHited->SetCurrentAttack(Owner->GetName() + EHited->GetName() + FString::FromInt(AttackCnt));
            if (EHited) UGameplayStatics::ApplyDamage(EHited, Damage, Controller, Owner, DamageType);
          }
          else if (Type == "Enemy") {
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
  ```
  ```c++
  //Enemy.cpp
  void AEnemy::AttackStart_Internal() {
    FString Type = "Enemy";
    /** 트레이스채널을 사용한 공격 호출 */
    AttackFunction->SkillAttackStart(InternalDamageType, Type, GetHitParticle(),GetAttackRange(), AttackDamage, AttackCnt);

    AttackCnt++;
    if (AttackCnt > 2) AttackCnt = 0;
  }
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  //AttackFunction.h
  UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
  class COMBATARENA_API UAttackFunction : public UActorComponent{
  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackFunction | Owner")
    class AActor* Owner;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackFunction | Owner")
    class AController* Controller;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackFunction | Owner")
    class USkeletalMeshComponent* Mesh;

  public:
    UFUNCTION()
    void SetOwner(USkeletalMeshComponent* TakeMesh, AController* TakeController);

    UFUNCTION(BlueprintCallable)
    virtual void SkillAttackStart(TSubclassOf<UDamageType> DamageType, FString OwnerType, UParticleSystem* HitParticle, float AttackRange, float Damage, int AttackCnt);
  }
  ```
  ```c++
  //Enemy.h
  private:
    class UEnemyAttackFunction* AttackFunction;
  ```
  </details>

---
## __6. 콤보 구현__
  - <img src="Image/Docs/Combo.gif" height="300" title="Combo">
  - <img src="Image/Docs/Combo_Anim.png" height="300" title="Combo_Anim">
  - 설명 : 연속공격을 구현한 로직
### __작업 내용__
- __함수명__ : AttackInputCheck()메서드
  - 연속 공격을 체크(AttackCheck)하는 노티파이를 MainPlayer클래스의 AttackInputCheck()와 연결
  - 그 시점에 AttackInputCheck()메서드가 실행되는데 이때 만약 bIsAttackCheck, 즉 공격이 가능하다면 콤보카운트를 증가 시키고 Attack()메서드 실행
  - Attack()메서드에서 기존 몽타주가 실행되고 있기다면 끊고 다음 몽타주를 실행 
  - GetAttackMontageSection(int32 Section)에서는 입력번호에 따라 FName이 반환되는데 이를 사용하여 콤보 몽타주를 1씩 증가하여 사용
  - 초기 설정시 콤보의 개수를 정하는 ComboMaxCount를 설정해줘야 하고, 반드시 공격 노티파이의 이름은 Attack으로 시작하며 시작번호는 0이다.

### __호출 방식__
  - 애니메이션 진행 중 노티파이에 의한 AttackInputCheck() 호출

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  //MainPlayer.cpp
  void AMainPlayer::Attack(bool bIsSpecial) {
    ...
    else if (AnimInstance && PlayMontage)	
    { 
      /** 기존 PlayMontage 애니메이션이 실행되지 않고 있을때 */
      if (!AnimInstance->Montage_IsPlaying(PlayMontage)) {
        AnimInstance->Montage_Play(PlayMontage);
        ComboCnt = 0;
      }
      /** 기존 PlayMontage 애니메이션이 실행 중일때 */
      else {													
        AnimInstance->Montage_Play(PlayMontage);
        AnimInstance->Montage_JumpToSection(GetAttackMontageSection("Attack", ComboCnt), PlayMontage);
      }
    }
  }
  void AMainPlayer::AttackInputCheck() {
    if (bIsAttackCheck) {
      ComboCnt++;
      if (ComboCnt >= ComboMaxCnt) ComboCnt = 0;
      bIsAttackCheck = false;
      Attack();
    }
  }
  /** 타입에 따른 다음 공격 모션을 반환 */
  FName AMainPlayer::GetAttackMontageSection(FString Type, int32 Section) {
    if (Type == "Attack") return FName(*FString::Printf(TEXT("Attack%d"), Section));
    else if (Type == "Dodge") return FName(*FString::Printf(TEXT("Dodge%d"), Section));
    else return "Error";
  }
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  //MainPlayer.h
  private:
	  bool bIsAttackCheck;  //콤보 체크 관련 변수
  public:
  	UFUNCTION(BlueprintCallable)
	  void AttackInputCheck();

  	void Attack(bool bIsSpecial = false);
	  FName GetAttackMontageSection(FString Type, int32 Section);
  ```
  </details>

---
## __7. 균형__
  - <img src="Image/Docs/Balance.gif" height="300" title="Balance">
  - 설명 : 하단에 뜨는 보라색 바가 플레이어의 균형(Balance)이고, 상단에 뜨는 파란색 바가 적의 균형.
  - '세키로'에 존재하는 균형시스템을 구현한 것으로 피격이 중첩되어 균형이 무너지면 액터는 일정 시간 동안 기절
### __작업 내용__
- __클래스명__ : MainPlayer클래스
  - 설명 : 공격을 막거나 피해를 받을 시 균형(Balance)가 흐트러지며 일정량 도달하면 기절
  - Player는 enum 클래스의 EMovementStatus에 EMS_Faint라는 기절 상태가 있는데 Lookup(),Turn()메서드에서 회전을 금지하였고, IsCanMove()메서드에도 조건을 추가하여 움직임을 금지.
  - 피격 시 균형이 흐트러지고, 일정량에 도달하면 BrokenBalance()메서드를 통해 기절 상태로 변경하고, 타이머를 활용하여 1.5초뒤에 RecoverBalance()메서드를 사용하여 기절 상태를 복구
  - 만약 일정량에 도달하지 않았다면 위와 비슷하세 타이머와 람다식을 사용하여 SetDecreaseBalance(true)로 균형을 되찾으며 계속해서 데미지를 받는다면 균형이 회복되지 않는다.
  - Enemy클래스에는 bool 타입인 bIsFainted을 추가하여 BTTask_Faint노드 실행

- __클래스명__ : Balance클래스
  - 균형을 담당하는 클래스로 BalancePercent()메서드는 TickComponent내에서 호출되며 외부에 의해 bIsDecreaseBalance가 True로 전환된다면 균형을 점차 회복

### __호출 방식__
  - 플레이어나 적이 피격 시 TakeDamage()메서드 내에서 호출

    <details><summary>Cpp File</summary> 

    ```c++
    //MainPlayer.cpp
    float AMainPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
      ...
      /** Active Shield */
      if (GetCombatStatus() == ECombatStatus::ECS_Blocking) {
        if (IsBlockingSuccess(DamageCauser)) {
          Balance->SetCurrentBalance(10.f);
          return 0;
        }
      }
      ...
      /** Balance */
      Balance->SetCurrentBalance(20.f);
      Balance->SetDecreaseBalance(false);
      if (Balance->GetCurrentBalance() >= 100.f) {
        /** 균형이 무너지며 상태 변경 */
        BrokenBalance();
        return DamageAmount;
      }
      else {
        /** DecreaseBalanceTime동안 다시 피격되지 않는다면 Balance를 회복 */
        GetWorldTimerManager().ClearTimer(BalanceHandle);
        GetWorldTimerManager().SetTimer(BalanceHandle, FTimerDelegate::CreateLambda([&] { Balance->SetDecreaseBalance(true);}), DecreaseBalanceTime, false);
      }
      SetBalanceRatio();
      ...
    }
    ```
    </details>

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  //Balance.cpp
  void UBalance::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    BalancePercent(); //Balance를 점차 회복
  }

  void UBalance::BalancePercent() {
    if (bIsDecreaseBalance && Currentbalance > 0.f) {
      Currentbalance -= 0.1f;
      if (Currentbalance < 0.f) Currentbalance = 0.f;
    }
    BalanceRatio = Currentbalance / Maxbalance;
  }
  ```
  ```c++
  //MainPlayer.cpp
  void AMainPlayer::SetBalanceRatio() {
    if (Balance->GetCurrentBalance() > 0.f) {
      PlayerController->SetPlayerBalance();
    }
  }
  void AMainPlayer::BrokenBalance() {
    /** 상태 변경 및 RecoverBalance()메서드 타이머로 호출 */
    Balance->SetCurrentBalance(-100.f);
    SetMovementStatus(EMovementStatus::EMS_Faint); 
    GetWorldTimerManager().ClearTimer(BalanceHandle);
    GetWorldTimerManager().SetTimer(BalanceHandle, this, &AMainPlayer::RecoverBalance , 1.5f, false);
    
    /** Play Animation */
    if(!FaintMontage) return;
    AnimInstance->Montage_Play(FaintMontage);
    AnimInstance->Montage_JumpToSection("Faint", FaintMontage);
  }
  void AMainPlayer::RecoverBalance() {
    /** 상태 복구 */
    if(GetMovementStatus() != EMovementStatus::EMS_Faint) return;
    AnimInstance->Montage_Stop(0.1f);
    SetMovementStatus(EMovementStatus::EMS_Normal);
  }
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  //Balance.h
  private:
    UPROPERTY(VisibleAnywhere, Category = "BALANCE")
    float Currentbalance;

    UPROPERTY(VisibleAnywhere, Category = "BALANCE")
    float Maxbalance;

    UPROPERTY(VisibleAnywhere, Category = "BALANCE")
    float BalanceRatio;

    UPROPERTY(VisibleAnywhere, Category = "BALANCE")
    bool bIsDecreaseBalance;

  public:
    UFUNCTION()
    void BalancePercent();

    FORCEINLINE float GetMaxBalance() { return Maxbalance; }
    FORCEINLINE float GetCurrentBalance() { return Currentbalance; }
    FORCEINLINE float GetBalanceRatio() { return BalanceRatio; }
    FORCEINLINE void SetBalanceRatio(float value) { BalanceRatio = value; }
    FORCEINLINE void SetCurrentBalance(float value) { Currentbalance += value; }
    FORCEINLINE void SetDecreaseBalance(bool value) { bIsDecreaseBalance = value; }
  ```
  ```c++
  //MainPlayer.h
  private:
    class UBalance* Balance;
    FTimerHandle BalanceHandle;
    float DecreaseBalanceTime;
  public:
    void SetBalanceRatio();       //현재 Balance값을 계산
    void BrokenBalance();         //균형이 무너진 후 기절을 표현
    void RecoverBalance();        //기절 후 1.5초 뒤 실행되는 메서드

    FORCEINLINE UBalance* GetBalance() { return Balance; }
  ```
  </details>

---
## __8. 특정 공격__
  - <img src="Image/Docs/EnemyFainted.gif" height="300" title="EnemyFainted">
  - 설명 : 적의 균형이 무너졌을때 E를 사용하여 특별한 공격을 진행
### __작업 내용__
- __클래스명__ : MainPlayer클래스
  - MainPlayer에 새로운 SphereComponent(EnemyBalanceOverlap)를 추가하여 범위 이내의 적을 파악하고, 해당 적을 BalanceTarget으로 설정
  - 매 틱마다 BalanceTarget이 없거나, 있지만 기절 상태가 아닌 경우는 bCanSpecialAttack은 false, 있고 기절 상태인 경우에는 true로 설정
  - 적이 스턴상태일때 특정 키(E)를 사용하여 처리하며, Enemy클래스에는 SpecialHit()메서드를 제작하여 특별한 경우의 피격을 처리 (우선순위 : SpecialAttack -> EquipItem)
  - Attack()메서드에 매개변수에 따라 true인 경우 특정 공격을 처리 
  - ActiveSpecialAttack()메서드에서는 Attack()메서드의 매개변수가 true로 넘겨 특별한 공격을 하며 이때 ZoomInCam()메서드를 사용하여 카메라는 특정위치로 이동하고, 적의 애니메이션 실행

### __호출 방식__
  - E를 사용하여 조건에 적합하다면 특별한 공격을 진행

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  //MainPlayer.cpp
  void AMainPlayer::Attack(bool bIsSpecial) {
    ...
    if (bIsSpecial) {
      AnimInstance->Montage_Play(PlayMontage);
      AnimInstance->Montage_JumpToSection("SpecialAttack", PlayMontage);
      ZoomInCam(SpringArm_Attacking, FRotator(0.f, -60.f, 0.f));
    }
  }
  void AMainPlayer::ActiveInteraction() {
    if(!IsCanMove()) return;
    float Inner = this->GetDotProductTo(BalanceTarget);

    /** 조건을 만족한다면 특정 공격 실행 */
    if (Inner > 0.3f && bCanSpecialAttack && !bAttacking) ActiveSpecialAttack();
    else if (ActiveOverlappingItem != nullptr) ItemEquip();
  }
  void AMainPlayer::ActiveSpecialAttack() {
    BalanceTarget->SpecialHitMontage();
    Attack(true);   //매개변수 true를 넘겨 SpecailAttack 몽타주 실행
  }
  void AMainPlayer::CanSpeicalAttackToEnemy() {
    if(BalanceTarget == nullptr) bCanSpecialAttack = false;
    if (BalanceTarget) {
      if (BalanceTarget->GetIsFainted() && !bCanSpecialAttack) bCanSpecialAttack = true;
      else if(!BalanceTarget->GetIsFainted()) bCanSpecialAttack = false;
    }
  }
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  //MainPlayer.h
  private:
  	class ABoss_Enemy* BalanceTarget; 
	  USphereComponent* EnemyBalanceOverlap;
  public:
	  void CanSpeicalAttackToEnemy();	  //현재 적에게 특정 공격이 가능한지 체크
    void ZoomInCam(USpringArmComponent* Arm, FRotator Rot = FRotator(0.f));
	  void ZoomOutCam();
  ```
  </details>

---
## __9. 연막탄__
  - <img src="Image/Docs/IsInFog_Enemy.gif" height="300" title="IsInFog_Enemy">
  - <img src="Image/Docs/IsInFog_Player.gif" height="300" title="IsInFog_Player">
  - 설명 : 연막안에 들어가면 플레이어의 시점은 흐려지고, 적은 방황하는 로직
### __작업 내용__
- __클래스명__ : Grenade클래스
  - Grenade클래스의 SetFire()는 Player가 클릭하여 던질때 활성화되며 이때 SimulationEnabled가 활성화되어 Projectile을 사용한다. (Projectile : 발사체를 사용할때 추가하는 컴포넌트로 일시적인 힘을 제공)
  - 던지고 일정 시간 후에 SpawnSmoke()메서드가 활성화되어 연기가 생성되고, 일정 시간 후에 IsGrowing이 True가 되어 Tick마다 연막의 범위가 넓어지다가 소멸된다.
  - 3번 키를 누르면 EMovementStatus를 Throwing로 변경하고 오른쪽 손에 Grenade를 인스턴스화 하고 다시 누르면 삭제하며, 이때 마우스 왼쪽 클릭 시 던진다.
  - 적 중첩 시 : 
    - TaskNode클래스인 UBTTask_IsInFog클래스는 Enemy가 Grenade에 영향을 받는다면 실행하는 로직 생성.
    - Enemy클래스의 De/ActiveFogEvent()메서드를 통해 애니메이션의 실행
  - 플레이어 중첩 시 :
    - MainController클래스와 연관되어 SetFogSplatter()메서드를 통해 위젯의 Visibility를 On/Off 하는 형식

### __호출 방식__
  - MainPlayer클래스의 Throw/StartThrow/Throwing/EndThrow()메서드에서 진행. (코드 생략)

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  //Grenade.cpp
  void AGrenade::Tick(float DeltaTime){
    Super::Tick(DeltaTime);

    /** 조건 만족 시 연막의 범위 증가 */
    if (isGrowing && Smoke) {
      float Csize = Collision->GetUnscaledSphereRadius();
      Csize += (DeltaTime * 100.0f);
      Collision->SetSphereRadius(Csize);

      FVector temp = Smoke->GetRelativeScale3D();
      temp += FVector(DeltaTime * 0.3f);
      Smoke->SetRelativeScale3D(temp);
      SmokeTime -= DeltaTime;

      /** 일정 범위 초과 및 시간 소요 시 파괴*/
      if(Csize > 700.f && SmokeTime <= 0) {
        isGrowing = false;
        GetWorldTimerManager().ClearTimer(SpawnSmokeHandle);
        GetWorldTimerManager().SetTimer(SpawnSmokeHandle, this, &AGrenade::DestorySmoke, SpawnSmokeTime, false);
      }
    }
  }
  void AGrenade::SetFire(FRotator Rot) {
    /** 플레이어로부터 Detach하고 방향 나아갈 방향 지정 */
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    SetActorLocation(GetActorLocation());
    
    /** 회전과 Projectile을 사용한 발사 */
    Mesh->SetRelativeRotation(Rot);
    Projectile->SetVelocityInLocalSpace(FVector::ForwardVector * 8000);
    Projectile->Activate();

    /** SpawnSmokeTime 이후 연기 생성 */
    GetWorldTimerManager().SetTimer(SpawnSmokeHandle, this, &AGrenade::SpawnSmoke, SpawnSmokeTime, false);
  }
  void AGrenade::SpawnSmoke() {
    /** 연기 생성 */
    if(Smoke) {
      Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
      Smoke->SetVisibility(true);
      GetWorldTimerManager().ClearTimer(SpawnSmokeHandle);
      GetWorldTimerManager().SetTimer(SpawnSmokeHandle, this, &AGrenade::GrowingSmoke, SpawnSmokeTime, false);
    }
  }
  void AGrenade::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    if (OtherActor) {
      AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
      AEnemy* Enemy = Cast<AEnemy>(OtherActor);
      if (Player) Player->SetFogSplatter();                 //플레이어의 시야 방해
      if (Enemy) Enemy->SetVisibleInFog(true, SmokeTime);   //적의 방황 애니메이션 실행 (SmokeTime동안)
    }
  }
  ```
  ```c++
  //MainPlayer.cpp
  void AMainPlayer::SetFogSplatter() {
    (PlayerController->GetFogSplatterVisible()) ? PlayerController->RemoveFogSplatter() : PlayerController->DisplayFogSplatter();
  }
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  //Grenade.h
  private:
    UPROPERTY(EditAnywhere, Category = "Grenade | Mesh", Meta = (AllowPrivateAccess = true))
    class UProjectileMovementComponent* Projectile;

    UPROPERTY(EditAnywhere, Category = "Grenade | Smoke", Meta = (AllowPrivateAccess = true))
    class UParticleSystemComponent* Smoke;

    bool isGrowing = false;
  public:
    void SpawnSmoke();
    void DestorySmoke();
    void SetFire(FRotator Rot);
  ```
  ```c++
  //MainPlayer.h
    void SetFogSplatter();	
  ```
  </details>

---
## __10. 활과 화살__
  - <img src="Image/Docs/Bow_Arrow.gif" height="300" title="Bow_Arrow">
  - <img src="Image/Docs/Bow_ChargeAmount.png" height="300" title="Bow_ChargeAmount">
  - 설명 : 공격 무기 중 하나로 활을 사용하여 화살을 발사한다. 위의 그림은 BowWeapon클래스의 Begin/EndCharge를 연장한 블루프린트 구조이다. 
### __작업 내용__
- __클래스명__ : BowWeapon 
  - Eqiup 방식은 기존 Weapon과 동일한 방식으로 진행되고 양손 무기이다.
  - enum타입인 EBowStatus를 제작하여 당기는 중인지 상태를 구분
  - Begin/EndCharge_Implementation()메서드와 TimeLine을 사용하여 ChargeAmount의 크기에 따라 활시위 애니메이션 진행 (블루프린트와 연계하기 위해 메서드를 BlueprintCallable로 설정) 
  - Fire()메서드는 화살을 발사하는 메서드로 Arrow클래스의 Fire를 ChargeAmount를 힘으로 하여 발사
  - Reload()메서드는 Arrow를 "arrow_attach_socket"의 소켓에 Attach하여 활시위 당길 때 같이 이동

- __클래스명__ : Arrow
  - 화살의 상태를 나타내기 위해서 enum타입인 EArrowStatus를 제작하여 상태를 구분. 
  - Fire() 는 활의 ChargeAmount를 받아 FirePower값을 설정. 이 값을 Lerp()를 사용하여 화살을 AddImpulse하여 발사한다. 또한 DetachFromActor()를 사용하여 사용자로 부터 해제하고 KeepWorldTransform으로 전환하고, 상태 또한 InArrow로 변환. 
  - Arrow클래스에서 Overlap을 위해 SphereComponent를 추가하고 Overlap시 피격처리 (이전 피격처리와 동일) 
  - 발사 시 Collsion의 이름은 Arrow로 설정하여 바꾸고 발사. (기존과 동일) 
  - Projectile로 제작할 수 있지만 학습을 위해서 AddImpulse로 진행

### __호출 방식__
  - MainPlayer클래스의 Begin/EndCharge()메서드를 마우스 오른쪽 클릭을 통해 호출되며 이때 Bow클래스의 Reload(), BeginCharge()가 호출되고 상태 변경 및 카메라를 스위칭한다.
  - 또한 LMBDown()메서드에서 현재 WeaponPos가 Bow라면 Bow클래스의 Fire()메서드 호출되면 화살을 발사

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  //BowWeapon.cpp
  void ABowWeapon::BeginCharge_Implementation() {
    if(!Arrow) return;
    if(DrowSound != nullptr) UGameplayStatics::PlaySound2D(this, DrowSound);
  }
  void ABowWeapon::EndCharge_Implementation() {
    if (Arrow) {
      Arrow->Destroy();
      Arrow = nullptr;
    }
  }
  void ABowWeapon::Fire(int Cnt) {
    if(!Arrow) return;

    /** 화살의 발사 */
    Arrow->Fire(ChargeAmount, Cnt);
    Arrow = nullptr;
    EndCharge();
  }
  void ABowWeapon::Reload() {
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    /** 화살이 현재 없다면 재생성하고 활에 Attach */
    if (!Arrow) {
      Arrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, FVector(0.f), FRotator(0.f), SpawnParams);
      Arrow->AttachToComponent(GetSkeletalMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("arrow_attach_socket"));
      Arrow->InitalArrow(BowOwner,BowController);
    }
  }
  ```
  ```c++
  //Arrow.cpp
  void AArrow::SetBowState() {
    switch (ArrowStatus)
    {
    case EArrowStatus::EAS_InBow:
      ArrowCollision->SetCollisionProfileName(FName("NoCollision"));
      ArrowCollision->SetSimulatePhysics(false);
      break;
    case EArrowStatus::EAS_InArrow:
      ArrowCollision->SetCollisionProfileName(FName("Arrow"));
      ArrowCollision->SetSimulatePhysics(true);
      /** 화살의 발사 */
      if(!bisFire){
        ArrowCollision->AddImpulse(GetActorForwardVector() * UKismetMathLibrary::Lerp(600, 15000, FirePower));
        bisFire = true;
      }
      break;
    default:
      break;
    }
  }
  void AArrow::Fire(float Amount,int Cnt) {
    /** 활로 부터 Detach하고 상태 변경 */
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    SetArrowStatus(EArrowStatus::EAS_InArrow);
    FirePower = Amount;
    this->AttackCnt = Cnt;

    if(ShotSound != nullptr) UGameplayStatics::PlaySound2D(this, ShotSound);
  }
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  //BowWeapon.h
  UENUM(BlueprintType)
  enum class EBowStatus : uint8 {
    EBS_Normal		UMETA(DisplayName = "Normal"),
    EBS_Drawing		UMETA(DisplayName = "Drawing"),
    EBS_Default		UMETA(DisplayName = "Default")
  };
  private:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon | Bow", Meta = (AllowPrivateAccess = true))
    class AArrow* Arrow;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon | Bow", Meta = (AllowPrivateAccess = true))
    TSubclassOf<class AArrow> ArrowClass;

    AActor* BowOwner;
    AController* BowController;
  public:
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Arrow")
    float ChargeAmount;   //활을 당기고 있는 정도 

    UFUNCTION(BlueprintCallable)
    void SetChargeAmount(float value);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void BeginCharge();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void EndCharge();

    void Fire(int Cnt);   //화살을 발사
    void Reload();        //화살을 리로드
  ```
  ```c++
  //Arrow.h
  UENUM(BlueprintType)
  enum class EArrowStatus : uint8 {
    EAS_InBow		UMETA(DisplayName = "InBow"),
    EAS_InArrow		UMETA(DisplayName = "InArrow"),
    EAS_Destroy		UMETA(DisplayName = "Destroy")
  };
  private:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arrow | Physic", Meta = (AllowPrivateAccess = true))
    class URadialForceComponent* RadiaForce;    //Destructible Mesh에 사용했던 힘컴포넌트, 현재 사용 X
    
    UPROPERTY(EditDefaultsOnly, Category = "Arrow | Damage", Meta = (AllowPrivateAccess = true))
    float Damage = 5.f;

    UPROPERTY(VisibleAnywhere, Category = "Arrow")
    bool bisFire = false;

    UPROPERTY(VisibleAnywhere, Category = "Arrow")
    float FirePower;

    UPROPERTY(VisibleAnywhere, Category = "Arrow")
    EArrowStatus ArrowStatus;
  public:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    void Fire(float Amount,int Cnt);            //화살의 발사로 상태만 변경
    void SetArrowStatus(EArrowStatus Status);   //매 틱마다 호출되며 상태에 따른 행동
  ```
  </details>

---
## __11. 방패__
  - <img src="Image/Docs/" height="300" title="">
  - 설명 : 
### __작업 내용__
- __클래스명__ : 
  - 설명 :

### __호출 방식__
  - 

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  ```
  </details>

---
## __12. 타켓팅__
  - <img src="Image/Docs/" height="300" title="">
  - 설명 : 
### __작업 내용__
- __클래스명__ : 
  - 설명 :

### __호출 방식__
  - 

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  ```
  </details>

---
## __13. 카메라의 이동__
  - <img src="Image/Docs/" height="300" title="">
  - 설명 : 
### __작업 내용__
- __함수명__ : ZoomInCam()
  - 매개변수를 이동을 원하는 SpringArm과 회전값 Rotator로 입력받음. 이때 Rotator는 기본 0.f로 고정. 
  - 카메라를 특정 SpringArm에 어태치 하고 자연스럽게 이동

### __호출 방식__
  - 

### __참조 코드__

  <details><summary>Cpp File</summary> 

  ```c++
  //MainPlayer.cpp
  void AMainPlayer::SetArms(USpringArmComponent* Arm) {
    Arm->SetupAttachment(SpringArmSence);
    Arm->bUsePawnControlRotation = true;
    Arm->bInheritPitch = true;
    Arm->bInheritRoll = true;
    Arm->bInheritYaw = true;
  }
  void AMainPlayer::ZoomInCam(USpringArmComponent* Arm,FRotator Rot) {
    /** 특정 SpringArm으로 어태치 */
    Camera->AttachToComponent(Arm, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this;
    UKismetSystemLibrary::MoveComponentTo(Camera, FVector(0.f), Rot,false,false,0.3f, true,EMoveComponentAction::Type::Move,LatentInfo);
  }
  void AMainPlayer::ZoomOutCam() {
    /** 기존 SpringArm으로 어태치 */
    Camera->AttachToComponent(SpringArm, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this;
    UKismetSystemLibrary::MoveComponentTo(Camera, FVector(0.f), FRotator(0.f), false, false, 0.4f, true, EMoveComponentAction::Type::Move, LatentInfo);
  }
  ```
  </details>
  <details><summary>Header File</summary> 

  ```c++
  //MainPlayer.h
  private:
  	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
    class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
    class USpringArmComponent* SpringArm_Sprinting;
    
    UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
    class USpringArmComponent* SpringArm_Attacking;
    
    UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
    class USpringArmComponent* SpringArm_Drawing;
    
    UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
    class USpringArmComponent* SpringArm_Skilling;
  public:
  	void SetArms(USpringArmComponent* Arm);
  	void ZoomInCam(USpringArmComponent* Arm, FRotator Rot = FRotator(0.f));
	  void ZoomOutCam();
  ```
  </details>