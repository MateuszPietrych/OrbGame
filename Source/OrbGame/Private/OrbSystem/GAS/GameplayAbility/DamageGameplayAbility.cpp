// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/GameplayAbility/DamageGameplayAbility.h"
#include "OrbGameBlueprintLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OrbGameGameplayTags.h"
#include "AbilitySystemComponent.h"

UDamageGameplayAbility::UDamageGameplayAbility()
{
	DamageEffectParams.DamageGameplayAbilityInstance = this;
}


void UDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectParams.DamageGameplayEffectClass, 1.f);
	const float ScaledDamage = DamageEffectParams.Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FOrbGameGameplayTags::Get().Effect_Damage, ScaledDamage);

    GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}
