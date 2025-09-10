// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/GAS/OrbGameGameplayAbility.h"
#include "ProjectileGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UProjectileGameplayAbility : public UOrbGameGameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Gameplay Ability")
	float ProjectileSpeed = 1000.0f;
	
};
