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

    if(!OrbEffectData.FloatParams.Contains(OrbEffectsFloatParams::DAMAGE))
    {
        UE_LOG(LogTemp, Warning, TEXT("LineEffect::ApplyEffect: Missing parameters"));
        return;
    }

   TArray<AActor*> AffectedActors = GetActorsAffected(OrbEffectData);

    for(AActor* HitActor : AffectedActors)
    {
        if(HitActor == nullptr)
        {
            continue;
        }
        UGameplayStatics::ApplyDamage(HitActor, OrbEffectData.FloatParams[OrbEffectsFloatParams::DAMAGE], nullptr, nullptr, nullptr);
    }

}

TArray<AActor*> ULineEffect::GetActorsAffected(FOrbEffectData OrbEffectData)
{
    if(!OrbEffectData.VectorParams.Contains(OrbEffectsVectorParams::START_LOCATION) || 
        !OrbEffectData.VectorParams.Contains(OrbEffectsVectorParams::DIRECTION) || 
        !OrbEffectData.FloatParams.Contains(OrbEffectsFloatParams::RANGE) ||
        !OrbEffectData.BoolParams.Contains(OrbEffectsBoolParams::USE_DEBUG))
    {
        UE_LOG(LogTemp, Error, TEXT("LineEffect::GetActorsAffected: Missing parameters"));
        return TArray<AActor*>();
    }
    FVector StartLocation = OrbEffectData.VectorParams[OrbEffectsVectorParams::START_LOCATION];
    FVector Direction = OrbEffectData.VectorParams[OrbEffectsVectorParams::DIRECTION];
    float Range = OrbEffectData.FloatParams[OrbEffectsFloatParams::RANGE];

    FVector EndLocation = StartLocation + Direction * Range;
    if(OrbEffectData.BoolParams[OrbEffectsBoolParams::USE_DEBUG])
    {
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.1f, 0, 20.0f);
    }

    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    GetWorld()->LineTraceMultiByChannel(HitResults, StartLocation, EndLocation, ECollisionChannel::ECC_Pawn, CollisionQueryParams);

    TArray<AActor*> AffectedActors;
    AffectedActors.SetNum(HitResults.Num());

    std::transform(HitResults.GetData(), HitResults.GetData() + HitResults.Num(), AffectedActors.GetData(), [](const FHitResult& HitResult) {
        return HitResult.GetActor();
    });

    return AffectedActors;
}