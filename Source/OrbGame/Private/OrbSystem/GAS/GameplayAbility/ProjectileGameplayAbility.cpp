// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/GameplayAbility/ProjectileGameplayAbility.h"
#include "Actor/OrbGameProjectile.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

#include "AbilitySystemComponent.h"

void UProjectileGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UProjectileGameplayAbility::SpawnProjectile(const FVector& ProjectileTargetLocation, const FVector& StartLocation)
{
    if (!ProjectileClass) return;

    FTransform SpawnTransform;
    SpawnTransform.SetLocation(StartLocation);
    SpawnTransform.SetRotation((ProjectileTargetLocation - StartLocation).Rotation().Quaternion());

    AOrbGameProjectile* Projectile = GetWorld()->SpawnActor<AOrbGameProjectile>(
		ProjectileClass,
		SpawnTransform);

    AActor* AvatarActor = GetAvatarActorFromActorInfo();
	
    if (Projectile)
    {
        SetBaseDamageParams();
        Projectile->DamageEffectParams = DamageEffectParams;
    }
}