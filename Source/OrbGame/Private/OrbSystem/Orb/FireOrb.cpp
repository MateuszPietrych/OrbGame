// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/Orb/FireOrb.h"
#include "AreaEffect/LineOrbEffect.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AFireOrb::AFireOrb()
{

}

void AFireOrb::BeginPlay()
{
	Super::BeginPlay();
    // create object from class LineOrbEffect
    LineEffectInstance = NewObject<ULineOrbEffect>(this, LineOrbEffect);
    UE_LOG(LogTemp, Warning, TEXT("LineEffectInstance is created"));
}

void AFireOrb::ActivateLongUsageEffect()
{
    Super::ActivateLongUsageEffect();
}

void AFireOrb::LongUseTickEffect()
{   
    Super::LongUseTickEffect();

    // FVector Direction = GetActorForwardVector();
    // Direction.Normalize();
    // FVector StartLocation = GetOrbWorldLocation();

    // UE_LOG(LogTemp, Warning, TEXT("LineEffect Start Location: %s"), *StartLocation.ToString());
    // UE_LOG(LogTemp, Warning, TEXT("LineEffect Direction: %s"), *Direction.ToString());
    // UE_LOG(LogTemp, Warning, TEXT("LineEffect Instance 1: %s"), *LineEffectInstance->GetName());

	// SetBaseParamsForOrbEffect(LineEffectInstance);

    // UE_LOG(LogTemp, Warning, TEXT("LineEffect Instance 2: %s"), *LineEffectInstance->GetName());

    // if(LineOrbEffect == nullptr)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("LineEffect is null"));
    //     return;
    // }

    // LineEffectInstance->ApplyEffectToAffectedActors();
    // TArray<AActor*> AffectedActors = LineEffectInstance->GetActorsAffected(OrbEffectsData[0]);
    // for(AActor* HitActor : AffectedActors)
    // {
    //     if(HitActor == nullptr)
    //         continue;
    //     UGameplayStatics::ApplyDamage(HitActor, LineEffectInstance->Damage, nullptr, nullptr, nullptr);
    //     UE_LOG(LogTemp, Warning, TEXT("Applying damage to %s"), *HitActor->GetName());
    // }
}
