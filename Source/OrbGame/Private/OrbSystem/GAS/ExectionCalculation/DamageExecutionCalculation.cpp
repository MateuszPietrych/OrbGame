// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/ExectionCalculation/DamageExecutionCalculation.h"
#include "OrbSystem/GAS/OrbGameAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Kismet/GameplayStatics.h"


struct OrbGameDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

    OrbGameDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UOrbGameAttributeSet, Armor, Target, false);
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

    float BaseDamage = 100.f; // Spec.GetSetByCallerMagnitude(Pair.Key, false);
    float Armor = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);

    float ArmorModifier = 1.0f;
    if (Armor > 0.0f)
    {
        // Use the armor data for damage calculation
        ArmorModifier *= (1.0f - Armor / 100.0f); // Example: Reduce damage by armor percentage
    }


    // Apply the final damage calculation
    float FinalDamage = BaseDamage * ArmorModifier;

    UE_LOG(LogTemp, Warning, TEXT("BaseDamage: %f, Armor: %f, ArmorModifier: %f, FinalDamage: %f"), BaseDamage, Armor, ArmorModifier, FinalDamage);

    const FGameplayModifierEvaluatedData EvaluatedData(UOrbGameAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, FinalDamage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}