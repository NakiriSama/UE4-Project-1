// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

UMyAnimInstance::UMyAnimInstance()
{
	IsInCover = false;
}

void UMyAnimInstance::Reload(uint8 Mod)
{
	
	UAnimMontage* NewReloadMontage;
	IsInCover = false;
	switch (Mod)
	{
	case 0:
		
		NewReloadMontage = RelaxReloadMontage;
		break;
	case 1:
		NewReloadMontage = AimReloadMontage;
		break;
	case 2:
		IsInCover = true;
		NewReloadMontage = CoverReloadMontage;
		break;
	default:
		NewReloadMontage = RelaxReloadMontage;
		break;
	}
	
	Montage_Play(NewReloadMontage);
	UE_LOG(LogTemp, Log, TEXT("Begin Reload!"));
}
