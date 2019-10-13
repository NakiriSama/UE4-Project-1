// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIGuard.generated.h"

UENUM(BlueprintType)
enum class EAIStateTest:uint8
{
	Idle,

    Subspicious,

    Alermed,

	SearchingPlayer

};

class UPawnSensingComponent;
class UHealthComponent;
class UBehaviorTree;
class AMyAIController;
class ACCharacter;
class AAI_TargetPoint;
class ATargetPoint;

UCLASS()
class C_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuard();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
		bool IsDead;

	UPROPERTY(EditAnywhere, Category = "Behavior")
		UBehaviorTree* AIBehaviorTree;

	TArray<AActor*> GetPatrolPoints() const;

	UFUNCTION()
	void TraceToPlayer(ACCharacter* PlayerCharacter);

	void SetGuardState(EAIStateTest NewState);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	    TSubclassOf <ATargetPoint> EmptyMemoryMark;

	EAIStateTest GetGuardState() { return GuardState; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UHealthComponent* HealthComp;


	UFUNCTION()
	void OnSeenPawn(APawn* SeenPawn);

	UFUNCTION()
	void OnHearNoise(APawn* HearPawn, const FVector& Location, float Volume);

	FRotator OriginalRotation;
	FTimerHandle TimerForResetRotator;

	UFUNCTION()
	void ResetOriginalRotation();

	EAIStateTest GuardState;

	UFUNCTION()
		void ResetFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChange(EAIStateTest NewState);

	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bPatorl;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatorl"))
	TArray<AActor*> PatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatorl"))
	AActor* TestPatrolPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		TArray<USoundBase*> AIDeathSounds;

	int32 CurrentPatrolPointNum;

	void MoveToNextPatrolPoint();

	UFUNCTION()
		void OnHealthChange(UHealthComponent* HealthC, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	AMyAIController* AIController;

	
private:


};
