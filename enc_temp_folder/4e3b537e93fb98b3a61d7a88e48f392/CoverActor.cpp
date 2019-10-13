// Fill out your copyright notice in the Description page of Project Settings.
/**Creating a Cover System By Orfeas
*https://orfeasel.com/creating-a-cover-system/
*ÑÚÌåÏµÍ³
*/
#include "CoverActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "CCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"

void ACoverActor::OnCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA<ACCharacter>())
	{
		ACCharacter* MyCharacter = Cast<ACCharacter>(OtherActor);
		MyCharacter->SetCanTakeCover(true,this);
		if (StaticMeshComp->GetComponentScale().Z >= 2.f)
		{
			MyCharacter->SetIsStandCover(true);
		}
		else {
			MyCharacter->SetIsStandCover(false);
		}
		
	}
}

void ACoverActor::OnCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<ACCharacter>())
	{
		ACCharacter* MyCharacter = Cast<ACCharacter>(OtherActor);
		if (!MyCharacter->GetIsInCorner())
		{
			MyCharacter->SetCanTakeCover(false, this);
		}
		
		
	}
}

bool ACoverActor::IsCloseToPlayer(FName SocketName)
{
	TArray<FHitResult> HitResults;
	ACCharacter* Player = Cast<ACCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	const FVector StartLocation = StaticMeshComp->GetSocketLocation(SocketName);
	const FVector EndLocation = Player->GetActorLocation();
	//²âÊÔ
	//float length = (StartLocation - EndLocation).Size();
	FCollisionShape Shape;
	Shape.ShapeType = ECollisionShape::Line;
	GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat(), ECC_WorldDynamic, Shape);
	return HitResults.ContainsByPredicate([&](FHitResult hitResult) 
	{
		AActor* HitActor = hitResult.GetActor();
		return HitActor && HitActor->IsA<ACCharacter>();
	}
	);
}


FName ACoverActor::GetNearbySocket()
{
	const FName AvailableSockets[4]=
	{
		FName("ForwardSocket"),
		FName("BackwardSocket"),
		FName("RightSocket"),
		FName("LeftSocket")
	};
	for (uint8 SocketPtr = 0; SocketPtr < 4; SocketPtr++)
	{
		if (IsCloseToPlayer(AvailableSockets[SocketPtr]))
		{
			return AvailableSockets[SocketPtr];
		}
	}
	return AvailableSockets[0];
}

void ACoverActor::DetermineMovementDirection(FVector& MovementDirection, FRotator& FacingDirection)
{
	FName NearbySocket = GetNearbySocket();
	
	if (NearbySocket.IsEqual("ForwardSocket"))
	{
		MovementDirection = -GetActorRightVector();
		FacingDirection = GetActorRotation() + FRotator(0, 180, 0);
	}
	else if (NearbySocket.IsEqual("BackwardSocket"))
	{
		MovementDirection = GetActorRightVector();
		FacingDirection = GetActorRotation() ;
	}
	else if (NearbySocket.IsEqual("RightSocket"))
	{
		MovementDirection = GetActorForwardVector();
		FacingDirection = GetActorRotation() + FRotator(0, -90, 0);
	}
	else
	{
		MovementDirection = -GetActorForwardVector();
		FacingDirection = GetActorRotation() + FRotator(0, 90.f, 0);
	}
}

bool ACoverActor::IsCloseToCorner()
{
	const FName AvailableSockets[4] =
	{
		FName("FR"),
		FName("FL"),
		FName("BR"),
		FName("BL")
	};
	ACCharacter* Player = Cast<ACCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	FVector PlayerLocation = Player->GetActorLocation();
	PlayerLocation.Z = 0;
	for (uint8 SocketPtr = 0; SocketPtr < 4; SocketPtr++)
	{
		 FVector SocketLocation = StaticMeshComp->GetSocketLocation(AvailableSockets[SocketPtr]);
		 SocketLocation.Z = 0;
		 
		 FVector NewLocation = SocketLocation - PlayerLocation;
		 if (NewLocation.Size() <TestLength)
		 {
			 return true;
		 }
	}
	return false;
}

// Sets default values
ACoverActor::ACoverActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(StaticMeshComp);
	

}

// Called when the game starts or when spawned
void ACoverActor::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComp->SetCanEverAffectNavigation(true);
	if (BoxComp)
	{
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACoverActor::OnCompBeginOverlap);
		BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACoverActor::OnCompEndOverlap);
	}
	
}



void ACoverActor::PublicDetermineMovementDirection(FVector& MovementDirection, FRotator& FacingDirection)
{
	DetermineMovementDirection(MovementDirection, FacingDirection);
}

