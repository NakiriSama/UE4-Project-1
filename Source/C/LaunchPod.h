// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPod.generated.h"

UCLASS()
class C_API ALaunchPod : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPod();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* OverlapComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundBase* PickupAmmoSounds;

	UFUNCTION()
		void PodOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	UPROPERTY(EditInstanceOnly, Category = "LaunchPod")
	float LaunchPitchAngle;

	UPROPERTY(EditInstanceOnly, Category = "LaunchPod")
	float LaunchStrength;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystem* LaunchFX;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundBase* LaunchSound;

	


	

};
