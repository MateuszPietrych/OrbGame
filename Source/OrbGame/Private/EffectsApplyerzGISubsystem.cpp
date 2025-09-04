// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectsApplyerzGISubsystem.h"
#include "OrbSystem/OrbEffect/OrbEffectActionConfig.h"
#include "Kismet/GameplayStatics.h" 


UEffectsApplyerzGISubsystem::UEffectsApplyerzGISubsystem()
{
    // OnEffectApply.AddDynamic(this, &UEffectsApplyerzGISubsystem::HandleEffectApply);
    OnDamageDealt.AddDynamic(this, &UEffectsApplyerzGISubsystem::HandleDamageDealt);
}

void UEffectsApplyerzGISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("[GISubsystem] Initialize"));
}

void UEffectsApplyerzGISubsystem::Deinitialize()
{
    UE_LOG(LogTemp, Log, TEXT("[GISubsystem] Deinitialize"));
    Super::Deinitialize();
}

void UEffectsApplyerzGISubsystem::DoSomething()
{
UE_LOG(LogTemp, Warning, TEXT("EffectsApplyerzGISubsystem is doing something!"));
}

// void UEffectsApplyerzGISubsystem::HandleEffectApply(AActor* HitActor, UOrbEffectActionConfig* EffectConfig)
// {

// }

void UEffectsApplyerzGISubsystem::HandleDamageDealt(AActor* Target, float Damage)
{
    UE_LOG(LogTemp, Log, TEXT("[GISubsystem] HandleDamageDealt"));
    UGameplayStatics::ApplyDamage(Target, Damage, nullptr, nullptr, nullptr);
}