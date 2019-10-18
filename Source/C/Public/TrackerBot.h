// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrackerBot.generated.h"

UCLASS()
class C_API ATrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;


	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bPatorl;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatorl"))
		TArray<AActor*> PatrolPoint;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UPawnSensingComponent* PawnSensingComp;



	UFUNCTION()
		void OnSeenPawn(APawn* SeenPawn);

	UFUNCTION()
		void OnHearNoise(APawn* HearPawn, const FVector& Location, float Volume);

	FVector GetNextPathPoint();

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UFloatingPawnMovement* FloatingComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UHealthComponent* HealthComp;
	UFUNCTION()
	void WhenTakeDamage(UHealthComponent* HealthC, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	


};
