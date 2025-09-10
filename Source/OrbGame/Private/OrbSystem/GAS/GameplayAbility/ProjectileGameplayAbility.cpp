// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/GameplayAbility/ProjectileGameplayAbility.h"
#include "Actor/OrbGameProjectile.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


void UProjectileGameplayAbility::SpawnProjectile(const FVector& ProjectileTargetLocation, const FVector& StartLocation)
{
    if (!ProjectileClass) return;

    FTransform SpawnTransform;
    SpawnTransform.SetLocation(StartLocation);
    SpawnTransform.SetRotation((ProjectileTargetLocation - StartLocation).Rotation().Quaternion());

    AOrbGameProjectile* Projectile = GetWorld()->SpawnActor<AOrbGameProjectile>(
		ProjectileClass,
		SpawnTransform);
	
    if (Projectile)
    {
        DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
        DamageEffectParams.WorldContextObject = GetWorld();
        Projectile->DamageEffectParams = DamageEffectParams;
    }
}