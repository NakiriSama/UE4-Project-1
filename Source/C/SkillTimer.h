// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillTimer.generated.h"

/**
*在蓝图里选择相应的技能类别
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

	/**技能持续时间计时器开始计时*/
	void SkillTimerBegin();

	/**技能冷却时间计时器开始计时*/
	void CooldownTimerBegin();

	/**清除计时器，用于给角色类调用*/
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

	/**技能持续时间计时器停止计时*/
	UFUNCTION()
	void SkillTimerEnd();

	/**技能冷却时间计时器停止计时*/
	void CooldownTimerEnd();

	/**调用技能触发方程*/
	void SkillFunction();

	/**调用冷却判定设定方程*/
	void SkillState();


private:	


};
