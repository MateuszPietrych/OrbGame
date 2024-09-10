// Fill out your copyright notice in the Description page of Project Settings.


#include "FireOrb.h"
#include "LineEffect.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AFireOrb::AFireOrb()
{
    LineEffect = CreateDefaultSubobject<ULineEffect>(TEXT("ULineEffect"));
}

void AFireOrb::ActivateLongUsageEffect()
{
    Super::ActivateLongUsageEffect();
}

void AFireOrb::LongUseTickEffect()
{
    Super::LongUseTickEffect();

    if(OrbEffectsData.Num() == 0)
    {
        return;
    }
    SetBaseParamsForOrbEffect();

    if(LineEffect == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("LineEffect is null"));
        return;
    }

    TArray<AActor*> AffectedActors = LineEffect->GetActorsAffected(OrbEffectsData[0]);
    for(AActor* HitActor : AffectedActors)
    {
        if(HitActor == nullptr)
            continue;
        UGameplayStatics::ApplyDamage(HitActor, OrbEffectsData[0].FloatParams[OrbEffectsFloatParams::DAMAGE], nullptr, nullptr, nullptr);
    }
}
