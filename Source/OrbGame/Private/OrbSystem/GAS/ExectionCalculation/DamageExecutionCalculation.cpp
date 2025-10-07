// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/ExectionCalculation/DamageExecutionCalculation.h"
#include "OrbSystem/GAS/OrbGameAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Kismet/GameplayStatics.h"
#include "OrbGameGameplayTags.h"


struct OrbGameDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);

    OrbGameDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UOrbGameAttributeSet, Armor, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UOrbGameAttributeSet, Strength, Source, false);
    }
};

static const OrbGameDamageStatics& DamageStatics()
{
	static OrbGameDamageStatics DStatics;
	return DStatics;
}

UDamageExecutionCalculation::UDamageExecutionCalculation()
{
    RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

    float BaseDamage = Spec.GetSetByCallerMagnitude(FOrbGameGameplayTags::Get().Effect_Damage, false);
    float Armor = 0.0f;
    float Strength = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().StrengthDef, EvaluationParameters, Strength);

    float ArmorModifier = 1.0f;
    if (Armor > 0.0f)
    {
        ArmorModifier *= (1.0f - Armor / 100.0f); 
    }

    float StrengthModifier = 1.0f;
    if (Strength > 0.0f)
    {
        StrengthModifier *= (1.0f + Strength / 100.0f); 
    }

    float FinalDamage = BaseDamage * ArmorModifier * StrengthModifier;

    UE_LOG(LogTemp, Warning, TEXT("BaseDamage: %f, Armor: %f, ArmorModifier: %f, FinalDamage: %f"), BaseDamage, Armor, ArmorModifier, FinalDamage);

    const FGameplayModifierEvaluatedData EvaluatedData(UOrbGameAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, FinalDamage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}