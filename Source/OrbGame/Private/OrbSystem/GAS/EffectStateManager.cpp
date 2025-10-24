// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/EffectStateManager.h"
#include "Engine/World.h"


UEffectStateManager::UEffectStateManager()
{
    DefaultEffectTags = TArray<FGameplayTag>();
    EffectStates = TMap<FGameplayTag, FEffectState>();
}

void UEffectStateManager::Initialize(UObject* InWorldContextObject)
{
    WorldContextObject = InWorldContextObject;
}

UWorld* UEffectStateManager::GetWorldChecked() const
{
    if (WorldContextObject)
    {
        return WorldContextObject->GetWorld();
    }

    UE_LOG(LogTemp, Warning, TEXT("EffectStateManager has no valid WorldContextObject!"));
    return nullptr;
}

bool UEffectStateManager::CanActivateEffect(FGameplayTag EffectTag, float EffectCooldown) const
{
    if(EffectStates.Contains(EffectTag))
    {
        const FEffectState& EffectState = EffectStates[EffectTag];
        UWorld* World = GetWorldChecked();
        if(!World) return false;
        double CurrentTime = World->GetTimeSeconds();
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
        UWorld* World = GetWorldChecked();
        if(!World) return false;

        FEffectState& EffectState = EffectStates[EffectTag];
        EffectState.bIsEffectActive = true;
        EffectState.TimeOfLastEffect = World->GetTimeSeconds();
        return true;
    }
    return false;
}

void UEffectStateManager::DeactivateEffect(FGameplayTag EffectTag)
{
    if(EffectStates.Contains(EffectTag))
    {
        UWorld* World = GetWorldChecked();
        if(!World) return;

        FEffectState& EffectState = EffectStates[EffectTag];
        if(EffectState.bIsEffectActive)
        {
            EffectState.bIsEffectActive = false;
            EffectState.TimeOfLastEffect = World->GetTimeSeconds();
        }
    }
}