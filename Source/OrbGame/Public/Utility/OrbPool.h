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

class AOrb;

UCLASS()
class ORBGAME_API UOrbPool : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	TMap<FGameplayTag, class UOrbGameObjectPool*> OrbPoolByTag;

	AOrb* GetOrbFromPool(FGameplayTag OrbTag);

	void ReturnOrbToPool(AOrb* Orb);

	void Initialize(FItemSet<FGameplayTag> OrbTags);

	TSubclassOf<AOrb> GetOrbClassByTag(FGameplayTag OrbTag);
};
