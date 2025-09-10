// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/GAS/OrbGameGameplayAbility.h"
#include "OrbGameStructs.h"
#include "DamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UDamageGameplayAbility : public UOrbGameGameplayAbility
{
	GENERATED_BODY()

public:
	void CauseDamage(AActor* TargetActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OrbGame|Damage")
	FDamageEffectParams DamageEffectParams;
};
