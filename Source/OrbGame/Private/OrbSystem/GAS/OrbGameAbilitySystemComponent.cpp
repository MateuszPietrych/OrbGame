// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/OrbGameAbilitySystemComponent.h"
#include "OrbSystem/GAS/OrbGameAttributeSet.h"
#include "OrbSystem/GAS/EffectStateManager.h"



void UOrbGameAbilitySystemComponent::BeginPlay()
{
    Super::BeginPlay();
    EffectStateManager = NewObject<UEffectStateManager>(this, UEffectStateManager::StaticClass());
    EffectStateManager->Initialize(this);
}

void UOrbGameAbilitySystemComponent::InitializeAttributesDelegate(UOrbGameAttributeSet* AttributeSet)
{
    for (auto& Pair : AttributeSet->TagsToAttributes)
	{
		GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
            [this, Pair, AttributeSet](const FOnAttributeChangeData& Data)
            {
                BroadcastAttributeInfo(Pair.Key, Pair.Value(), AttributeSet);
            }
        );
    }
}

void UOrbGameAbilitySystemComponent::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute, UOrbGameAttributeSet* AttributeSet) const
{
	OnAttributeChanged.Broadcast(AttributeTag, Attribute.GetNumericValue(AttributeSet));
}
