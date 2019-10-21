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

	SearchingPlayer,

	Dead

};
USTRUCT()
struct FStateStruct
{
	GENERATED_BODY()

	UPROPERTY()
	UMaterialInterface* StateMaterialsls;
	UPROPERTY()
	USoundBase* StateSounds;
};

class UPawnSensingComponent;
class UHealthComponent;
class UBehaviorTree;
class AMyAIController;
class ACCharacter;
class AAI_TargetPoint;
class ATargetPoint;
class UMaterialInterface;
class UCapsuleComponent;
class UUserWidget_AITracking;

UCLASS()
class C_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuard();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**死亡状态*/
	UPROPERTY(BlueprintReadOnly, Category = "AI")
		bool IsDead;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
		bool IsMarked;

	/**行为树组成，在蓝图中选择行为树*/
	UPROPERTY(EditAnywhere, Category = "Behavior")
		UBehaviorTree* AIBehaviorTree;

	UUserWidget_AITracking* MyMark;

	/**获取巡逻点数组 用于AIController给Blackboard赋值*/
	TArray<AActor*> GetPatrolPoints() const;

	/**从AI发射射线到玩家头部插槽，用于判定是否脱离视野*/
	UFUNCTION()
	    void TraceToPlayer(ACCharacter* PlayerCharacter);

	/**设定AI内部状态，用于UI读取显示*/
	void SetGuardState(EAIStateTest NewState);

	/**获取AI行为树状态*/
	EAIStateTest GetGuardState() { return GuardState; }

	/**视野巡逻点*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		TSubclassOf <ATargetPoint> EmptyMemoryMark;

	UFUNCTION(BlueprintCallable, Category = "AI")
		void DeathAnimation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**感知类*/
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UHealthComponent* HealthComp;


	/**是否巡逻*/
	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bPatorl;

	/**巡逻点数组，在蓝图中添加*/
	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatorl"))
		TArray<AActor*> PatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatorl"))
		AActor* TestPatrolPoint;

	/**死亡音效数组*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		TArray<USoundBase*> AIDeathSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	    UMaterialInterface* IdleMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	    UMaterialInterface* SubspiciousMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
		UMaterialInterface* AlermedMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
		UMaterialInterface* DeathMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		USoundBase* IdleSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		USoundBase* SubspiciousSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		USoundBase* AlermedSound;
		


	AMyAIController* AIController;

	EAIStateTest GuardState;

	int32 CurrentPatrolPointNum;

	USkeletalMeshComponent* MeshComp;


	/**视觉感知方程*/
	UFUNCTION()
	void OnSeenPawn(APawn* SeenPawn);

	/**听觉感知方程*/
	UFUNCTION()
	void OnHearNoise(APawn* HearPawn, const FVector& Location, float Volume);



	UFUNCTION()
	void ResetOriginalRotation();


	UFUNCTION()
		void ResetFocus();

	/**蓝图实现方程，用于AI自身状态改变来更改UI*/
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChange(EAIStateTest NewState);

	UFUNCTION()
		void OnHealthChange(UHealthComponent* HealthC, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void MoveToNextPatrolPoint();

	UPROPERTY()
	TMap<EAIStateTest, FStateStruct> StateMap;


	
private:

	FRotator OriginalRotation;

	FTimerHandle TimerForResetRotator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* DeathCapsuleComp;

	UPROPERTY()
	    UCapsuleComponent* DefaultCapsuleComp;
};
