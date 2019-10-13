// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChangeAIState.h"
#include "C.h"
#include "AIGuard.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MyAIController.h"
#include "CCharacter.h"


EBTNodeResult::Type UBTTaskNode_ChangeAIState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)

{

	AMyAIController* AIController = Cast<AMyAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		AAIGuard* AISelf = Cast<AAIGuard>(AIController->GetPawn());
		AISelf->SetGuardState(EAIStateTest::Idle);
		AIController->MyAIState = EAIState::Patrol;
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
