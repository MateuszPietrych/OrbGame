// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/GameplayAbility/DamageGameplayAbility.h"
#include "OrbGameBlueprintLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OrbGameGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "OrbSystem/GAS/AbilityDataAsset.h"
#include "Utility/DamageDataAsset.h"
#include "OrbSystem/GAS/OrbUserAbilitySystemComponent.h"

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

void UDamageGameplayAbility::SetBaseDamageParams()
{
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if(!DamageEffectParams.SourceAbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UProjectileGameplayAbility::SpawnProjectile: SourceAbilitySystemComponent is null"));
	}
	DamageEffectParams.WorldContextObject = GetWorld();

	UOrbUserAbilitySystemComponent* OrbUserASC = Cast<UOrbUserAbilitySystemComponent>(DamageEffectParams.SourceAbilitySystemComponent);
	DamageEffectParams.AbilityLevel = OrbUserASC != nullptr && AbilityDataAsset ? OrbUserASC->GetAbilityLevel(AbilityDataAsset->AbilityTag) : 1;

}
