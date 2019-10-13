// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FacsToPlayer.h"
#include "C.h"
#include "AIGuard.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MyAIController.h"
#include "CCharacter.h"

EBTNodeResult::Type UBTTask_FacsToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Succeeded;
}
