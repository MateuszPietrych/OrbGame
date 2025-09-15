// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/GAS/GameplayAbility/DamageGameplayAbility.h"
#include "ProjectileGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UProjectileGameplayAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FVector& StartLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AOrbGameProjectile> ProjectileClass;

private:
	
	
};
