// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CGameMode.h"
#include "CCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MyAIController.h"
#include "UserWidget_AlertLevels.h"


ACGameMode::ACGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//PlayerControllerClass = AMyPlayerController::StaticClass();
}


void ACGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (AlertLevelWidgetBP)
	{
		CurrentAlertLevelWidget = CreateWidget<UUserWidget_AlertLevels>(GetWorld(), AlertLevelWidgetBP);
		if (CurrentAlertLevelWidget)
		{
			CurrentAlertLevelWidget->AddToViewport();
		}
	}

	
}



void ACGameMode::CompleteMission(APawn* InstigationPawn, bool MissionSuccess)
{
	if (InstigationPawn)
	{
		InstigationPawn->DisableInput(nullptr);
		if (ViewpointClass)
		{
			AActor* ViewPoint = nullptr;

			TArray<AActor*> ReturnedActor;

			UGameplayStatics::GetAllActorsOfClass(this, ViewpointClass, ReturnedActor);
			if (ReturnedActor.Num() > 0)
			{
				ViewPoint = ReturnedActor[0];

				APlayerController* PC = Cast<APlayerController>(InstigationPawn->GetController());
				if (PC)
				{
					PC->SetViewTargetWithBlend(ViewPoint, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					if (MissionSuccess == false) {
						UGameplayStatics::PlaySound2D(this, GameOverSound);
					}
				}
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ViewpointClass is nullptr"));
		}

		

	
	}

	OnMissionCompleted(InstigationPawn,  MissionSuccess);
}
float ACGameMode::GetLoseAlertLevels() const
{
	return LoseAlertLevels;
}

