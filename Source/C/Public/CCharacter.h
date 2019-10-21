// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"

#include "CCharacter.generated.h"

#define WEAPON_SOCKET_NORMAL "Weapon_Socket"
#define WEAPON_SOCKET_CROUCH "Weapon_Socket_C"
#define WEAPON_SOCKET_COVER "Weapon_Socket_Co"
/**
*调试用Log方程中信息分类，以用来显示对应颜色
*/
UENUM(BlueprintType)
enum class ELogLevel : uint8 {
	TRACE			UMETA(DisplayName = "Trace"),
	DEBUG			UMETA(DisplayName = "Debug"),
	INFO			UMETA(DisplayName = "Info"),
	WARNING			UMETA(DisplayName = "Warning"),
	ERROR			UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class ELogOutput : uint8 {
	ALL				UMETA(DisplayName = "All levels"),
	OUTPUT_LOG		UMETA(DisplayName = "Output log"),
	SCREEN			UMETA(DisplayName = "Screen")
};

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	MELEE_FIST			UMETA(DisplayName = "Melee - Fist")
};

struct Energy
{
	float Magic;
	float FullMagic;
	float MagicPercentage;
	float PreviousMagic;
	float MagicValue;

};

class ASkillTimer;
class USpringArmComponent;
class UCameraComponent;
class UPawnNoiseEmitterComponent;
class UHealthComponent;
class AMyWeapon;
class ACoverActor;
class APostProcessVolume;
class UCurveFloat;
class UMaterialParameterCollection;
class UUserWidget;
class UTimelineComponent;
class UBoxComponent;
class UUserWidget_AITracking;
class AAIGuard;


UCLASS(config=Game)
class ACCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	    USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	     UCameraComponent* FollowCamera;

public:
	ACCharacter();
	UFUNCTION(BlueprintCallable)
	AMyWeapon* GetWeaponComp() const { return CurrentWeapon; }
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	    float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	    float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE  USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE  UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** 是否拾取物品*/
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
		bool bIsCarryingObjective;

	/** 获取摄像机位置*/
	virtual FVector GetPawnViewLocation() const override;

	virtual void Tick(float DeltaTime) override;

	/**是否处于瞄准状态*/
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool MarkingMode;

	/**是否正在下蹲*/
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool bIsCrouching;

	/**是否死亡*/
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool IsDead;

	/**初始移动速度*/
	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float DefaultSpeed;

	/**瞄准时移动速度*/
	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float AimingSpeed;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	FText CurrentSurface;

	float Magic;
	float FullMagic;
	float MagicPercentage;
	float PreviousMagic;
	float MagicValue;

	float Velocity;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	//	float MaxTurn;

	/**设定能否使用掩体*/
	void SetCanTakeCover(bool CanTakeCover, class ACoverActor* CoverActor);

	/**设定掩体能否站立*/
	void SetIsStandCover(bool HighCube);

	/**触发隐形技能*/
	UFUNCTION()
	void ToggleInvisibility();

	UFUNCTION()
		void ToggleReload();

	/**触发透视技能*/
	UFUNCTION()
	void ToggleXRay();

	/**输入掩体移动浮点值———Private变量调用方程，适用于蓝图以及其他类*/
	UFUNCTION(BlueprintCallable, Category = "CoverSystem")
		float CoverValueInput() { return CoverValue; }

	/**获取是否处于掩体状态———Private变量调用方程，适用于蓝图以及其他类*/
	UFUNCTION(BlueprintCallable, Category = "CoverSystem")
		bool GetIsInCover() { return IsInCover; }

	/**获取是否处于站立掩体状态———Private变量调用方程，适用于蓝图以及其他类*/
	UFUNCTION(BlueprintCallable, Category = "CoverSystem")
		bool GetIsStandCover() { return IsStandCover; }

	/**获取是否处于隐形状态———Private变量调用方程，适用于蓝图以及其他类*/
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
		bool GetIsInvisible() { return IsInvisible; }

	/**获取是否处于掩体墙角———Private变量调用方程，适用于蓝图以及其他类*/
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
		bool GetIsInCorner() { return IsInCorner; }

	/**获取是否处于瞄准状态———Private变量调用方程，适用于蓝图以及其他类*/
	UFUNCTION(BlueprintCallable)
		bool GetIsInZoom() { return MarkingMode; }

	/**设定隐形技能冷却判定———Private变量调用方程，适用于蓝图以及其他类*/
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
		void SetCanBeInvisible();

	/**设定透视技能冷却判定———Private变量调用方程，适用于蓝图以及其他类*/
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
		void SetCanBeInXRay();

	/**显示准星———蓝图实现方程*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SpawnCrosshair();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SpreadCrosshair(float Velocity);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ConvergeCrosshair();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ResetTimeline();

	/**隐藏准星———蓝图实现方程*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void RemoveCrosshair();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SpawnClip();

	UFUNCTION(BlueprintPure, Category = "Magic")
		float GetMagic();

	UFUNCTION(BlueprintPure, Category = "Magic")
		float GetXRay();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SetbCanFire();

	FVector GetScreenCenter() { return CameraCenter; }

	/**透视技能开始动画时间线*/
	UFUNCTION()
	    void XRayTimelineBegin(float Value);

	/**透视技能结束动画时间线*/
	UFUNCTION()
		void XRayTimelineEnd(float Value);

	/**透视技能结束动画时间线完成方程*/
	UFUNCTION()
	    void XRayTimelineEndStateChange();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
		float FullInvisibleCooldown;

	/**隐形技能持续时间开始计时*/
	UFUNCTION()
		void InvisibleBeginDuration();

	/**隐形技能冷却时间开始计时*/
	UFUNCTION()
		void InvisibleBeginCooldown();

	/**透视技能持续时间开始计时*/
	UFUNCTION()
		void XRayBeginDuration();

	/**透视技能冷却时间开始计时*/
	UFUNCTION()
		void XRayBeginCooldown();

	UFUNCTION()
		void XRayTimelineBeginTimer();

	int8 GetCoverDirction() const { return CoverDirection; }


protected:
	void FootstepSurface();

	float CornerDirection;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float MouseAxisTurn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float MouseLookUpAxis;


	/*————————视角移动以及相关变量*/

	//已用RelativeLocation替代FOV，方便改变相机位置
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	    float ZoomFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player",meta = (ClampMin = 0.1 , ClampMax =100))
		float ZoominCameraSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
		float ZoomoutCameraSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
		float CoverCameraSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
		float CoverOutCameraSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	    float StandCoverSpeed;

	//float DefaultFOV;


	UPROPERTY(EditDefaultsOnly, Category = "CameraMove", meta = (ClampMin = -200, ClampMax = 200))
		FVector CenterCameraLocation;

	UPROPERTY(EditDefaultsOnly, Category = "CameraMove", meta = (ClampMin = -200, ClampMax = 200))
		FVector RightCameraCoverMove;

	UPROPERTY(EditDefaultsOnly, Category = "CameraMove", meta = (ClampMin = -200, ClampMax = 200))
		FVector LeftCameraCoverMove;

	UPROPERTY(EditDefaultsOnly, Category = "CameraMove", meta = (ClampMin = -200, ClampMax = 200))
		FVector RightCameraCornerMove;

	UPROPERTY(EditDefaultsOnly, Category = "CameraMove", meta = (ClampMin = -200, ClampMax = 200))
		FVector LeftCameraCornerMove;

	UPROPERTY(EditDefaultsOnly, Category = "CameraMove", meta = (ClampMin = -200, ClampMax = 200))
		FVector RightSocketOffset;

	UPROPERTY(EditDefaultsOnly, Category = "CameraMove", meta = (ClampMin = -200, ClampMax = 200))
		FVector LeftSocketOffset;

	UPROPERTY(EditDefaultsOnly, Category = "CameraMove", meta = (ClampMin = -200, ClampMax = 200))
		FVector MidSocketOffset;

	FVector DefaultSocketOffset;

	FVector RightCornerCamera;

	FVector LeftCornerCamera;

	

	/** 是否打开屏幕的Log显示 */
	UPROPERTY(EditDefaultsOnly, Category = "Log")
		bool OpenLog;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerEffect")
		UParticleSystem*  InvisibilityEffect;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void AITrack();

	void Marking();

	void QuitMarking();

	void BeginCrouch();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	    UPawnNoiseEmitterComponent* NoiseEmitterComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	    UHealthComponent* HealthComp;

	void SetMagicChange(float MagicChange);

	void UpdateMagic();

	void SetXRayChange(float XRayChange);

	void UpdateXRay();

	void StartFireOrStartAttack();

	void StopFireOrAttackEnd();

	void ToggleCover();

	void SetClosedToCorner();

	void SpawnTimer(ASkillTimer* &NewTimer, TSubclassOf <ASkillTimer> SkillClass);

	// ASkillTimer* CurrentSkillTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	    TSubclassOf <AMyWeapon> StartWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	    TSubclassOf <ASkillTimer> Invisibility;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	    TSubclassOf <ASkillTimer> XRay;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf <UUserWidget> CrosshairBPReference;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		TSubclassOf <UUserWidget_AITracking> AITrackingWidgetBP;

	UPROPERTY()
	    UUserWidget_AITracking* CurrentAITrackingWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	    USoundBase*  BeginInvisibleSound;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		USoundBase*  BeginXRaySound;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		USoundBase*  EndXRaySound;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	    USoundBase*  EndInvisibleSound;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		USoundBase*  MarkOnSound;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		USoundBase*  MarkOffSound;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	    int8 MaxMarkNumber;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* PickupAmmoSounds;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	    UMaterialParameterCollection* XRayParameter;

	AMyWeapon* CurrentWeapon;

	APostProcessVolume* InvisibilityPPV;

	APostProcessVolume* XRayPPV;

	ASkillTimer* InvisibilityTimer;

	ASkillTimer* XRayTimer;

	UUserWidget* Crosshair;

	FTimeline XRayCurveTimelineBegin;

	FTimeline XRayCurveTimelineEnd;

	FVector CameraCenter;

	UTimelineComponent* InvisibleDuration;

	UTimelineComponent* InvisibleCooldown;

	UTimelineComponent* XRayDuration;

	UTimelineComponent* XRayCooldown;

	UPROPERTY(EditAnywhere, Category = "Skill")
	    UCurveFloat* XRayCurveFloatBegin;

	UPROPERTY(EditAnywhere, Category = "Skill")
	    UCurveFloat* XRayCurveFloatEnd;

	UPROPERTY(EditAnywhere, Category = "Skill")
		UCurveFloat* InvisibleFloatDuration;

	UPROPERTY(EditAnywhere, Category = "Skill")
		UCurveFloat* InvisibleFloatCooldown;

	UPROPERTY(EditAnywhere, Category = "Skill")
		UCurveFloat* XRayFloatDuration;

	UPROPERTY(EditAnywhere, Category = "Skill")
		UCurveFloat* XRayFloatCooldown;

	UFUNCTION()
	void OnHealthChange(UHealthComponent* HealthC, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

	TQueue<AAIGuard*> MarkedAIQueue;
	TQueue<int> TestQueue;

	//virtual void Crouch(bool bClientSimulation = false) override;

	//virtual void UnCrouch(bool bClientSimulation = false) override;

private:

	virtual void AddControllerYawInput(float Val) override;

	virtual void AddControllerPitchInput(float Val) override;

	void PopElementinAIqueue(TQueue<AAIGuard*> &Queue, AAIGuard* Element);

	void PopElementinTest(TQueue<int> &Queue, int Element);
	 ACoverActor* Cover;

	//————————行为状态

	bool IsInCover;

	bool bCanFire;

	bool IsClosedToCorner;

	bool IsCrouchZooming;

	bool IsCrouchWalking;

	bool bCanTakeCover;

	//bool ZoonoutCooldown;
	//————————技能状态

	bool IsInvisible;

	bool IsInXRay;

	bool bCanInvisible;

	bool bCanXRay;

	bool IsInCorner;



	float CoverValue;

	float IsStandCover;

	float TimeLineValue;

	float TimeLineValue2;

	float CurveFloatValue;

	float CurveFloatValue2;

	int8 CoverDirection;

	int8 MarkNumber;

	FName WeaponSocketName = "Weapon_Socket";


	FName CrouchWeaponSocketName = "Weapon_Socket_C";

	FName CoverWeaponSocketName = "Weapon_Socket2";

	FVector DefaultCameraLocation;

	FVector V;

	FVector CoverDirectionMovement;

	FVector NewCameraLocation;
	
	Energy Invis;

	Energy Xray;


	/**
     * Log - prints a message to all the log outputs with a specific color
     * @param LogLevel {@see ELogLevel} affects color of log
     * @param FString the message for display
     */
	void Log(ELogLevel LogLevel, FString Message);
	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param LogLevel {@see ELogLevel} affects color of log
	* @param FString the message for display
	* @param ELogOutput - All, Output Log or Screen
	*/
	void Log(ELogLevel LogLevel, FString Message, ELogOutput LogOutput);



	


};

