// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_AITracking.generated.h"

/**
 * 
 */
UCLASS()
class C_API UUserWidget_AITracking : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UUserWidget_AITracking(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	bool TrackTracingline(FVector TraceStart, FVector TraceEnd, const bool IsInXray);
	
protected:
private:
	
};
