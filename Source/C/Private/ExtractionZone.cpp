// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "CCharacter.h"
#include "CGameMode.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AExtractionZone::AExtractionZone()
{
 
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	RootComponent = OverlapComp;
	OverlapComp->SetBoxExtent(FVector(200.0f));
	
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	OverlapComp->SetHiddenInGame(true);
	
	

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AExtractionZone::HandleOverlap);

	DecalComp=CreateDefaultSubobject<UDecalComponent>(TEXT("DecalcComp"));
	DecalComp->DecalSize = FVector(200.0f);
	DecalComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned


void AExtractionZone::HandleOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone!"));
	ACCharacter* MyPawn = Cast<ACCharacter>(OtherActor);
	if (MyPawn == nullptr)
	{
		return;
	}
	if(MyPawn->bIsCarryingObjective == true)
	{
		ACGameMode* GameMode = Cast<ACGameMode>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			GameMode->CompleteMission(MyPawn, true);
			
			UGameplayStatics::PlaySound2D(this, MissionCompleteSound);

		}
		
		UE_LOG(LogTemp, Log, TEXT("Mission Complete!"));
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}
}

// Called every frame

