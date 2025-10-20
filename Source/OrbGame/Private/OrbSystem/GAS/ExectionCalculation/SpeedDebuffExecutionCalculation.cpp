// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/ExectionCalculation/SpeedDebuffExecutionCalculation.h"
#include "OrbSystem/GAS/OrbGameAttributeSet.h"
#include "GameplayEffect.h"
#include "OrbSystem/OrbManager.h"
#include "OrbSystem/GAS/OrbUserAbilitySystemComponent.h"
#include "ScalableFloat.h"

struct OrbGameSpeedDebuffStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Speed);

    OrbGameSpeedDebuffStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UOrbGameAttributeSet, Speed, Target, false);
    }
};

static const OrbGameSpeedDebuffStatics& SpeedDebuffStatics()
{
	static OrbGameSpeedDebuffStatics SDStatics;
	return SDStatics;
}

USpeedDebuffExecutionCalculation::USpeedDebuffExecutionCalculation()
{
    RelevantAttributesToCapture.Add(SpeedDebuffStatics().SpeedDef);
}

void USpeedDebuffExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    const UOrbUserAbilitySystemComponent* TargetASC = Cast<UOrbUserAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent());
    if(!TargetASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpeedDebuffExecutionCalculation: TargetASC is not UOrbUserAbilitySystemComponent"));
        return;
    }

    float Speed = 0.0f;
    FAggregatorEvaluateParameters EvaluationParameters;
    ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(SpeedDebuffStatics().SpeedDef, Speed);

    if(Speed <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpeedDebuffExecutionCalculation: Target speed is non-positive: %f"), Speed);
        return;
    }


    UOrbManager* OrbManager = TargetASC->GetOrbManager();
    if(!OrbManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpeedDebuffExecutionCalculation: OrbManager is null"));
        return;
    }

    int OrbsCount = OrbManager->GetTotalOrbsCount();
    FScalableFloat SpeedDebuffPercent = TargetASC->GetSpeedDebuffPercent();
    float TotalSpeedDebuffPercent = SpeedDebuffPercent.GetValueAtLevel(OrbsCount);
    float FinalMultiplier = 1.0f - TotalSpeedDebuffPercent / 100.0f;
    float FinalSpeed = Speed * FinalMultiplier - Speed;

    const FGameplayModifierEvaluatedData EvaluatedData(UOrbGameAttributeSet::GetSpeedAttribute(),
        EGameplayModOp::Additive, FinalSpeed);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
