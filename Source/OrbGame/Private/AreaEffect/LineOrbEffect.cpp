// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaEffect/LineOrbEffect.h"
#include "Enums.h"
#include "Structures.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h" 
#include "Engine/World.h"


void ULineOrbEffect::ApplyEffect(AActor* HitActor)
{
    Super::ApplyEffect(HitActor);
}

TArray<AActor*> ULineOrbEffect::GetActorsAffected()
{
    FVector EndLocation = StartLocation + Direction * Range;
    if(bUseDebug)
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