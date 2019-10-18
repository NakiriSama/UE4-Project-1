// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPod.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "CCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "MyWeapon.h"


// Sets default values
ALaunchPod::ALaunchPod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetupAttachment(RootComponent);
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPod::PodOverlap);
	LaunchPitchAngle = 60.0f;
	LaunchStrength = 1500.0f;
	
}

// Called when the game starts or when spawned

void ALaunchPod::PodOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FRotator LaunchAngle = GetActorRotation();
	LaunchAngle.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchAngle.Vector() * LaunchStrength;

	ACCharacter* MyPawn = Cast<ACCharacter>(OtherActor);
	if (MyPawn)
	{
		/*MyPawn->LaunchCharacter(LaunchVelocity, true, true);
		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation());
		UGameplayStatics::PlaySound2D(this, LaunchSound);
		MakeNoise(1.0f, MyPawn);*/
		AMyWeapon* MyWeapon = MyPawn->GetWeaponComp();
		if (MyWeapon)
		{
			if (MyWeapon->ClipStock!= MyWeapon->MaxClipStock)
			{
				//MyWeapon->OnFireClipChange(MyWeapon->ClipSetting);
				MyWeapon->StockReload();
				
				UGameplayStatics::PlaySoundAtLocation(this, PickupAmmoSounds, GetActorLocation());
			}
			
		}
		
	}
}
