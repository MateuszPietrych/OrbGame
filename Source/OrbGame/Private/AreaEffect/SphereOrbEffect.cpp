// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaEffect/SphereOrbEffect.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h" 


void USphereOrbEffect::ApplyEffect(AActor* HitActor)
{
    Super::ApplyEffect(HitActor);
}

TArray<AActor*> USphereOrbEffect::GetActorsAffected()
{
    FVector EndLocation = StartLocation;
    FQuat SphereRotation = FQuat::Identity; // No rotation for sphere
    if(bUseDebug)
    {
        DrawDebugSphere(GetWorld(), StartLocation, Radius, 12, FColor::Red, true, 10.0f, 0, 5.0f);
    }

    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    FCollisionShape SphereShape = FCollisionShape::MakeSphere(Radius);
    FCollisionResponseParams ResponseParams;
    GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, SphereRotation, ECollisionChannel::ECC_Pawn, SphereShape, CollisionQueryParams, ResponseParams);

    TArray<AActor*> AffectedActors;
    AffectedActors.SetNum(HitResults.Num());

    UE_LOG(LogTemp, Warning, TEXT("Sphere Overlap with %d actors"), HitResults.Num());

    std::transform(HitResults.GetData(), HitResults.GetData() + HitResults.Num(), AffectedActors.GetData(), [](const FHitResult& HitResult) {
        return HitResult.GetActor();
    });

    return AffectedActors; 
}