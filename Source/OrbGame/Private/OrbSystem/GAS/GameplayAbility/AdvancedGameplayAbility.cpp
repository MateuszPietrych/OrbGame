// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/GameplayAbility/AdvancedGameplayAbility.h"


void UAdvancedGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    const UOrbUseContextWrapper* Wrapper = Cast<UOrbUseContextWrapper>(TriggerEventData->OptionalObject);
    if(Wrapper)
    {
        OrbUseContext = Wrapper->OrbUseContext;
    }
    GetWorld()->GetTimerManager().SetTimer(AbilityTickTimer, this, &UAdvancedGameplayAbility::AbilityTickAction, AbilityTickInterval, true);
}


void UAdvancedGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    GetWorld()->GetTimerManager().ClearTimer(AbilityTickTimer);
}

void UAdvancedGameplayAbility::AbilityTickAction_Implementation()
{
    
}
