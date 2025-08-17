// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbEffectBase.h"
#include "OrbEffectActionConfig.h"


void UOrbEffectBase::ApplyEffect(AActor* HitActor)
{
    for (UOrbEffectActionConfig* EffectAction : EffectActions)
    {
        if (!EffectAction)
            continue;

        FString OutError;
        if (EffectAction->IsValidConfig(OutError))
            EffectAction->ApplyEffect(HitActor);
        else
            UE_LOG(LogTemp, Warning, TEXT("Invalid effect action config: %s"), *OutError);
    }
}

TArray<AActor*> UOrbEffectBase::GetActorsAffected()
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

void UOrbEffectBase::ApplyEffectToAffectedActors(TArray<AActor*>* AdditionalAffectedActors)
{
    TArray<AActor*> AffectedActors = GetActorsAffected();
    if (AdditionalAffectedActors)
    {
        AffectedActors.Append(*AdditionalAffectedActors);
    }

    for(AActor* HitActor : AffectedActors)
    {
        if(HitActor == nullptr)
        {
            continue;
        }
        ApplyEffect(HitActor);
    }
}