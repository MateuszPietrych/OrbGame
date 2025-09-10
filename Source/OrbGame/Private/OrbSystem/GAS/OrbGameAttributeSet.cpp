// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/OrbGameAttributeSet.h"

#include "GameplayEffectExtension.h"


UOrbGameAttributeSet::UOrbGameAttributeSet()
{

}

void UOrbGameAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute called for attribute: %s"), *Data.EvaluatedData.Attribute.GetName());
    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Data);
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
        UE_LOG(LogTemp, Warning, TEXT("Health changed to: %f"), GetHealth());
    }
}