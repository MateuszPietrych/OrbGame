// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/ObjectMultiPool.h"
#include "OrbMultiPool.generated.h"

class AOrb;

/**
 * 
 */
UCLASS()
class ORBGAME_API UOrbMultiPool : public UObjectMultiPool
{
	GENERATED_BODY()

public:
	TObjectMultiPool<AOrb> Pool;
	
	void Initialize(FItemSet<FGameplayTag> OrbTags);
	AOrb* GetOrbFromPool(FGameplayTag Tag);
	void ReturnOrbToPool(AOrb* Orb);
	TSubclassOf<AOrb> GetOrbClassByTag(FGameplayTag Tag);

};
