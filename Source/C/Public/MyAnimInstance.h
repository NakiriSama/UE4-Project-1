// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class C_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyAnimInstance();

	void Reload(uint8 Mod);
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool IsInCover;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* RelaxReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* AimReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* CoverReloadMontage;

	

private:

	
};
