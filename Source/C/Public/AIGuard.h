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

	/**����״̬*/
	UPROPERTY(BlueprintReadOnly, Category = "AI")
		bool IsDead;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
		bool IsMarked;

	/**��Ϊ����ɣ�����ͼ��ѡ����Ϊ��*/
	UPROPERTY(EditAnywhere, Category = "Behavior")
		UBehaviorTree* AIBehaviorTree;

	UUserWidget_AITracking* MyMark;

	/**��ȡѲ�ߵ����� ����AIController��Blackboard��ֵ*/
	TArray<AActor*> GetPatrolPoints() const;

	/**��AI�������ߵ����ͷ����ۣ������ж��Ƿ�������Ұ*/
	UFUNCTION()
	    void TraceToPlayer(ACCharacter* PlayerCharacter);

	/**�趨AI�ڲ�״̬������UI��ȡ��ʾ*/
	void SetGuardState(EAIStateTest NewState);

	/**��ȡAI��Ϊ��״̬*/
	EAIStateTest GetGuardState() { return GuardState; }

	/**��ҰѲ�ߵ�*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		TSubclassOf <ATargetPoint> EmptyMemoryMark;

	UFUNCTION(BlueprintCallable, Category = "AI")
		void DeathAnimation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**��֪��*/
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UHealthComponent* HealthComp;


	/**�Ƿ�Ѳ��*/
	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bPatorl;

	/**Ѳ�ߵ����飬����ͼ�����*/
	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatorl"))
		TArray<AActor*> PatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatorl"))
		AActor* TestPatrolPoint;

	/**������Ч����*/
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


	/**�Ӿ���֪����*/
	UFUNCTION()
	void OnSeenPawn(APawn* SeenPawn);

	/**������֪����*/
	UFUNCTION()
	void OnHearNoise(APawn* HearPawn, const FVector& Location, float Volume);



	UFUNCTION()
	void ResetOriginalRotation();


	UFUNCTION()
		void ResetFocus();

	/**��ͼʵ�ַ��̣�����AI����״̬�ı�������UI*/
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
