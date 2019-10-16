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
	
	/**��Actor��Box�����ص�*/
	UFUNCTION()
	  void OnCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	/**��Actor��Box�����ص�*/
	UFUNCTION()
	  void OnCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/**�ж�����Ƿ�ӽ��������*/
	bool IsCloseToPlayer(FName SocketName);

	/**������Ϊ�ӽ���ҵ��������*/
	FName GetNearbySocket();

	void DetermineMovementDirection(FVector& MovementDirection, FRotator& FacingDirection);

	

public:	
	ACoverActor();

	bool IsCloseToCorner(float& Direction);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* StaticMeshComp;


	/**�趨�����ڵ��ƶ��������泯����*/
	void PublicDetermineMovementDirection(FVector& MovementDirection, FRotator& FacingDirection);

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, Category= "Components")
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "Test")
	float TestLength;
	
	/**������*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test")
	float Hight;


	

};
