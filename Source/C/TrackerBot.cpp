// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "HealthComponent.h"
// Sets default values
ATrackerBot::ATrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetCanEverAffectNavigation(false);
	bPatorl = true;
	FloatingComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingComp"));
	FloatingComp->MaxSpeed = 300;
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	//HealthComp->OnHealthChange.AddDynamic(this, &ATrackerBot::WhenTakeDamage);
	HealthComp->OnHealthChange.AddDynamic(this, &ATrackerBot::WhenTakeDamage);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ATrackerBot::OnSeenPawn);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &ATrackerBot::OnHearNoise);

	
}

// Called when the game starts or when spawned
void ATrackerBot::BeginPlay()
{
	Super::BeginPlay();
	//UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), PatrolPoint[0]);
	
}

void ATrackerBot::OnSeenPawn(APawn* SeenPawn)
{
	float Distance = SeenPawn->GetDistanceTo(this);
	if (SeenPawn == nullptr)
	{
		return;
	}
	if (Distance > 500 && Distance <= 2000)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), SeenPawn->GetActorLocation());
	}

}

void ATrackerBot::OnHearNoise(APawn* HearPawn, const FVector& Location, float Volume)
{

}

FVector ATrackerBot::GetNextPathPoint()
{
	return FVector();
}

void ATrackerBot::WhenTakeDamage(UHealthComponent * HealthC, float Health, float HealthDelta,
	const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("Health: %s %s"), *FString::SanitizeFloat(Health),*GetName());
}

// Called every frame
void ATrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


