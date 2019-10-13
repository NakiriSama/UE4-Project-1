// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

UCLASS()
class C_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

	
	class USkeletalMeshComponent* GetWeaponMeshComp() { return WeaponMeshComp; }

	void StartFire();

	void StopFire();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 Clip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		uint8 ClipSetting;

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
		void OnFireClipChange(uint8 CurrentBullet);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component" )
	class USkeletalMeshComponent* WeaponMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	TArray<USoundBase*> AKSingleSounds;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName FireSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName TargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TraceEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool DebugWeaponDrawing;

	void PlayEffects(FVector TraceEnd);

	UPROPERTY(EditDefaultsOnly, Category = "Camera Shake")
	TSubclassOf<UCameraShake> FireShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	float LastFireTime;

	void Fire();

	FVector ScreenCenter;

	FTimerHandle TimerHandleForFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TimeBetweenFire;

public:	
	

};
