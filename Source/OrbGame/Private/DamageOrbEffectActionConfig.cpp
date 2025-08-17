// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageOrbEffectActionConfig.h"
#include "Kismet/GameplayStatics.h" 


void UDamageOrbEffectActionConfig::ApplyEffect(AActor* Target)
{
    UGameplayStatics::ApplyDamage(Target, Damage, nullptr, nullptr, nullptr);
}

bool UDamageOrbEffectActionConfig::IsValidConfig(FString& OutError) const
{
    if(Damage < 0)
    {
        OutError = TEXT("Damage must be greater or equal to zero.");
        return false;
    }
    return true;
}