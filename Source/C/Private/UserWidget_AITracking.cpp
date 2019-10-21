// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_AITracking.h"
#include "C.h"
#include "CCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyWeapon.h"
#include "AIGuard.h"
#include "MyPlayerController.h"



void UUserWidget_AITracking::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
	int32 MaxX;
	int32 MaxY;
	//SetPositionInViewport();
	AIPosition = HitAI->GetMesh()->GetSocketLocation(SocketName);
	GetOwningPlayer()->ProjectWorldLocationToScreen(AIPosition, ScreenPosition);
	GetOwningPlayer()->GetViewportSize(MaxX, MaxY);
	ScreenPosition.X = FMath::Clamp(ScreenPosition.X, 27.5f, (float)(MaxX-27.5));
	ScreenPosition.Y = FMath::Clamp(ScreenPosition.Y, 27.5f, (float)(MaxY-27.5));
	//UE_LOG(LogTemp, Log, TEXT("%f,%f"), ScreenPosition.X, ScreenPosition.Y);
	SetPositionInViewport(ScreenPosition);
	if (HitAI->IsDead)
	{
		RemoveFromParent();
	}
}

UUserWidget_AITracking::UUserWidget_AITracking(const FObjectInitializer& ObjectInitializer)
  :Super(ObjectInitializer)
{

}

void UUserWidget_AITracking::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UUserWidget_AITracking::TrackTracingline(FVector TraceStart, FVector TraceEnd, const bool IsInXray, bool &Marked, AAIGuard* &MarkedAI)
{

	MyPlayer = Cast<ACCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	MyPlayerController = Cast<AMyPlayerController>(MyPlayer->GetController());
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
	if (Hit.GetActor())
	{
		if (Hit.GetActor()->IsA<AAIGuard>())
		{

			HitAI = Cast<AAIGuard>(Hit.GetActor());

			if (HitAI->IsDead)
			{

				return false;
			}
			else
			{
				Marked = HitAI->IsMarked;
				MarkedAI = HitAI;

				return true;
			}
		}
	
	}


	return false;
	
}

void UUserWidget_AITracking::CancelMarked()
{
	
	HitAI->IsMarked = !HitAI->IsMarked;
}

