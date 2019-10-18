// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100.0f;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AActor* MyActor = GetOwner();
	if (MyActor)
	{
		MyActor->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleDamage);
	}
	Health = DefaultHealth;
}

void UHealthComponent::HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health: %s"), *FString::SanitizeFloat(Health));

	OnHealthChange.Broadcast(this, Health,Damage,DamageType,InstigatedBy,DamageCauser);
}


