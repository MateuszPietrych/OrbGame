// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbGameBlueprintLibrary.h"
#include "OrbGameStructs.h"
#include "OrbGameGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"

void UOrbGameBlueprintLibrary::DealDamage(FDamageEffectParams DamageParams)
{
    if (!DamageParams.TargetAbilitySystemComponent || !DamageParams.SourceAbilitySystemComponent || !DamageParams.DamageGameplayEffectClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("DealDamage: Invalid parameters"));
        return;
    }
    FGameplayEffectContextHandle EffectContext = DamageParams.SourceAbilitySystemComponent->MakeEffectContext();
    EffectContext.AddSourceObject(DamageParams.SourceAbilitySystemComponent->GetOwnerActor());
    FGameplayEffectSpecHandle DamageSpecHandle = DamageParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageParams.DamageGameplayEffectClass, DamageParams.AbilityLevel, EffectContext);
    const float ScaledDamage = DamageParams.GetDamageAtLevel();
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FOrbGameGameplayTags::Get().Effect_Damage, ScaledDamage);

    DamageParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
}

// void UOrbGameBlueprintLibrary::CauseDamage(AActor* TargetActor, UGameplayAbility* SourceAbility, FDamageEffectParams DamageParams)
// {
// 	FGameplayEffectSpecHandle DamageSpecHandle = SourceAbility->MakeOutgoingGameplayEffectSpec(DamageParams.DamageGameplayEffectClass, 1.f);
// 	const float ScaledDamage = DamageParams.Damage;

// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FOrbGameGameplayTags::Get()->Effect_Damage, ScaledDamage);
//     UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
// 	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
// }
