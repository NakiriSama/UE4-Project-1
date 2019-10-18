// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

UMyAnimInstance::UMyAnimInstance()
{

}

void UMyAnimInstance::Reload()
{
	Montage_Play(ReloadMontage);
	UE_LOG(LogTemp, Log, TEXT("Begin Reload!"));
}
