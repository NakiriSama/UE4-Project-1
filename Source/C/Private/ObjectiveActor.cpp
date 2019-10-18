// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CCharacter.h"


// Sets default values
AObjectiveActor::AObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent= MeshComp;
	
	//�����������Ϊû����ײ���
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*�������������Ϊ����ײ�����ֻ����ѯ�ʡ����Գ���Pawn�����ͨ����������Pawn�ص�
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	*/
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	//�����������ײ�������Ϊֻ����ѯ�ʡ����Գ���Pawn�����ͨ������������Pawn�ص�
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	SphereComp->SetupAttachment(MeshComp);

	
}

// Called when the game starts or when spawned
void AObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	
}

//������Ч
void AObjectiveActor::PlayEffect()
{
	//�ڵ�ǰActorλ�ò�����Ч�� ��Ч��PickupFX
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

// Called every frame


//��Actor��ײʱ����PlayEffect()
void AObjectiveActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	PlayEffect();

	ACCharacter* Mycharacter = Cast<ACCharacter>(OtherActor);
	if (Mycharacter)
	{
		Mycharacter->bIsCarryingObjective = true;
		UGameplayStatics::PlaySound2D(this, PickupObjective);
		Destroy();
	}

}

