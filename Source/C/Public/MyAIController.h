// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Patrol UMETA(DisplayName = "Patrol"),
	SeePlayer UMETA(DisplayName = "SeePlayer"),
	HearPlayer UMETA(DisplayName = "HearPlayer"),
	LostPlayer UMETA(DisplayName = "LostPlayer")
};
class UBlackboardComponent;
class UBehaviorTreeComponent;
UCLASS()
class C_API AMyAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(transient)
	    UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
		UBehaviorTreeComponent* BehaviorComp;

	TArray<AActor*> PatrolPoints;





public:

	AMyAIController();

	virtual void OnPossess(APawn* InPawn) override;

	uint8 PlayerKeyID;

	uint8 StateKeyID;

	uint8 NextPatrolPointID;

	uint8 MemoryMarkID;

	EAIState MyAIState;

	int32 CurrentPatorlPoint;

	TArray<AActor*> GetPatorlPoints() const { return PatrolPoints; }

	UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
