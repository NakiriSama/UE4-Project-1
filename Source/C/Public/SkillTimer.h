// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillTimer.generated.h"

/**
*����ͼ��ѡ����Ӧ�ļ������
*/
UENUM(BlueprintType)
enum class SkillType : uint8 {
	INVISIBILITY,
	X_RAY
};

class ACCharacter;

UCLASS()
class C_API ASkillTimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillTimer();

	/**���ܳ���ʱ���ʱ����ʼ��ʱ*/
	void SkillTimerBegin();

	/**������ȴʱ���ʱ����ʼ��ʱ*/
	void CooldownTimerBegin();

	/**�����ʱ�������ڸ���ɫ�����*/
	void ClearTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Time")
		float Duration;

	
	UPROPERTY(EditDefaultsOnly, Category = "Time")
	 float Cooldown;

	FTimerHandle TimerHandle_Skill;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	SkillType CurrentSkill;
    

	//ACCharacter* MyCharacter;

	/**���ܳ���ʱ���ʱ��ֹͣ��ʱ*/
	UFUNCTION()
	void SkillTimerEnd();

	/**������ȴʱ���ʱ��ֹͣ��ʱ*/
	void CooldownTimerEnd();

	/**���ü��ܴ�������*/
	void SkillFunction();

	/**������ȴ�ж��趨����*/
	void SkillState();


private:	


};
