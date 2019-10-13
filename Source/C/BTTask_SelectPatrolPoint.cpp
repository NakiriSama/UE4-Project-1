// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SelectPatrolPoint.h"
#include "AI_TargetPoint.h"
#include "C.h"
#include "AIGuard.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MyAIController.h"
#include "CCharacter.h"
#include "Classes/Engine/TargetPoint.h"

EBTNodeResult::Type UBTTask_SelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyAIController* AIController = Cast<AMyAIController>(OwnerComp.GetAIOwner());

	
	if (AIController)
	{
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		//AAI_TargetPoint* CurrentPoint = Cast<AAI_TargetPoint>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(AIController->NextPatrolPointID));

		TArray<AActor*> AvailablePatrolPoints = AIController->GetPatorlPoints();

		AAI_TargetPoint* NextPatrolPoint = nullptr;

		if (AIController->CurrentPatorlPoint != AvailablePatrolPoints.Num() - 1)
		{
			NextPatrolPoint = Cast<AAI_TargetPoint>(AvailablePatrolPoints[++AIController->CurrentPatorlPoint]);
		}
		else
		{
			NextPatrolPoint = Cast<AAI_TargetPoint>(AvailablePatrolPoints[0]);
			AIController->CurrentPatorlPoint = 0;
		}
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(AIController->NextPatrolPointID, NextPatrolPoint);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
