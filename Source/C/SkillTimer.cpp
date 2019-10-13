/**
*���ܼ�ʱ����
*�����趨����ʱ������ȴʱ��
*��Ҫ���ý�ɫ����ļ��ܴ�������
*��Ҫ���ý�ɫ����ļ�����ȴ״̬����
*/

#include "SkillTimer.h"
#include "TimerManager.h"
#include "CCharacter.h"

// Sets default values
ASkillTimer::ASkillTimer()
{
	//����ʱ�����ȴʱ�䣬��������ͼ����ֵ
	Duration = 0.0f;
	Cooldown = 0.0f;
	//MyCharacter = Cast<ACCharacter>(GetOwner());
}

// Called when the game starts or when spawned
void ASkillTimer::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Log, TEXT("%s"), TEXT("Spawn success!!"));
	
	SkillTimerBegin();
	//SkillFunction();
	
}

void ASkillTimer::SkillState()
{
	ACCharacter* MyCharacter = Cast<ACCharacter>(GetOwner());
	switch (CurrentSkill)
	{
	case SkillType::INVISIBILITY:
		MyCharacter->SetCanBeInvisible();
		break;
	case SkillType::X_RAY:
		MyCharacter->SetCanBeInXRay();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("Need to set Skill Type"));
		break;
	}

}

void ASkillTimer::SkillFunction()
{
	ACCharacter* MyCharacter = Cast<ACCharacter>(GetOwner());
		switch (CurrentSkill)
		{
		case SkillType::INVISIBILITY:
		
			MyCharacter->ToggleInvisibility();
			break;
		case SkillType::X_RAY:
			MyCharacter->ToggleXRay();
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("Need to set Skill Type"));
			break;
		}
	

}






void ASkillTimer::ClearTimer()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Skill);
	UE_LOG(LogTemp, Log, TEXT("%s"), TEXT("Time Cleared"));
	
}


/////////////////////////////////////////////////////////////////////////////////////
/**
*����ʱ���ʱ����ʼ�����
*/

void ASkillTimer::SkillTimerBegin()
{
	if (Duration > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_Skill, this, &ASkillTimer::SkillTimerEnd, Duration);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("Need to set Skill Duration"));
	}
}

void ASkillTimer::SkillTimerEnd()
{
	SkillFunction();
	ClearTimer();
	CooldownTimerBegin();

}

/////////////////////////////////////////////////////////////////////////////////////

/**
*��ȴʱ���ʱ����ʼ�����
*/

void ASkillTimer::CooldownTimerBegin()
{
	if (Cooldown > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_Skill, this, &ASkillTimer::CooldownTimerEnd, Cooldown);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("Need to set Skill Cooldown"));
	}
}

void ASkillTimer::CooldownTimerEnd()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Skill);
	SkillState();
	Destroy();
}

