// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameMode.generated.h"

UCLASS(minimalapi)
class ACGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<AActor> ViewpointClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundBase*  GameOverSound;

public:
	ACGameMode();

	void CompleteMission(APawn* InstigationPawn, bool MissionSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InstigationPawn, bool MissionSuccess);

	
};



