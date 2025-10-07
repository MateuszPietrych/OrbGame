// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/GameplayAbility/ChangeStatGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"



void UChangeStatGameplayAbility::ApplyChangeStat()
{
    if (ChangeStatGameplayEffectClass)
    {
        int32 Level = GetAbilityLevel();
        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(ChangeStatGameplayEffectClass, Level);
        if (SpecHandle.IsValid())
        {
            ActiveGameplayEffectHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle);
        }
    }
}

void UChangeStatGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

    if(ActiveGameplayEffectHandle.IsValid())
    {
        UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
        if(!ASC)
        {
            return;
        }
        ASC->RemoveActiveGameplayEffect(ActiveGameplayEffectHandle);
        ActiveGameplayEffectHandle.Invalidate();
    }
}