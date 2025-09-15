// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/OrbGameGameplayAbility.h"
#include "AbilitySystemComponent.h"

void UOrbGameGameplayAbility::SetActorInfo(AActor* AvatarActor, AActor* OwnerActor)
{
    FGameplayAbilityActorInfo ActorInfo;
    UAbilitySystemComponent* AbilitySystemComponent = Cast<UAbilitySystemComponent>(OwnerActor->GetComponentByClass(UAbilitySystemComponent::StaticClass()));
    ActorInfo.InitFromActor(OwnerActor, AvatarActor, AbilitySystemComponent);
    const FGameplayAbilityActorInfo* ConstActorInfo = &ActorInfo;

    FGameplayAbilitySpecHandle Handle = GetCurrentAbilitySpecHandle();
    SetCurrentActorInfo(Handle, ConstActorInfo);
}
