// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverActor.generated.h"


UCLASS()
class C_API ACoverActor : public AActor
{
	GENERATED_BODY()

private:
	
	/**当Actor与Box发生重叠*/
	UFUNCTION()
	  void OnCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	/**当Actor与Box脱离重叠*/
	UFUNCTION()
	  void OnCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/**判断玩家是否接近掩体侧面*/
	bool IsCloseToPlayer(FName SocketName);

	/**返回最为接近玩家的掩体侧面*/
	FName GetNearbySocket();

	void DetermineMovementDirection(FVector& MovementDirection, FRotator& FacingDirection);

	

public:	
	ACoverActor();

	bool IsCloseToCorner(float& Direction);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* StaticMeshComp;


	/**设定掩体内的移动方向与面朝方向*/
	void PublicDetermineMovementDirection(FVector& MovementDirection, FRotator& FacingDirection);

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, Category= "Components")
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "Test")
	float TestLength;
	
	/**测试用*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test")
	float Hight;


	

};
