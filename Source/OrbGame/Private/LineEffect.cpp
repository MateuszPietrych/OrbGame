// Fill out your copyright notice in the Description page of Project Settings.


#include "LineEffect.h"
#include "Enums.h"
#include "Structures.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"



ULineEffect::ULineEffect()
{
}

// ULineEffect::~ULineEffect()
// {
// }

void ULineEffect::ApplyEffect(FOrbEffectData OrbEffectData)
{

    if(!OrbEffectData.VectorParams.Contains(OrbEffectsVectorParams::START_LOCATION) || 
        !OrbEffectData.VectorParams.Contains(OrbEffectsVectorParams::DIRECTION) || 
        !OrbEffectData.FloatParams.Contains(OrbEffectsFloatParams::RANGE))
    {
        return;
    }

    FVector StartLocation = OrbEffectData.VectorParams[OrbEffectsVectorParams::START_LOCATION];
    FVector Direction = OrbEffectData.VectorParams[OrbEffectsVectorParams::DIRECTION];
    float Range = OrbEffectData.FloatParams[OrbEffectsFloatParams::RANGE];

    FVector EndLocation = StartLocation + Direction * Range;
    UE_LOG(LogTemp, Warning, TEXT("LineEffect::ApplyEffect"));
    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 20, 0, 100.0f);
}

AActor* ULineEffect::GetActorsEffected()
{
    return nullptr;
}