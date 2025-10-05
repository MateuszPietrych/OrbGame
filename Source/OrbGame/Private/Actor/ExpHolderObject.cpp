// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ExpHolderObject.h"

// Sets default values
AExpHolderObject::AExpHolderObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExpHolderObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExpHolderObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AExpHolderObject::IsHoldingExp_Implementation()
{
	return bIsHoldingExp;
}

float AExpHolderObject::GetExpAmount_Implementation()
{
	return ExpAmount;
}
