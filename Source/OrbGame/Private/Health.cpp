// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

// Sets default values for this component's properties
UHealth::UHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealth::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UHealth::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

float UHealth::GetHealth()
{
	return Health;
}

void UHealth::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
}

float UHealth::GetMaxHealth()
{
	return MaxHealth;
}

void UHealth::SetStartHealth(float NewStartHealth)
{
	StartHealth = NewStartHealth;
}

bool UHealth::IsDead()
{
	return Health <= 0;
}