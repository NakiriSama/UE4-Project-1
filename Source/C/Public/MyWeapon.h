// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

class USkeletalMeshComponent;
UCLASS()
class C_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

	/**获取骨架*/
	USkeletalMeshComponent* GetWeaponMeshComp() { return WeaponMeshComp; }

	void StartFire();

	void StopFire();

	/**最大弹量*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Weapon")
		uint8 ClipSetting;

	/**当前存弹量*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		uint8 ClipStock;

	/**最大存弹量*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		uint8 MaxClipStock;

	/**当前弹量*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Weapon")
		uint8 Clip;


	

	/**蓝图实现方程用于在UI上显示子弹数值*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
		void OnFireClipChange();
	UFUNCTION(BlueprintImplementableEvent)
		void ChangeColor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
		void OnReload();

	UFUNCTION(BlueprintCallable)
		void StockReload();

    /**填装弹药调用函数*/
	UFUNCTION(BlueprintCallable)
		void WeaponReload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**内部使用 根据输入参数填装函数*/
	UFUNCTION(BlueprintCallable)
		void ChangeClip(uint8 BulletNumber, bool IsReload);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component" )
	    USkeletalMeshComponent* WeaponMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	    TArray<USoundBase*> AKSingleSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* EmptyGunSounds;

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

private:
	

};
