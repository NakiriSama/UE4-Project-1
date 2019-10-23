// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShake.h"
#include "PhysicalMaterials//PhysicalMaterial.h"
#include "C.h"
#include "TimerManager.h"
#include "CCharacter.h"

// static int32 DebugWeaponDrawing = 0;
//FAutoConsoleVariableRef("Debug.Weapons", DebugWeaponDrawing, TEXT("Draw Debug Lines for Fire()"), ECVF_Cheat);
	
	
	
// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComp"));
	RootComponent = WeaponMeshComp;

	MuzzleSocketName = "MuzzleSocket";
	FireSocketName =  "FireBegin";
	TargetName = "Target";
	BaseDamage = 20.0f;
	FireRate = 600.0f;
	ClipSetting =6;
	
	
}
void AMyWeapon::BeginPlay()
{
	Clip = ClipSetting;
	Super::BeginPlay();
	TimeBetweenFire = 60 / FireRate;
}

void AMyWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenFire - (GetWorld()->TimeSeconds), 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandleForFire, this, &AMyWeapon::Fire, TimeBetweenFire ,true, FirstDelay);
	
}

void AMyWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandleForFire);
}

void AMyWeapon::ChangeClip(uint8 BulletNumber, bool IsReload)
{
	if (IsReload)
	{
		Clip +=BulletNumber;
	}
	else
	{
		Clip -= BulletNumber;
	}
	
}

void AMyWeapon::StockReload()
{
	ClipStock = MaxClipStock;
	OnReload();
	AActor* MyOwner = GetOwner();
	ACCharacter* Player = Cast<ACCharacter>(MyOwner);
	Player->SpawnClip();
}

void AMyWeapon::WeaponReload()
{
	if (ClipStock > (ClipSetting - Clip))
	{
		ClipStock -= (ClipSetting - Clip);
		ChangeClip(ClipSetting - Clip,true);
		
	}
	else
	{
		ChangeClip(ClipStock, true);
		ClipStock = 0;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Stock: %u"), ClipStock);
	OnReload();
}

// Called when the game starts or when spawned


void AMyWeapon::Fire()
{

	AActor* MyOwner = GetOwner();
	if (Clip >= 1)
	{
		if (MyOwner)
		{
			ChangeClip(1,false);
			OnFireClipChange(); 
			FVector FireLocation;
			FVector CrosshairLocation;
			FRotator EyeRotation;
			MyOwner->GetActorEyesViewPoint(FireLocation, EyeRotation);

			ACCharacter* MyCharacter = Cast<ACCharacter>(MyOwner);
			CrosshairLocation = MyCharacter->GetPawnViewLocation();
			FVector TraceEnd;
			//FVector Center = MyCharacter->FollowCamera->GetComponentRotation().Vector();
			ScreenCenter = MyCharacter->GetScreenCenter();
			TraceEnd = CrosshairLocation + (EyeRotation.Vector() * 10000);


			//TraceEnd = (WeaponMeshComp->GetSocketLocation(MuzzleSocketName) - WeaponMeshComp->GetSocketLocation(FireSocketName)) * 10000;

			FVector WeaponMeshLocation;
			WeaponMeshLocation = WeaponMeshComp->GetSocketLocation(FireSocketName);
			FireLocation = WeaponMeshComp->GetSocketLocation(MuzzleSocketName);
			FHitResult Hit;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(MyOwner);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;
			FVector TraceEndPoint = TraceEnd;
			QueryParams.bReturnPhysicalMaterial = true;

			FHitResult TrueHit;
			FCollisionQueryParams QueryParams2;
			QueryParams2.AddIgnoredActor(MyOwner);
			QueryParams2.AddIgnoredActor(this);
			QueryParams2.bTraceComplex = true;
			

			FHitResult TestHit;
			FCollisionQueryParams QueryParams3;
			QueryParams3.bTraceComplex=true;
		
			
			if (GetWorld()->LineTraceSingleByChannel(Hit, CrosshairLocation, TraceEnd, TRACECHANNEL_WEAPON, QueryParams))
			{
				FHitResult FinalHit;
				QueryParams3.AddIgnoredActor(Hit.GetActor());
				
				GetWorld()->LineTraceSingleByChannel(TestHit, Hit.ImpactPoint, WeaponMeshLocation, TRACECHANNEL_WEAPON, QueryParams3);
				GetWorld()->LineTraceSingleByChannel(TrueHit, FireLocation, Hit.ImpactPoint, TRACECHANNEL_WEAPON, QueryParams2);
				AActor* BackTraceHit = TestHit.GetActor();
				FinalHit = TrueHit;
					if (BackTraceHit && BackTraceHit->IsA<ACCharacter>())
					{
						FinalHit = Hit;
					}
										
					

				AActor* HitActor = FinalHit.GetActor();
				float TrueDamage = BaseDamage;
				TraceEndPoint = FinalHit.ImpactPoint;

				EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(FinalHit.PhysMaterial.Get());
				if (SurfaceType == SURFACE_FLESHHEAD)
				{
					TrueDamage *= 4.0;
				}
				if (SurfaceType == SURFACE_AIEYE)
				{
					TrueDamage *= 2;
				}
				if (SurfaceType == SURFACE_AIBODY)
				{
					TrueDamage = 1.f;
				}
				UGameplayStatics::ApplyPointDamage(HitActor, TrueDamage, EyeRotation.Vector(), FinalHit, MyOwner->GetInstigatorController(), this, DamageType);

				UParticleSystem* SelectEffect = nullptr;

				switch (SurfaceType)
				{
				case SURFACE_FLESHBODY:
				case SURFACE_FLESHHEAD:
				case SURFACE_AIEYE:
					SelectEffect = FleshImpactEffect;
					break;
				default:
					SelectEffect = DefaultImpactEffect;
					break;
				}
				if (SelectEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectEffect, FinalHit.ImpactPoint, FinalHit.ImpactNormal.Rotation());
				}

			}




			if (DebugWeaponDrawing)
			{
				DrawDebugLine(GetWorld(), FireLocation, TraceEnd, FColor::Red, false, 2.0f, 0, 2.0f);
			}
			if (AKSingleSounds[0])
			{
				int x = rand() % (AKSingleSounds.Num());
				UGameplayStatics::PlaySoundAtLocation(this, AKSingleSounds[x], WeaponMeshComp->GetSocketLocation(MuzzleSocketName));
			}

			PlayEffects(TraceEndPoint);
			LastFireTime = GetWorld()->TimeSeconds;
		}
	}
    else 
	{
     	if (ClipStock >=  1)
	    {
			ACCharacter* MyCharacter = Cast<ACCharacter>(MyOwner);
			MyCharacter->ToggleReload();
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, EmptyGunSounds, WeaponMeshComp->GetSocketLocation(MuzzleSocketName));
		}
		
	}
	


}


void AMyWeapon::PlayEffects(FVector TraceEnd)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMeshComp, MuzzleSocketName);
	}
	if (TraceEffect)
	{
		UParticleSystemComponent* TraceComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, WeaponMeshComp->GetSocketLocation(MuzzleSocketName));
		if (TraceComp)
		{
			TraceComp->SetVectorParameter(TargetName, TraceEnd);
		}
		
	}
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* MyController = Cast<APlayerController>(MyOwner->GetController());
		if (MyController)
		{
			MyController->ClientPlayCameraShake(FireShake);
		}
	}
}

