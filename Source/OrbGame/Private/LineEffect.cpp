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

ULineEffect::~ULineEffect()
{
}

void ULineEffect::ApplyEffect(FOrbEffectData OrbEffectData)
{
    FVector StartLocation = OrbEffectData.VectorParams[OrbEffectsVectorParams::START_LOCATION];
    FVector Direction = OrbEffectData.VectorParams[OrbEffectsVectorParams::DIRECTION];
    float Range = OrbEffectData.FloatParams[OrbEffectsFloatParams::RANGE];

    FVector EndLocation = StartLocation + Direction * Range;

    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1, 0, 1);
}
