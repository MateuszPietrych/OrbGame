// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "OrbGameGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UOrbGameGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	

	UPROPERTY(EditDefaultsOnly, Category="Gameplay Ability")
	FGameplayTag StartGameplayTag;

public:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Ability")
	class UOrbEffectBase* OrbEffectInstance;
};
