// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/GameplayAbility/ElectricFanceGameplayAbility.h"
#include "OrbSystem/OrbManager.h"
#include "OrbSystem/Orb/Orb.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OrbGameBlueprintLibrary.h"





void UElectricFanceGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    SetBaseDamageParams();

    GetWorld()->GetTimerManager().SetTimer(ElectricFanceTickTimerHandle, this, &UElectricFanceGameplayAbility::TickElectricFance, TickInterval, true);
    GetWorld()->GetTimerManager().SetTimer(ElectricFanceEndAbilityTimerHandle, this, &UElectricFanceGameplayAbility::EndFance, AbilityTime, false);
}



void UElectricFanceGameplayAbility::TickElectricFance_Implementation()
{
    TArray<AActor*> AffectedActors = GetAffectedActors();
    UOrbGameBlueprintLibrary::DealDamageToActors(DamageEffectParams, AffectedActors);
}

TArray<AActor*> UElectricFanceGameplayAbility::GetAffectedActors() const
{
    TArray<AActor*> AffectedActors;

    // Logic to determine affected components goes here
    UOrbManager* OrbManager = OrbUseContext.OrbManager;
    if (OrbManager)
    {
        TArray<FOrbLevelData> OrbLevelsData = OrbManager->GetOrbLevelsData();
        for (const FOrbLevelData& LevelData : OrbLevelsData)
        {
            if(LevelData.Orbs.Num() <= 1) continue;

            TArray<AOrb*> AffectedOrbs;
            for (AOrb* Orb : LevelData.Orbs)
            {
                if (Orb && Orb->IsA(AffectedOrbClass))
                {
                    AffectedOrbs.Add(Orb);
                }
            }

            int FirstOrbIndex = 0;
            int LastOrbIndex = AffectedOrbs.Num() - 1;

            for (int i = FirstOrbIndex; i <= LastOrbIndex; ++i)
            {
                AOrb* OrbA = i < LastOrbIndex ? AffectedOrbs[i] : AffectedOrbs[FirstOrbIndex];
                AOrb* OrbB = i < LastOrbIndex ? AffectedOrbs[i + 1] : AffectedOrbs[LastOrbIndex];

                if (!OrbA || !OrbB) return AffectedActors;

                TArray<AActor*> HitActors = GetAffectedActorsByOrbs(OrbA, OrbB);
                for (AActor* HitActor : HitActors)
                {
                    if (!AffectedActors.Contains(HitActor))
                    {
                        AffectedActors.Add(HitActor);
                    }
                }
            }

        }
    }

    return AffectedActors;
}

TArray<AActor*> UElectricFanceGameplayAbility::GetAffectedActorsByOrbs(AOrb* OrbA, AOrb* OrbB) const
{
    TArray<AActor*> AffectedActors;
    
    FVector Start = OrbA->GetOrbWorldLocation();
    FVector End   = OrbB->GetOrbWorldLocation();

    float SphereRadius = BeamThicknessRadius;

    ECollisionChannel TraceChannel = ECC_Visibility;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OrbA);
    QueryParams.AddIgnoredActor(OrbB);
    QueryParams.bTraceComplex = false;

    TArray<FHitResult> Hits;

    bool bHit = GetWorld()->SweepMultiByChannel(
        Hits,
        Start,
        End,
        FQuat::Identity, // rotation irrelevant for a sphere
        TraceChannel,
        FCollisionShape::MakeSphere(SphereRadius),
        QueryParams
    );

    // --- Debug visualization ---
    FColor DebugColor = bHit ? FColor::Red : FColor::Green;
    DrawDebugLine(GetWorld(), Start, End, DebugColor, false, 0.05f, 0, 1.5f);
    DrawDebugSphere(GetWorld(), Start, SphereRadius, 12, DebugColor, false, 0.05f);
    DrawDebugSphere(GetWorld(), End, SphereRadius, 12, DebugColor, false, 0.05f);

    // // --- Process all hits ---
    if (bHit)
    {
        for (const FHitResult& Hit : Hits)
        {
            if (AActor* HitActor = Hit.GetActor())
            {
                AffectedActors.Add(HitActor);
            }
        }
    }
    return AffectedActors;
}

void UElectricFanceGameplayAbility::EndFance()
{
    GetWorld()->GetTimerManager().ClearTimer(ElectricFanceTickTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(ElectricFanceEndAbilityTimerHandle);
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}