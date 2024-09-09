// Fill out your copyright notice in the Description page of Project Settings.


#include "FireOrb.h"
#include "LineEffect.h"
#include "GameFramework/ProjectileMovementComponent.h"


AFireOrb::AFireOrb()
{
    LineEffect = CreateDefaultSubobject<ULineEffect>(TEXT("ULineEffect"));
}

void AFireOrb::ActivateEffect()
{
    Super::ActivateEffect();

    UE_LOG(LogTemp, Warning, TEXT("FireOrb::ActivateEffect"));
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
    LineEffect->ApplyEffect(OrbEffectsData[0]);
    
}
