// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckPlayer.h"
#include "AIGuard.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MyAIController.h"
#include "Engine/TargetPoint.h"
#include "CCharacter.h"
#include "DrawDebugHelpers.h"

UBTService_CheckPlayer::UBTService_CheckPlayer()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACCharacter* Player = nullptr;
	AMyAIController* AIController = Cast<AMyAIController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
	    Player = Cast<ACCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(AIController->PlayerKeyID,Player);
			EAIState NewAIState = AIController->MyAIState;
			
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(AIController->StateKeyID, uint8(NewAIState));
		}

	}
	if (AIController->MyAIState == EAIState::SeePlayer && Player)
	{
		
		AAIGuard* AISelf = Cast<AAIGuard>(AIController->GetPawn());
		AISelf->TraceToPlayer(Player);
		FActorSpawnParameters SpawnParameter;
		FVector MemoryLocation = Player->GetActorLocation();
		MemoryLocation.Z = 130;
		ATargetPoint* MemoryMark = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(AIController->MemoryMarkID));
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		if (MemoryMark)
		{
			
			MemoryMark->SetActorLocation(MemoryLocation);

		}
		else
		{
			MemoryMark = GetWorld()->SpawnActor<ATargetPoint>(AISelf->EmptyMemoryMark, MemoryLocation,Player->GetActorRotation(), SpawnParameter);

		}
		
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(AIController->MemoryMarkID, MemoryMark);
		//DrawDebugSphere(GetWorld(), MemoryMark->GetActorLocation(), 32.0f, 12.0f, FColor::Purple, false, 10.0f);
		if (AISelf->GetGuardState() == EAIStateTest::SearchingPlayer)
		{
			if (MemoryMark)
			{
				AIController->MyAIState = EAIState::LostPlayer;
			}
			else
			{
				AIController->MyAIState = EAIState::Patrol;
			}
			
		}
		
	}
}
