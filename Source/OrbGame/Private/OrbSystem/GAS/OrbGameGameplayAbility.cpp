// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/OrbGameGameplayAbility.h"
#include "AbilitySystemComponent.h"



void UOrbGameGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    const UOrbUseContextWrapper* Wrapper = Cast<UOrbUseContextWrapper>(TriggerEventData->OptionalObject);
    if(Wrapper)
    {
        OrbUseContext = Wrapper->OrbUseContext;
    }
}


void UOrbGameGameplayAbility::SetActorInfo(AActor* AvatarActor, AActor* OwnerActor)
{
    FGameplayAbilityActorInfo ActorInfo;
    UAbilitySystemComponent* AbilitySystemComponent = Cast<UAbilitySystemComponent>(OwnerActor->GetComponentByClass(UAbilitySystemComponent::StaticClass()));
    ActorInfo.InitFromActor(OwnerActor, AvatarActor, AbilitySystemComponent);
    const FGameplayAbilityActorInfo* ConstActorInfo = &ActorInfo;

    FGameplayAbilitySpecHandle Handle = GetCurrentAbilitySpecHandle();
    SetCurrentActorInfo(Handle, ConstActorInfo);
}


