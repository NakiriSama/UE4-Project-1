// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "MyWeapon.h"
#include "SkillTimer.h"
#include "Math/UnrealMathUtility.h"
#include "IConsoleManager.h"
#include "TimerManager.h"
#include "C.h"
#include "HealthComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "CoverActor.h"
#include "Particles/ParticleSystem.h"
#include "Engine/PostProcessVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "PhysicalMaterials//PhysicalMaterial.h"
#include "MyAnimInstance.h"


/////////////////////////////////////////////////


ACCharacter::ACCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//初始化数据
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
//	MaxTurn = 0;
	DefaultSpeed = 400;
	AimingSpeed = 250;
	StandCoverSpeed = 250;
	CoverDirection = 1;
	////////////////////////////
	
	Invis.FullMagic = 100.0f;
	Xray.FullMagic = 100.0f;
	Invis.Magic = 100.0f;
	Xray.Magic = 100.0f;
	Invis.MagicPercentage = 1.0f;
	Xray.MagicPercentage = 1.0f;
	Invis.PreviousMagic = 1.0f;
	Xray.PreviousMagic = 1.0f;
	Invis.MagicValue = 0.0f;
	Xray.MagicValue = 0.0f;
	
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	
	IsDead = false;
	IsCrouchWalking = false;
	IsStandCover = false;
	IsInCover = false;
	IsInvisible = false;
	IsInXRay = false;
	IsInCorner = false;
	IsClosedToCorner = false;
	MarkingMode = false;
	bCanTakeCover = false;
	bCanInvisible = true;
	bCanXRay = true;
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	OpenLog = true;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(TRACECHANNEL_WEAPON, ECR_Ignore);
	
	
	ZoominCameraSpeed = 15.0f;
	ZoomoutCameraSpeed = 3.0f;
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	//自定义CameraLag
	CameraBoom->CameraRotationLagSpeed = 30;
	CameraBoom->CameraLagSpeed = 10;
	CameraBoom->CameraLagMaxDistance = 100;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	NoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitterComp"));
	InvisibleCooldown = CreateDefaultSubobject<UTimelineComponent>(TEXT("InvisibleCooldown"));
	InvisibleDuration = CreateDefaultSubobject<UTimelineComponent>(TEXT("InvisibleDuration"));
    XRayCooldown  = CreateDefaultSubobject<UTimelineComponent>(TEXT("XRayCooldown"));
	XRayDuration = CreateDefaultSubobject<UTimelineComponent>(TEXT("XRayDuration"));
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));


	//ZoomFOV = 60.0f;
	WeaponSocketName = WEAPON_SOCKET_NORMAL;

	//GetController()->SetControlRotation(SpringArmRotationTarget);
	
	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ACCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings

	check(PlayerInputComponent);
	//暂时去掉跳跃动作，将Space绑定到Cover上
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &ACCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//	PlayerInputComponent->BindAxis("LookUpRate", this, &ACCharacter::LookUpAtRate);

		// handle touch devices


	//移动输入按键绑定
	PlayerInputComponent->BindAction("Mark", IE_Pressed, this, &ACCharacter::Marking);
	PlayerInputComponent->BindAction("Mark", IE_Released, this, &ACCharacter::QuitMarking);

	//下蹲输入按键绑定
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACCharacter::BeginCrouch);
    
	//掩体按键绑定
	PlayerInputComponent->BindAction("TakeCover", IE_Pressed, this, &ACCharacter::ToggleCover);

	//开火按键绑定
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACCharacter::StartFireOrStartAttack);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACCharacter::StopFireOrAttackEnd);


	//触发隐身按键绑定
	PlayerInputComponent->BindAction("Invisibility", IE_Pressed, this, &ACCharacter::ToggleInvisibility);

	//触发透视按键绑定
	PlayerInputComponent->BindAction("XRay", IE_Pressed, this, &ACCharacter::ToggleXRay);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ACCharacter::ToggleReload);
}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();
	DefaultSocketOffset = RightSocketOffset;
	//AMyWeapon* CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>()
	DefaultCameraLocation = FollowCamera->GetRelativeTransform().GetLocation();
	RightCornerCamera = LeftCornerCamera = DefaultCameraLocation;
	
	bIsCrouching = false;
	//ZoonoutCooldown = true;
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(StartWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameter);

	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		
	}

	HealthComp->OnHealthChange.AddDynamic(this, &ACCharacter::OnHealthChange);
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, APostProcessVolume::StaticClass(), Actors);
	for (auto& PostActor :Actors)
	{
		if (PostActor->GetName() == "InvisibilityPPV")
		{
			InvisibilityPPV = Cast<APostProcessVolume>(PostActor);
		}
		if (PostActor->GetName() == "XRayPPV")
		{
			XRayPPV = Cast<APostProcessVolume>(PostActor);
		}
	}
	if (InvisibilityPPV)
	{
		Log(ELogLevel::INFO, "InvisPPV Cast Success!!!");
	}
	if (XRayPPV)
	{
		Log(ELogLevel::INFO, "XRayPPV Cast Success!!!");
	}

	if (XRayCurveFloatBegin)
	{
		FOnTimelineFloatStatic TimelineBegin;
		FOnTimelineEventStatic TimelineBeginTimer;
		TimelineBegin.BindUFunction(this, FName("XRayTimelineBegin"));
		TimelineBeginTimer.BindUFunction(this, FName("XRayTimelineBeginTimer"));
		XRayCurveTimelineBegin.AddInterpFloat(XRayCurveFloatBegin, TimelineBegin);
		XRayCurveTimelineBegin.SetTimelineFinishedFunc(TimelineBeginTimer);
		
		
	}
	else
	{
		Log(ELogLevel::WARNING, "XRayCurveFloatBegin is nullptr!!");
	}

	if (XRayCurveFloatEnd)
	{
		FOnTimelineFloatStatic TimelineEnd;
		FOnTimelineEventStatic TimelineEndStateChange;
		TimelineEnd.BindUFunction(this, FName("XRayTimelineEnd"));
		TimelineEndStateChange.BindUFunction(this, FName("XRayTimelineEndStateChange"));
		XRayCurveTimelineEnd.AddInterpFloat(XRayCurveFloatEnd,TimelineEnd);
		XRayCurveTimelineEnd.SetTimelineFinishedFunc(TimelineEndStateChange);
	} 
	else
	{
		Log(ELogLevel::WARNING, "XRayCurveFloatEnd is nullptr!!");
	}
	if (InvisibleFloatDuration)
	{
		FOnTimelineFloat InvisibleCurveDurationBegin;
		FOnTimelineEventStatic InvisibleCurveDurationEnd;
		InvisibleCurveDurationBegin.BindUFunction(this, FName("InvisibleBeginDuration"));
		InvisibleCurveDurationEnd.BindUFunction(this, FName("ToggleInvisibility"));
		InvisibleDuration->AddInterpFloat(InvisibleFloatDuration, InvisibleCurveDurationBegin);
		InvisibleDuration->SetTimelineFinishedFunc(InvisibleCurveDurationEnd);
	}
	else
	{
		Log(ELogLevel::WARNING, "InvisibleFloatDuration is nullptr!!");
	}

	if (InvisibleFloatCooldown)
	{
		FOnTimelineFloat InvisibleCurveCooldownBegin;
		FOnTimelineEventStatic InvisibleCurveCooldownEnd;
		InvisibleCurveCooldownBegin.BindUFunction(this, FName("InvisibleBeginCooldown"));
		InvisibleCurveCooldownEnd.BindUFunction(this, FName("SetCanBeInvisible"));
		InvisibleCooldown->AddInterpFloat(InvisibleFloatCooldown, InvisibleCurveCooldownBegin);
		InvisibleCooldown->SetTimelineFinishedFunc(InvisibleCurveCooldownEnd);
		
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if (XRayFloatDuration)
	{
		FOnTimelineFloat XRayCurveDurationBegin;
		FOnTimelineEventStatic XRayCurveDurationEnd;
		XRayCurveDurationBegin.BindUFunction(this, FName("XRayBeginDuration"));
		XRayCurveDurationEnd.BindUFunction(this, FName("ToggleXRay"));
		XRayDuration->AddInterpFloat(XRayFloatDuration, XRayCurveDurationBegin);
		XRayDuration->SetTimelineFinishedFunc(XRayCurveDurationEnd);
	}
	else
	{
		Log(ELogLevel::WARNING, "InvisibleFloatDuration is nullptr!!");
	}
	if (XRayFloatCooldown)
	{
		FOnTimelineFloat XRayCurveCooldownBegin;
		FOnTimelineEventStatic XRayCurveCooldownEnd;
		XRayCurveCooldownEnd.BindUFunction(this, FName("SetCanBeInXRay"));
		XRayCurveCooldownBegin.BindUFunction(this, FName("XRayBeginCooldown"));
		XRayCooldown->AddInterpFloat(XRayFloatCooldown, XRayCurveCooldownBegin);
		XRayCooldown->SetTimelineFinishedFunc(XRayCurveCooldownEnd);
	}



	Crosshair = CreateWidget<UUserWidget>(GetWorld(), CrosshairBPReference);


	
}



void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//float CurrentFOV = MarkingMode ? ZoomFOV : DefaultFOV;
	Velocity = GetVelocity().Size();
	if (MarkingMode || IsInCorner)
	{
		SpreadCrosshair(Velocity);
	}
	
	if (IsInCover)
	{
		FVector NewCoverCameraLocation;
		
		float TempCameraSpeed = CoverCameraSpeed;
		NewCoverCameraLocation = MidSocketOffset;
		if (IsClosedToCorner)
		{
			if (CoverDirection <0)
			{
				NewCoverCameraLocation = LeftCameraCoverMove;
			}
			else
			{
				NewCoverCameraLocation = RightCameraCoverMove;
			}
			
		}
		
		if (CoverDirection < 0 && MarkingMode)
		{
			NewCoverCameraLocation = LeftCameraCornerMove + CenterCameraLocation;
			TempCameraSpeed = ZoominCameraSpeed;
		}
		if (CoverDirection > 0 && MarkingMode)
		{
			NewCoverCameraLocation = RightCameraCornerMove + CenterCameraLocation;
			TempCameraSpeed = ZoominCameraSpeed;
		}
		
		FVector NewCoverLocation = FMath::VInterpTo(CameraBoom->SocketOffset, NewCoverCameraLocation, DeltaTime, TempCameraSpeed);
		//FollowCamera->SetRelativeLocation(NewCoverLocation);
		//CameraBoom->SocketOffset=
		CameraBoom->SocketOffset = NewCoverLocation;
		
	}
	//else
	//{
	//	FVector NewCL = FMath::VInterpTo(FollowCamera->GetRelativeTransform().GetLocation(), DefaultCameraLocation, DeltaTime, ZoomoutCameraSpeed);
	//	FollowCamera->SetRelativeLocation(NewCL);
	//}

	if (MarkingMode && !IsInCover)
	{
		

		float NewZoominLocation = FMath::FInterpTo(FollowCamera->FieldOfView, ZoomFOV, DeltaTime, ZoominCameraSpeed);
		CameraBoom->SocketOffset = DefaultSocketOffset;
		FollowCamera->SetFieldOfView(NewZoominLocation);
		//FollowCamera->SetRelativeLocation(NewZoominLocation);
		//CameraBoom->SetRelativeLocation(NewZoominLocation);

	}
	else if(!MarkingMode && !IsInCover)
	{
		ResetTimeline();
		float NewZoominLocation = FMath::FInterpTo(FollowCamera->FieldOfView, 90.0f, DeltaTime, ZoomoutCameraSpeed);
		//FVector NewCL = FMath::VInterpTo(CameraBoom->GetRelativeTransform().GetLocation(), DefaultCameraLocation, DeltaTime, ZoomoutCameraSpeed);
		FVector NewSocketOffLocation = FMath::VInterpTo(CameraBoom->SocketOffset,DefaultSocketOffset, DeltaTime, CoverOutCameraSpeed);
		//CameraBoom->SetRelativeLocation(NewCL);
		FollowCamera->SetFieldOfView(NewZoominLocation);
		CameraBoom->SocketOffset = NewSocketOffLocation;
	}


	



	if (bIsCrouching == true && (Velocity > 0.f) && !IsInCover)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CrouchWeaponSocketName);
		IsCrouchWalking = true;
	}
	else if(bIsCrouching == true && Velocity == 0.f && !IsInCover)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		IsCrouchWalking = false;
	}

	InvisibleDuration->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);

	InvisibleCooldown->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);

	XRayCooldown->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);

	XRayDuration->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);

	XRayCurveTimelineBegin.TickTimeline(DeltaTime);

	XRayCurveTimelineEnd.TickTimeline(DeltaTime);

}



void ACCharacter::SetCanTakeCover(bool CanTakeCover, class ACoverActor* CoverActor)
{
	if (!CanTakeCover && IsInCover )
	{
		ToggleCover();
	}
	bCanTakeCover = CanTakeCover;
	Cover = CoverActor;
	
}



void ACCharacter::SetIsStandCover(bool HighCube)
{
	IsStandCover = HighCube;
}



void ACCharacter::SetMagicChange(float MagicChange)
{
	Invis.PreviousMagic = Invis.MagicPercentage;
	Invis.MagicValue = (MagicChange / Invis.FullMagic);
	InvisibleDuration->PlayFromStart();
}

void ACCharacter::UpdateMagic()
{
	Invis.PreviousMagic = Invis.MagicPercentage;
	Invis.MagicPercentage = Invis.Magic / Invis.FullMagic;
	Invis.MagicValue = 1.0f;
	InvisibleCooldown->PlayFromStart();
}


void ACCharacter::SetXRayChange(float XRayChange)
{
	Xray.PreviousMagic = Xray.MagicPercentage;
	Xray.MagicValue = (XRayChange / Xray.FullMagic);
	XRayDuration->PlayFromStart();
}

void ACCharacter::UpdateXRay()
{
	Xray.PreviousMagic = Xray.MagicPercentage;
	Xray.MagicPercentage = Xray.Magic / Xray.FullMagic;
	Xray.MagicValue = 1.0f;
	XRayCooldown->PlayFromStart();
}

void ACCharacter::InvisibleBeginDuration()
{
	TimeLineValue = InvisibleDuration->GetPlaybackPosition();
	CurveFloatValue = Invis.PreviousMagic + Invis.MagicValue * InvisibleFloatDuration->GetFloatValue(TimeLineValue);
	Invis.Magic = FMath::Clamp(CurveFloatValue*Invis.FullMagic, 0.0f, Invis.FullMagic);
	Invis.MagicPercentage = FMath::Clamp(CurveFloatValue, 0.0f, 1.0f);
	
}

void ACCharacter::InvisibleBeginCooldown()
{
	TimeLineValue = InvisibleCooldown->GetPlaybackPosition();
	CurveFloatValue = Invis.PreviousMagic + Invis.MagicValue * InvisibleFloatCooldown->GetFloatValue(TimeLineValue);
	Invis.Magic = FMath::Clamp(CurveFloatValue*Invis.FullMagic, 0.0f, Invis.FullMagic);
	Invis.MagicPercentage = FMath::Clamp(CurveFloatValue, 0.0f, 1.0f);
	if (Invis.MagicPercentage == 1.0f)
	{
		InvisibleCooldown->Stop();
		SetCanBeInvisible();
	}

}

void ACCharacter::XRayBeginDuration()
{
	TimeLineValue2 = XRayDuration->GetPlaybackPosition();
	CurveFloatValue2 = Xray.PreviousMagic + Xray.MagicValue * XRayFloatDuration->GetFloatValue(TimeLineValue2);
	Xray.Magic = FMath::Clamp(CurveFloatValue2*Xray.FullMagic, 0.0f, Xray.FullMagic);
	Xray.MagicPercentage = FMath::Clamp(CurveFloatValue2, 0.0f, 1.0f);

}

void ACCharacter::XRayBeginCooldown()
{
	TimeLineValue2 = XRayCooldown->GetPlaybackPosition();
	CurveFloatValue2 = Xray.PreviousMagic + Xray.MagicValue * XRayFloatCooldown->GetFloatValue(TimeLineValue2);
	Xray.Magic = FMath::Clamp(CurveFloatValue2*Xray.FullMagic, 0.0f, Xray.FullMagic);
	Xray.MagicPercentage = FMath::Clamp(CurveFloatValue2, 0.0f, 1.0f);
	if (Xray.MagicPercentage == 1.0f)
	{
		XRayCooldown->Stop();
		SetCanBeInXRay();
	}
}

void ACCharacter::XRayTimelineBeginTimer()
{

}

void ACCharacter::StartFireOrStartAttack()
{

	if ((CurrentWeapon && MarkingMode && !IsInCover) || IsInCorner)
	{
		Log(ELogLevel::INFO, "StartFire");
		if (IsInvisible)
		{
			ToggleInvisibility();
		}
		if (IsInXRay)
		{
			ToggleXRay();
		}
		CameraCenter = FollowCamera->GetComponentRotation().Vector();
		CurrentWeapon->StartFire();

		

	}
	if (!MarkingMode)
	{
		Log(ELogLevel::INFO, "StartAttack");
	}
}



void ACCharacter::StopFireOrAttackEnd()
{
	
	
		CurrentWeapon->StopFire();
	
}




void ACCharacter::ToggleCover()
{
	//FName LastWeaponSocketName;
	if (bCanTakeCover)
	{
		IsInCover = !IsInCover;
		if (IsClosedToCorner)
		{
			IsClosedToCorner = false;
		}
		
		if (IsInCover && Cover)
		{	
			SetClosedToCorner();
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			FRotator CoverRotation;
			Cover->PublicDetermineMovementDirection(CoverDirectionMovement, CoverRotation);
			SetActorRotation(CoverRotation);
			
			if (!bIsCrouching && !IsStandCover)
			{
				Crouch();
				bIsCrouching = true;
			}
			
			GetCharacterMovement()->MaxWalkSpeed = StandCoverSpeed;
		
			if (CurrentWeapon )
			{
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CoverWeaponSocketName);
			}
		}
		else
		{
		
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
			GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
		}

	}

}




void ACCharacter::SetClosedToCorner()
{
	float Direction;
	float Distance;
	Log(ELogLevel::INFO, "getin");
	if (Cover->IsCloseToCorner(Direction, Distance))
	{
		if ((Direction >= 0.0f) == (Distance <= 0.0f))
		{
			Log(ELogLevel::INFO, "getin true");
			IsClosedToCorner = true;
		}
		
		
	}
	else
	{
	    IsClosedToCorner = false;
	}
	
}

void ACCharacter::ToggleInvisibility()
{
	IsInvisible = !IsInvisible;
	USkeletalMeshComponent* WeaponMesh = CurrentWeapon->GetWeaponMeshComp();
	if (IsInvisible)
	{
		if (bCanInvisible)
		{
			if (InvisibleDuration!= nullptr)
			{
				InvisibleDuration->Stop();
			}
			else
			{
				Log(ELogLevel::INFO, "InvisibleDuration is nullptr!!");
			}
			SetMagicChange(-100.0f);
			//InvisibleDuration->PlayFromStart();
		    InvisibilityPPV->bEnabled = true;
			bCanInvisible = false;
			UGameplayStatics::PlaySound2D(this, BeginInvisibleSound);
			//SpawnTimer(InvisibilityTimer,Invisibility);
			GetMesh()->SetRenderInMainPass(false);
			GetMesh()->SetRenderCustomDepth(true);
			GetMesh()->CustomDepthStencilValue = 3;
			
			WeaponMesh->SetVisibility(false);
		}
		else
		{
			IsInvisible = !IsInvisible;
			Log(ELogLevel::INFO, "Invisibility is not ready!");
			return;
			//TODO: HUD显示技能CD，提示技能未冷却
		}
		

	/*	if (InvisibilityEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), InvisibilityEffect, GetActorLocation());
		}*/

	}
	else {

		InvisibilityPPV->bEnabled = false;
		//if (InvisibilityTimer)
		//{
		//	InvisibilityTimer->ClearTimer();
		//	InvisibilityTimer->CooldownTimerBegin();
		//}
		InvisibleDuration->Stop();
		UpdateMagic();
		UGameplayStatics::PlaySound2D(this, EndInvisibleSound);
		GetMesh()->SetRenderInMainPass(true);
		GetMesh()->SetRenderCustomDepth(false);
		WeaponMesh->SetVisibility(true);
	
	}
	
	
}

void ACCharacter::ToggleReload()
{
	if (CurrentWeapon->Clip != CurrentWeapon->ClipSetting)
	{
		SpawnClip();
		UGameplayStatics::PlaySoundAtLocation(this, PickupAmmoSounds, GetMesh()->GetSocketLocation(WeaponSocketName));
		CurrentWeapon->WeaponReload();
		UMyAnimInstance* AnimInstanceC = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstanceC)
		{
			AnimInstanceC->Reload();
		}

	}

}

void ACCharacter::ToggleXRay()
{
	IsInXRay = !IsInXRay;
	if (IsInXRay)
	{
		if (bCanXRay)
		{
			if (XRayDuration != nullptr)
			{
				XRayDuration->Stop();
			}
			SetXRayChange(-100.0f);
			bCanXRay = false;
			UGameplayStatics::PlaySound2D(this, BeginXRaySound);
			//SpawnTimer(XRayTimer, XRay);
			XRayCurveTimelineBegin.PlayFromStart();

		}
		else
		{
			IsInXRay = !IsInXRay;
			Log(ELogLevel::INFO, "XRay is not ready!");
			return;
		}
		
	}
	else
	{
		
		/*	if (XRayTimer)
			{
				XRayTimer->ClearTimer();
				XRayTimer->CooldownTimerBegin();
			}*/
		XRayDuration->Stop();
		UpdateXRay();
		UGameplayStatics::PlaySound2D(this, EndXRaySound);
		XRayCurveTimelineEnd.PlayFromStart();
	}


}

void ACCharacter::SetCanBeInvisible()
{
	if (bCanInvisible == false)
	{
		bCanInvisible = !bCanInvisible;
	}
	
}

void ACCharacter::SetCanBeInXRay()
{
	if (bCanXRay == false)
	{
		 bCanXRay = !bCanXRay; 
	}

}

float ACCharacter::GetMagic()
{
	return Invis.MagicPercentage;
}

float ACCharacter::GetXRay()
{
	return Xray.MagicPercentage;
}

void ACCharacter::XRayTimelineBegin(float Value)
{
	XRayPPV->bEnabled = true;
	//float NewFloat = FMath::Lerp(1.0, 0.0, Value);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), XRayParameter, "Position", GetActorLocation());
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), XRayParameter, "Radius",Value);
}

void ACCharacter::XRayTimelineEnd(float Value)
{
	
	//float NewFloat = FMath::Lerp(0.0, 1.0, Value);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), XRayParameter, "Position", GetActorLocation());
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), XRayParameter, "Radius", Value);

}

void ACCharacter::XRayTimelineEndStateChange()
{
	
	XRayPPV->bEnabled = false;
}





void ACCharacter::SpawnTimer(ASkillTimer* &NewTimer, TSubclassOf <ASkillTimer> SkillClass)
{
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	NewTimer = GetWorld()->SpawnActor<ASkillTimer>(SkillClass, GetTransform(), SpawnParameter);
	if (NewTimer )
	{
	    NewTimer->SetOwner(this);
	}
	else
	{
		Log(ELogLevel::WARNING, "Skill Timer Spawn Failed!!!");
	}

	
}

void ACCharacter::AddControllerYawInput(float Val)
{
	//重写方程来获得鼠标Yaw输入变量
	MouseAxisTurn = Val;
	if (Val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddYawInput(Val);
	}
	//if (MouseAxisTurn > MaxTurn)
	//{
	//	MaxTurn = MouseAxisTurn;
	//}
	
}

void ACCharacter::AddControllerPitchInput(float Val)
{
	//重写方程来获得鼠标Pitch输入变量
	MouseLookUpAxis = Val;
	if (Val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(Val);
	}


}


FVector ACCharacter::GetPawnViewLocation() const
{
	if (FollowCamera)
	{
		return FollowCamera->GetComponentLocation();
	}
	
		return Super::GetPawnViewLocation();
	

	
}


void ACCharacter::BeginCrouch()
{
	if (bIsCrouching)
	{
		if (IsInCover && !IsStandCover)
		{
			ToggleCover();
			
		}
	
		UnCrouch();
		bIsCrouching = false;
		IsCrouchWalking = false;
		if (CurrentWeapon->GetAttachParentSocketName() != WeaponSocketName && !IsInCover)
		{
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		}
	} 
	else
	{
	
		
		Crouch();
		bIsCrouching = true;
	}
	
}



void ACCharacter::Marking()
{
	if (!IsCrouchWalking || IsInCover)
	{
		//Crosshair->AddToViewport();
		//FVector NewCL = FMath::VInterpTo(FollowCamera->GetRelativeTransform().GetLocation(), NewCameraLocation, MyDeltaTime, ZoominCameraSpeed);
	   // FollowCamera->SetRelativeLocation(NewZoominLocation);
		
		
		if (IsInCover)
		{
			float Distance;
			float Direction;
				if (Cover->IsCloseToCorner(Distance,Direction) && (Direction >= 0.0f) == (CoverDirection <= 0.0f))
				{
					//GetCharacterMovement()->Activate(true);
					//SetActorLocation();
				
					FVector AddForce;
					if (Direction < 0 )
					{
						AddForce = (CoverDirectionMovement * 300) + FVector(0, 0, 1) * 80;
					}
					else if(Direction > 0)
					{
						AddForce = (CoverDirectionMovement * -400) + FVector(0, 0, 1) * 80;
					}
					
					LaunchCharacter(AddForce , true, true);
					//AddMovementInput(GetActorLocation() + CoverDirectionMovement * 100);
					IsInCorner = true;
					Log(ELogLevel::INFO, "InCorner");
				}
				else
				{
					Log(ELogLevel::WARNING, "Not Closed to Corner");
				}
			
			
		}
		
		if (IsInCorner || !IsInCover)
		{
			SpawnCrosshair();
		}
		//改变瞄准状态下CameraLag，移动速度
		CameraBoom->CameraLagSpeed = 50;
		CameraBoom->CameraRotationLagSpeed = 50;
		
			bUseControllerRotationYaw = true;
		if (IsInCover && !IsInCorner)
		{
			bUseControllerRotationYaw = false;
		}
		
		
		GetCharacterMovement()->MaxWalkSpeed = AimingSpeed;
		MarkingMode = true;

	}
	
		
   
}

void ACCharacter::QuitMarking()
{
	
	CameraBoom->CameraRotationLagSpeed = 30;
	CameraBoom->CameraLagSpeed = 10;
	//Crosshair->RemoveFromViewport();
	//FVector NewCL = FMath::VInterpTo(FollowCamera->GetRelativeTransform().GetLocation(), DefaultCameraLocation, MyDeltaTime, ZoomoutCameraSpeed);
	//FollowCamera->SetRelativeLocation(NewCL);
	if (!IsInCover)
	{
	
		bUseControllerRotationYaw = false;
	}
	
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	MarkingMode = false;
	RemoveCrosshair();
	if (IsInCorner)
	{
		FVector AddForce;
		if (CoverDirection > 0)
		{
			AddForce = (CoverDirectionMovement * -300) + FVector(0, 0, 1) * 80;
		}
		else
		{
			AddForce = (CoverDirectionMovement * 400) + FVector(0, 0, 1) * 80;
		}
		LaunchCharacter(AddForce, true, true);
		IsInCorner = false;
		bUseControllerRotationYaw = false;
	}

	StopFireOrAttackEnd();
	

}

//生命变化方程
void ACCharacter::OnHealthChange(UHealthComponent * HealthC, float Health, float HealthDelta, 
	const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (!IsDead && Health<=0.0f)
	{
		IsDead = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
	}
}


void ACCharacter::FootstepSurface()
{
	FVector StartTrace = FVector::ZeroVector;
	FVector CamLoc;
	FVector target;
	FCollisionQueryParams TraceParams;
	FHitResult currentHit;


	CamLoc = FollowCamera->GetComponentTransform().GetLocation();

	target = CamLoc + FVector(0.0f, 0.0f, -300.0f);
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(currentHit, CamLoc, target, ECollisionChannel::ECC_Visibility, TraceParams);

	//UPhysicalMaterial* physMat = NULL;


	

		EPhysicalSurface FloorSurfaceType = UPhysicalMaterial::DetermineSurfaceType(currentHit.PhysMaterial.Get());

	
	//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPhysicalSurface"), true);
	//	if (EnumPtr)
	//	{
	//		CurrentSurface = EnumPtr->GetEnumText(FloorSurfaceType);
	//	}

	
	
}

void ACCharacter::MoveForward(float Value)
{

	//FootstepSurface();

	IsCrouchZooming = bIsCrouching && MarkingMode;
	
	if (IsInCorner)
	{
		Value = 0.f;
	}
		if ((!IsCrouchZooming) && (Controller != NULL) && (Value != 0.0f) && !IsInCover)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
			
			if (!bIsCrouching)
			{
				MakeNoise(1.0f, Instigator);
			}
	
		}
	
	
}

void ACCharacter::MoveRight(float Value)
{

	//FootstepSurface();
	IsCrouchZooming = bIsCrouching && MarkingMode && !IsInCover;

	if (CoverValue < 0)
	{
		CoverDirection = -1;
		
	}
	else if(CoverValue > 0)
	{
		CoverDirection = 1;

	}
	if (IsInCorner)
	{
		Value = 0.f;
	}
	else
	{
		CoverValue = Value;
	}
	//bool IsCoverZooming = IsInCover && MarkingMode;
	if ((!IsCrouchZooming) && ((Controller != NULL) && (Value != 0.0f)) )
	{
		if (!IsInCover)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);

			if (!bIsCrouching)
			{
				MakeNoise(1.0f, Instigator);
			}
		
		} 
		else
		{
			AddMovementInput(CoverDirectionMovement, Value);
			SetClosedToCorner();
		}
		
	
		
	}
}



void ACCharacter::Log(ELogLevel LogLevel, FString Message)
{
	if (OpenLog)
	{
		Log(LogLevel, Message, ELogOutput::ALL);
	}

}

void ACCharacter::Log(ELogLevel LogLevel, FString Message, ELogOutput LogOutput)
{
	// only print when screen is selected and the GEngine object is available
	if ((LogOutput == ELogOutput::ALL || LogOutput == ELogOutput::SCREEN) && GEngine)
	{
		// default color
		FColor LogColor = FColor::Cyan;
		// flip the color based on the type
		switch (LogLevel)
		{
		case ELogLevel::TRACE:
			LogColor = FColor::Green;
			break;
		case ELogLevel::DEBUG:
			LogColor = FColor::Cyan;
			break;
		case ELogLevel::INFO:
			LogColor = FColor::White;
			break;
		case ELogLevel::WARNING:
			LogColor = FColor::Yellow;
			break;
		case ELogLevel::ERROR:
			LogColor = FColor::Red;
			break;
		default:
			break;
		}
		// print the message and leave it on screen ( 4.5f controls the duration )
		GEngine->AddOnScreenDebugMessage(-1, 4.5f, LogColor, Message);
	}

	if (LogOutput == ELogOutput::ALL || LogOutput == ELogOutput::OUTPUT_LOG)
	{
		// flip the message type based on error level
		switch (LogLevel)
		{
		case ELogLevel::TRACE:
			UE_LOG(LogTemp, VeryVerbose, TEXT("%s"), *Message);
			break;
		case ELogLevel::DEBUG:
			UE_LOG(LogTemp, Verbose, TEXT("%s"), *Message);
			break;
		case ELogLevel::INFO:
			UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
			break;
		case ELogLevel::WARNING:
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
			break;
		case ELogLevel::ERROR:
			UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
			break;
		default:
			UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
			break;
		}
	}
}
