// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameMode.generated.h"

class UUserWidget_AlertLevels;

UCLASS(minimalapi)
class ACGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<AActor> ViewpointClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundBase*  GameOverSound;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		TSubclassOf <UUserWidget_AlertLevels> AlertLevelWidgetBP;

	UPROPERTY()
		UUserWidget_AlertLevels* CurrentAlertLevelWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Alert")
		float LoseAlertLevels;

public:
	ACGameMode();

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetLoseAlertLevels() const;

	void CompleteMission(APawn* InstigationPawn, bool MissionSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InstigationPawn, bool MissionSuccess);

	
};



