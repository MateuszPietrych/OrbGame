// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbEffectBase.h"


void UOrbEffectBase::ApplyEffect(FOrbEffectData OrbEffectData)
{
    // Default implementation can be empty or provide basic functionality
}

TArray<AActor*> UOrbEffectBase::GetActorsAffected(FOrbEffectData OrbEffectData)
{
    // Default implementation can be empty or provide basic functionality
    return TArray<AActor*>();
}

void UOrbEffectBase::SetStartLocation(const FVector& NewStartLocation)
{
    StartLocation = NewStartLocation;
}

void UOrbEffectBase::SetDirection(const FVector& NewDirection)
{
    Direction = NewDirection;
}