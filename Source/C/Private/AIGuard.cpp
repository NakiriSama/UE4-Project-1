// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"
#include "C.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "CGameMode.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CCharacter.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "MyAIController.h"
#include "CCharacter.h"
#include "AI_TargetPoint.h"
#include "Engine/TargetPoint.h"
#include "Classes/Materials/MaterialInterface.h"
#include "Classes/Components/CapsuleComponent.h"
#include "UserWidget_AITracking.h"


// Sets default values
AAIGuard::AAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsDead = false;
	IsMarked = false;

	
	DeathCapsuleComp=CreateDefaultSubobject<UCapsuleComponent>(TEXT("DeathCapsuleComp"));
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIGuard::OnSeenPawn);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AAIGuard::OnHearNoise);
	PawnSensingComp->SetPeripheralVisionAngle(45.f);
	
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	GuardState = EAIStateTest::Idle;
	
	
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	DefaultCapsuleComp = Cast<UCapsuleComponent>(GetRootComponent());
	//DeathCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FStateStruct ForState;
	ForState.StateMaterialsls = AlermedMaterial;
	ForState.StateSounds = AlermedSound;
	StateMap.Emplace(EAIStateTest::Alermed, ForState);
	ForState.StateMaterialsls = SubspiciousMaterial;
	ForState.StateSounds = SubspiciousSound;
	StateMap.Emplace(EAIStateTest::Subspicious, ForState);
	ForState.StateMaterialsls = SubspiciousMaterial;
	ForState.StateSounds = SubspiciousSound;
	StateMap.Emplace(EAIStateTest::SearchingPlayer, ForState);
	ForState.StateMaterialsls = IdleMaterial;
	ForState.StateSounds = IdleSound;
	StateMap.Emplace(EAIStateTest::Idle, ForState);
	//ForState.StateMaterialsls = DeathMaterial;
	//ForState.StateSounds = SubspiciousSound;
	//StateMap.Emplace(EAIStateTest::Dead, ForState);
	//StateMap.Emplace(EAIStateTest::Idle, IdleMaterial);
	//StateMap.Emplace(EAIStateTest::SearchingPlayer, SubspiciousMaterial);
	//StateMap.Emplace(EAIStateTest::Subspicious, SubspiciousMaterial);
	//StateMap.Emplace(EAIStateTest::Dead,DeathMaterial);
	MeshComp = GetMesh();
	MeshComp->SetMaterial(1, IdleMaterial);
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();
	HealthComp->OnHealthChange.AddDynamic(this, &AAIGuard::OnHealthChange);
	AIController = Cast<AMyAIController>(GetController());
	//UNavigationSystem::SimpleMoveToActor(GetController(), TestPatrolPoint);
	
	//if (bPatorl)
	//{
	//	CurrentPatrolPointNum = 0;
	//	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), PatrolPoint[CurrentPatrolPointNum]);
	//
	//}

	
}

void AAIGuard::OnSeenPawn(APawn * SeenPawn)
{
  if (!IsDead)
  {
	  
	  if (SeenPawn == nullptr)
	  {
		  return;
	  }

	  //UE_LOG(LogTemp, Log, TEXT("Find you!"));

	 // ACGameMode* GameMode = Cast<ACGameMode>(GetWorld()->GetAuthGameMode());

	  ACCharacter* Player = Cast<ACCharacter>(SeenPawn);
	  if (!Player->GetIsInvisible() && Player)
	  {
		  if (AIController)
		  {
			  AIController->MyAIState = EAIState::SeePlayer;
		  }
		  if (GuardState != EAIStateTest::Alermed && GuardState != EAIStateTest::SearchingPlayer)
		  {
			  SetGuardState(EAIStateTest::Alermed);
		  }
		
		  float Distance = SeenPawn->GetDistanceTo(this);
	  }

	  /*
	  {

		  DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12.0f, FColor::Red, false, 10.0f);
		  if (GameMode && Distance <= 500)
		  {
			  SetGuardState(EAIState::Alermed);
			  GameMode->CompleteMission(SeenPawn, false);
			  PawnSensingComp->DestroyComponent();


		  }
		  else if (Distance > 500 && Distance <= 3000)
		  {
			  SetGuardState(EAIState::Alermed);
			  UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), SeenPawn->GetActorLocation());
		  }
		  else if (Distance > 2000)
		  {
			  MoveToNextPatrolPoint();
		  }
	  }*/
  }
	

	

}

void AAIGuard::OnHearNoise(APawn* HearPawn, const FVector& Location, float Volume)
{
	if (!IsDead && AIController)
	{
		if (GuardState == EAIStateTest::Alermed || GuardState == EAIStateTest::SearchingPlayer)
		{
			return;
		}

		DrawDebugSphere(GetWorld(), Location, 32.0f, 12.0f, FColor::Green, false, 10.0f);
		//UE_LOG(LogTemp, Log, TEXT("Find you!"));

		AIController->MyAIState = EAIState::HearPlayer;
		//FVector Direction = Location - GetActorLocation();
		//Direction.Normalize();
		//FRotator NewRotator = FRotationMatrix::MakeFromX(Direction).Rotator();
		//NewRotator.Pitch = 0.0f;
		//NewRotator.Roll = 0.0f;

		//SetActorRotation(NewRotator);

		//GetWorldTimerManager().ClearTimer(TimerForResetRotator);


		GetWorldTimerManager().SetTimer(TimerForResetRotator, this, &AAIGuard::ResetOriginalRotation, 3.0f);


		SetGuardState(EAIStateTest::Subspicious);

		
		if (AIController)
		{
			//AIController->StopMovement();
		}
	}
	
	
	
}

void AAIGuard::ResetOriginalRotation()
{
	if (GuardState == EAIStateTest::Alermed)
	{
		return;
	}
	SetActorRotation(OriginalRotation);

	SetGuardState(EAIStateTest::Idle);

	if (PatrolPoint[CurrentPatrolPointNum])
	{
		MoveToNextPatrolPoint();
	}
}

void AAIGuard::ResetFocus()
{
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
	SetGuardState(EAIStateTest::Idle);
}

void AAIGuard::SetGuardState(EAIStateTest NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;



	MeshComp->SetMaterial(1, StateMap[NewState].StateMaterialsls);
	UGameplayStatics::PlaySoundAtLocation(this, StateMap[NewState].StateSounds, GetActorLocation());


	
	
	OnStateChange(GuardState);
}

void AAIGuard::MoveToNextPatrolPoint()
{
	if (CurrentPatrolPointNum == (PatrolPoint.Num()-1))
	{
		CurrentPatrolPointNum = 0;
	}
	else
	{
		CurrentPatrolPointNum ++;
	}
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), PatrolPoint[CurrentPatrolPointNum]);
	
}

void AAIGuard::DeathAnimation()
{
	DefaultCapsuleComp->SetSimulatePhysics(true);
	MeshComp->SetSimulatePhysics(true);
}

void AAIGuard::OnHealthChange(UHealthComponent* HealthC, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsDead && Health > 0.0f)
	{
		if (GuardState != EAIStateTest::Alermed)
		{
		    AIController->SetFocus(DamageCauser, EAIFocusPriority::Gameplay);
	
			SetGuardState(EAIStateTest::Subspicious);
		
	
		    GetWorldTimerManager().SetTimer(TimerForResetRotator, this, &AAIGuard::ResetFocus, 2.0f);
		}
	}
	if (!IsDead && Health <= 0.0f)
	{
		bPatorl = false;
		IsDead = true;

		GuardState = EAIStateTest::Dead;
		MeshComp->SetMaterial(1, DeathMaterial);
		if (AIDeathSounds[0])
		{
			int x = rand() % (AIDeathSounds.Num());
			UGameplayStatics::PlaySoundAtLocation(this,AIDeathSounds[x], GetActorLocation());
		}
		GetMovementComponent()->StopMovementImmediately();
		
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		DetachFromControllerPendingDestroy();
		if (DefaultCapsuleComp)
		{
			DefaultCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		
		//DeathCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//DeathCapsuleComp->SetCollisionObjectType(ECC_WorldStatic);
		//DeathCapsuleComp->SetCollisionResponseToAllChannels(ECR_Block);
		//DeathCapsuleComp->setcolli
		//DeathCapsuleComp->SetSimulatePhysics(true);
		//MeshComp->SetSimulatePhysics(true);
		//SetLifeSpan(10.0f);
	}
}

// Called every frame
void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UNavigationSystem::SimpleMoveToActor(GetController(), TestPatrolPoint);
	
	

	
}

TArray<AActor*> AAIGuard::GetPatrolPoints() const
{

		return PatrolPoint;


}

void AAIGuard::TraceToPlayer(ACCharacter* PlayerCharacter)
{
	const FVector TraceBeginLocation = GetMesh()->GetSocketLocation("AIEyeSocket");
	FVector TraceEndLocation;

	if (PlayerCharacter)
	{
		TraceEndLocation = PlayerCharacter->GetMesh()->GetSocketLocation("TraceLineHeadSocket");
	}
	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceBeginLocation, TraceEndLocation, TRACECHANNEL_AISEEING, QueryParams))
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor->IsA<ACCharacter>() || PlayerCharacter->GetIsInvisible())
		{
			//UE_LOG(LogTemp, Log, TEXT("Where are you?!"));
			if (AIController)
			{
				
				GuardState = EAIStateTest::SearchingPlayer;
			}
			//SetGuardState(EAIStateTest::Idle);
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("I can see you Now!"));
		}
		
	}
	
	//UE_LOG(LogTemp, Log, TEXT("Find you!"));
}

