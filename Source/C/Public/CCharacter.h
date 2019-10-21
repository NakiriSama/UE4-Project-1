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
*������Log��������Ϣ���࣬��������ʾ��Ӧ��ɫ
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

	/** �Ƿ�ʰȡ��Ʒ*/
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
		bool bIsCarryingObjective;

	/** ��ȡ�����λ��*/
	virtual FVector GetPawnViewLocation() const override;

	virtual void Tick(float DeltaTime) override;

	/**�Ƿ�����׼״̬*/
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool MarkingMode;

	/**�Ƿ������¶�*/
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool bIsCrouching;

	/**�Ƿ�����*/
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool IsDead;

	/**��ʼ�ƶ��ٶ�*/
	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float DefaultSpeed;

	/**��׼ʱ�ƶ��ٶ�*/
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

	/**�趨�ܷ�ʹ������*/
	void SetCanTakeCover(bool CanTakeCover, class ACoverActor* CoverActor);

	/**�趨�����ܷ�վ��*/
	void SetIsStandCover(bool HighCube);

	/**�������μ���*/
	UFUNCTION()
	void ToggleInvisibility();

	UFUNCTION()
		void ToggleReload();

	/**����͸�Ӽ���*/
	UFUNCTION()
	void ToggleXRay();

	/**���������ƶ�����ֵ������Private�������÷��̣���������ͼ�Լ�������*/
	UFUNCTION(BlueprintCallable, Category = "CoverSystem")
		float CoverValueInput() { return CoverValue; }

	/**��ȡ�Ƿ�������״̬������Private�������÷��̣���������ͼ�Լ�������*/
	UFUNCTION(BlueprintCallable, Category = "CoverSystem")
		bool GetIsInCover() { return IsInCover; }

	/**��ȡ�Ƿ���վ������״̬������Private�������÷��̣���������ͼ�Լ�������*/
	UFUNCTION(BlueprintCallable, Category = "CoverSystem")
		bool GetIsStandCover() { return IsStandCover; }

	/**��ȡ�Ƿ�������״̬������Private�������÷��̣���������ͼ�Լ�������*/
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
		bool GetIsInvisible() { return IsInvisible; }

	/**��ȡ�Ƿ�������ǽ�ǡ�����Private�������÷��̣���������ͼ�Լ�������*/
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
		bool GetIsInCorner() { return IsInCorner; }

	/**��ȡ�Ƿ�����׼״̬������Private�������÷��̣���������ͼ�Լ�������*/
	UFUNCTION(BlueprintCallable)
		bool GetIsInZoom() { return MarkingMode; }

	/**�趨���μ�����ȴ�ж�������Private�������÷��̣���������ͼ�Լ�������*/
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
		void SetCanBeInvisible();

	/**�趨͸�Ӽ�����ȴ�ж�������Private�������÷��̣���������ͼ�Լ�������*/
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
		void SetCanBeInXRay();

	/**��ʾ׼�ǡ�������ͼʵ�ַ���*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SpawnCrosshair();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SpreadCrosshair(float Velocity);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ConvergeCrosshair();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ResetTimeline();

	/**����׼�ǡ�������ͼʵ�ַ���*/
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

	/**͸�Ӽ��ܿ�ʼ����ʱ����*/
	UFUNCTION()
	    void XRayTimelineBegin(float Value);

	/**͸�Ӽ��ܽ�������ʱ����*/
	UFUNCTION()
		void XRayTimelineEnd(float Value);

	/**͸�Ӽ��ܽ�������ʱ������ɷ���*/
	UFUNCTION()
	    void XRayTimelineEndStateChange();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
		float FullInvisibleCooldown;

	/**���μ��ܳ���ʱ�俪ʼ��ʱ*/
	UFUNCTION()
		void InvisibleBeginDuration();

	/**���μ�����ȴʱ�俪ʼ��ʱ*/
	UFUNCTION()
		void InvisibleBeginCooldown();

	/**͸�Ӽ��ܳ���ʱ�俪ʼ��ʱ*/
	UFUNCTION()
		void XRayBeginDuration();

	/**͸�Ӽ�����ȴʱ�俪ʼ��ʱ*/
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


	/*�����������������ӽ��ƶ��Լ���ر���*/

	//����RelativeLocation���FOV������ı����λ��
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

	

	/** �Ƿ����Ļ��Log��ʾ */
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

	//������������������Ϊ״̬

	bool IsInCover;

	bool bCanFire;

	bool IsClosedToCorner;

	bool IsCrouchZooming;

	bool IsCrouchWalking;

	bool bCanTakeCover;

	//bool ZoonoutCooldown;
	//��������������������״̬

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

