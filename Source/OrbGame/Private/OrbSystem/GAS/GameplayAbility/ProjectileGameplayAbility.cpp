// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/GameplayAbility/ProjectileGameplayAbility.h"
#include "Actor/OrbGameProjectile.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

#include "AbilitySystemComponent.h"
#include "OrbSystem/GAS/AbilityDataAsset.h"
#include "OrbSystem/GAS/OrbUserAbilitySystemComponent.h"

void UProjectileGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
  // SetActorInfo(ActorInfo->AvatarActor.Get(), ActorInfo->OwnerActor.Get());

	
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
        DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
        if(!DamageEffectParams.SourceAbilitySystemComponent)
        {
            UE_LOG(LogTemp, Warning, TEXT("UProjectileGameplayAbility::SpawnProjectile: SourceAbilitySystemComponent is null"));
        }
        DamageEffectParams.WorldContextObject = GetWorld();
        UOrbUserAbilitySystemComponent* OrbUserASC = Cast<UOrbUserAbilitySystemComponent>(DamageEffectParams.SourceAbilitySystemComponent);
        if(OrbUserASC)
        {
            DamageEffectParams.AbilityLevel = OrbUserASC->GetAbilityLevel(AbilityDataAsset->AbilityTag);
        }
        else
        {
            DamageEffectParams.AbilityLevel = 1;
        }
        Projectile->DamageEffectParams = DamageEffectParams;
    }
}