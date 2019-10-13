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
	
	//网格组件设置为没有碰撞体积
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*将网格组件设置为有碰撞体积，只允许询问。忽略除了Pawn以外的通道，不能与Pawn重叠
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	*/
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	//球体组件的碰撞体积设置为只允许询问。忽略除了Pawn以外的通道，即可以与Pawn重叠
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

//播放特效
void AObjectiveActor::PlayEffect()
{
	//在当前Actor位置播放特效， 特效名PickupFX
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

// Called every frame


//与Actor碰撞时调用PlayEffect()
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

