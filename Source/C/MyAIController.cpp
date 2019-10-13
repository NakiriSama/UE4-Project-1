// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "C.h"
#include "AIGuard.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"


AMyAIController::AMyAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));

	CurrentPatorlPoint = 0;
	MyAIState = EAIState::Patrol;
	
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AAIGuard* AI = Cast<AAIGuard>(InPawn);

	if (AI&&AI->AIBehaviorTree)
	{
		BlackboardComp->InitializeBlackboard(*AI->AIBehaviorTree->BlackboardAsset);

		PlayerKeyID = BlackboardComp->GetKeyID("PlayerActor");

		StateKeyID = BlackboardComp->GetKeyID("AIState");

		MemoryMarkID = BlackboardComp->GetKeyID("MemoryMark");

		NextPatrolPointID = BlackboardComp->GetKeyID("NextPatrolPoint");

		BehaviorComp->StartTree(*AI->AIBehaviorTree);

		PatrolPoints = AI->GetPatrolPoints();
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(BlackboardComp->GetKeyID("SelfActor"), AI);

		BlackboardComp->SetValue<UBlackboardKeyType_Object>(NextPatrolPointID, PatrolPoints[0]);
	}

}
