// Fill out your copyright notice in the Description page of Project Settings.


#include "FireOrb.h"
#include "LineEffect.h"


AFireOrb::AFireOrb()
{
    LineEffect = CreateDefaultSubobject<ULineEffect>(TEXT("ULineEffect"));
}

void AFireOrb::ActivateEffect()
{
    Super::ActivateEffect();

    UE_LOG(LogTemp, Warning, TEXT("FireOrb::ActivateEffect"));
    UE_LOG(LogTemp, Warning, TEXT("OrbEffectsData.Num() = %d"), OrbEffectsData.Num());
    if(OrbEffectsData.Num() == 0)
    {
        return;
    }

    FVector Direction = GetActorForwardVector(); 
    FVector StartLocation = GetActorLocation();

    for(FOrbEffectData& OrbEffectData : OrbEffectsData)
    {
        OrbEffectData.VectorParams.Add(OrbEffectsVectorParams::START_LOCATION, StartLocation);
        OrbEffectData.VectorParams.Add(OrbEffectsVectorParams::DIRECTION, Direction);
    }
     
    if(LineEffect == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("LineEffect is null"));
        return;
    }
    LineEffect->ApplyEffect(OrbEffectsData[0]);
    
}
