// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeOrbEffectActionConfig.h"

void URangeOrbEffectActionConfig::ApplyEffect(AActor* Target)
{
    // No effect to apply
}

bool URangeOrbEffectActionConfig::IsValidConfig(FString& OutError) const
{
    if(Range < 0)
    {
        OutError = TEXT("Range must be greater or equal to zero.");
        return false;
    }
    return true;
}