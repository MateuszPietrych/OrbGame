// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/OrbGameAttributeSet.h"
#include "OrbGameGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "GameplayTagContainer.h"


UOrbGameAttributeSet::UOrbGameAttributeSet()
{
	const FOrbGameGameplayTags& GameplayTags = FOrbGameGameplayTags::Get();
	TagsToAttributes.Add(GameplayTags.Attribute_Health, GetHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Speed, GetSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Tenacity, GetTenacityAttribute);
}

void UOrbGameAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Data);
	}else if (Data.EvaluatedData.Attribute == GetIncomingHealAttribute())
	{
		HandleIncomingHeal(Data);
	}
}

void UOrbGameAttributeSet::HandleIncomingDamage(const FGameplayEffectModCallbackData& Data)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
        UE_LOG(LogTemp, Warning, TEXT("Target: %s,  Damage taken: %f,  Health changed to: %f"), *GetOwningActor()->GetName(), LocalIncomingDamage, GetHealth());
    }
}

void UOrbGameAttributeSet::HandleIncomingHeal(const FGameplayEffectModCallbackData& Data)
{
	const float LocalIncomingHeal = GetIncomingHeal();
	SetIncomingHeal(0.f);
	if (LocalIncomingHeal > 0.f)
	{
		const float NewHealth = GetHealth() + LocalIncomingHeal;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
        UE_LOG(LogTemp, Warning, TEXT("Target: %s,  Heal received: %f,  Health changed to: %f"), *GetOwningActor()->GetName(), LocalIncomingHeal, GetHealth());
    }
}

void UOrbGameAttributeSet::InitHealthToMaxHealth()
{
	SetHealth(GetMaxHealth());
}