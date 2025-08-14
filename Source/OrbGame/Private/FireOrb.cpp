// Fill out your copyright notice in the Description page of Project Settings.


#include "FireOrb.h"
#include "LineEffect.h"
#include "LineOrbEffect.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AFireOrb::AFireOrb()
{
    LineEffectInstance = CreateDefaultSubobject<ULineOrbEffect>(TEXT("LineEffectInstance"));
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

    FVector Direction = GetActorForwardVector();
    Direction.Normalize();
    FVector StartLocation = GetOrbWorldLocation();

	LineEffectInstance->SetStartLocation(StartLocation);
	LineEffectInstance->SetDirection(Direction);

    if(LineOrbEffect == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("LineEffect is null"));
        return;
    }

    TArray<AActor*> AffectedActors = LineEffectInstance->GetActorsAffected(OrbEffectsData[0]);
    for(AActor* HitActor : AffectedActors)
    {
        if(HitActor == nullptr)
            continue;
        UGameplayStatics::ApplyDamage(HitActor, LineEffectInstance->Damage, nullptr, nullptr, nullptr);
        UE_LOG(LogTemp, Warning, TEXT("Applying damage to %s"), *HitActor->GetName());
    }
}
