// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/EffectStateManager.h"
#include "Engine/World.h"


UEffectStateManager::UEffectStateManager()
{
    DefaultEffectTags = TArray<FGameplayTag>();
    EffectStates = TMap<FGameplayTag, FEffectState>();
}

bool UEffectStateManager::CanActivateEffect(FGameplayTag EffectTag, float EffectCooldown) const
{
    if(EffectStates.Contains(EffectTag))
    {
        const FEffectState& EffectState = EffectStates[EffectTag];
        double CurrentTime = GetWorld()->GetTimeSeconds();
        return !EffectState.bIsEffectActive && (CurrentTime - EffectState.TimeOfLastEffect) >= EffectCooldown;
    }
    return true;
}

bool UEffectStateManager::ActivateEffect(FGameplayTag EffectTag, float EffectCooldown)
{

    if(CanActivateEffect(EffectTag, EffectCooldown))
    {
        if(!EffectStates.Contains(EffectTag))
        {
            EffectStates.Add(EffectTag, FEffectState());
        }
        FEffectState& EffectState = EffectStates[EffectTag];
        EffectState.bIsEffectActive = true;
        EffectState.TimeOfLastEffect = GetWorld()->GetTimeSeconds();
        return true;
    }
    return false;
}

void UEffectStateManager::DeactivateEffect(FGameplayTag EffectTag)
{
    if(EffectStates.Contains(EffectTag))
    {
        FEffectState& EffectState = EffectStates[EffectTag];
        if(EffectState.bIsEffectActive)
        {
            EffectState.bIsEffectActive = false;
            EffectState.TimeOfLastEffect = GetWorld()->GetTimeSeconds();
        }
    }
}