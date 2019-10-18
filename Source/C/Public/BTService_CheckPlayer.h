// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPlayer.generated.h"

/**
 * 
 */
UCLASS()
class C_API UBTService_CheckPlayer : public UBTService
{
	GENERATED_BODY()

public:

	UBTService_CheckPlayer();

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


protected:

private:

};
