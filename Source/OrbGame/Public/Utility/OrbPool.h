// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "Chaos/ObjectPool.h"
#include "OrbGameStructs.h"
#include "GameplayTagContainer.h"
#include "OrbPool.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UOrbPool : public UObject
{
	GENERATED_BODY()

public:
	
	TMap<FGameplayTag, Chaos::TObjectPool<class AOrb*>*> OrbPoolByTag;

	AOrb* GetOrbFromPool(FGameplayTag OrbTag);

	void ReturnOrbToPool(AOrb* Orb, FGameplayTag OrbTag);

	void Initialize(FItemSet<FGameplayTag> OrbTags);
};
