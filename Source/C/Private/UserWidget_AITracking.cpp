// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_AITracking.h"
#include "C.h"
#include "CCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyWeapon.h"
#include "AIGuard.h"


void UUserWidget_AITracking::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}

UUserWidget_AITracking::UUserWidget_AITracking(const FObjectInitializer& ObjectInitializer)
  :Super(ObjectInitializer)
{

}

void UUserWidget_AITracking::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UUserWidget_AITracking::TrackTracingline(FVector TraceStart, FVector TraceEnd, const bool IsInXray)
{
	ACCharacter* MyPlayer = Cast<ACCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	FHitResult Hit;
	AActor* HitActor;
	FCollisionQueryParams QueryParams;
	if (MyPlayer)
	{
		QueryParams.AddIgnoredActor(MyPlayer);
		QueryParams.AddIgnoredActor(MyPlayer->GetWeaponComp());
	}
	
	QueryParams.bTraceComplex = true;
	FVector TraceEndPoint = TraceEnd;
	QueryParams.bReturnPhysicalMaterial = true;
	if (IsInXray)
	{
		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TRACECHANNEL_AITRACE, QueryParams)) {
			UE_LOG(LogTemp, Log, TEXT("In Xray"));
			HitActor = Hit.GetActor();
		}
	}
	else
	{
		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TRACECHANNEL_WEAPON, QueryParams)) {
			UE_LOG(LogTemp, Log, TEXT("In Normal"));
			HitActor = Hit.GetActor();
		}
	}
	if (Hit.GetActor()->IsA<AAIGuard>())
	{
		AAIGuard* HitAI = Cast<AAIGuard>(Hit.GetActor());
		if (HitAI->IsDead)
		{
			return false;
		}
		else 
		{
			return true;
		}
	}


	return false;
	
}

